#include "my_hashgen.hpp"

HashGen::HashGen(const int num_threads_hash, const int num_threads_sort, const int num_threads_write, const char* filename, const int memory_size, const int filesize, bool debug_model)
{
    // command line
    this->num_threads_hash = num_threads_hash;
    this->num_threads_sort = num_threads_sort;
    this->num_threads_write = num_threads_write;
    std::strcpy(this->filename, filename);
    this->memory_size = memory_size;
    this->filesize = filesize;
    this->debug_model = debug_model;
}

HashGen::~HashGen()
{
}

void HashGen::initScheduleList()
{
    uint32_t unit = 1024 / sizeof(Record);
    uint32_t total_memory_items = this->memory_size * 1024 * unit;      // The max memory size should less than 64GB (65536 MB)
    int schedule_size = std::ceil((float)this->filesize / this->memory_size);
    this->schedule.resize(schedule_size, total_memory_items);
    if(this->filesize % this->memory_size != 0)
    {
        this->schedule[schedule_size - 1] = (this->filesize % this->memory_size) * 1024 * unit;
    }
}

void HashGen::initMemoryBuffer()
{
    uint32_t unit = 1024 / sizeof(Record);
    uint32_t total_memory_items = this->memory_size * 1024 * unit;
    this->memory_buffer.resize(total_memory_items);
}

void HashGen::init()
{
    this->initScheduleList();  // create memory buffer
    this->initMemoryBuffer();  // initial scheduleList

    this->hashGen_status_vector.resize(this->num_threads_hash, false);
    this->hashSort_status_vector.resize(this->num_threads_sort, false);
    this->hashWrite_status_vector.resize(this->num_threads_write, false);

    this->hashGen_permit_vector.resize(this->schedule.size(), false);
    this->hashSort_permit_vector.resize(this->schedule.size(), false);
    this->hashWrite_permit_vector.resize(this->schedule.size(), false);
}

void HashGen::hashGen(int hashGenThreadID, uint32_t begin, uint32_t end)
{
    HashInterface hashInterface;
    for(uint32_t i=begin; i<end; i++)
    {
        hashInterface.getHash(&this->memory_buffer[i]);
    }
}

void HashGen::hashGenThread(int hashGenThreadID)
{
    uint32_t scheduleID = 0;
    while(true)
    {
        // wait permit signal
        std::mutex mtx;
        std::unique_lock<std::mutex> lck(mtx);
        this->cv_hashGen_permit.wait(lck, [&]{return this->hashGen_permit_vector[scheduleID];});

        uint32_t batchsize = this->scheduled_size / this->num_threads_hash;
        uint32_t begin = hashGenThreadID * batchsize;
        uint32_t end = (hashGenThreadID + 1) * batchsize;
        if(hashGenThreadID == this->num_threads_sort - 1)
            end = this->scheduled_size;

        // do the task
        this->hashGen(hashGenThreadID, begin, end);
        scheduleID++;

        // send finish signal
        this->hashGen_status_vector[hashGenThreadID] = true;
        this->cv_hashGen_finish.notify_all();

        if(scheduleID == this->hashGen_permit_vector.size())
            break;
    }
}

bool my_cmp(const Record &a, const Record &b)
{
    if(a.hash[0] < b.hash[0])
        return true;
    else if(a.hash[0] > b.hash[0])
        return false;
    else if(a.hash[1] < b.hash[1])
        return true;
    else if(a.hash[1] > b.hash[1])
        return false;
    else if(a.hash[2] < b.hash[2])
        return true;
    else if(a.hash[2] > b.hash[2])
        return false;
    else if(a.hash[3] < b.hash[3])
        return true;
    else if(a.hash[3] > b.hash[3])
        return false;
    else if(a.hash[4] < b.hash[4])
        return true;
    else if(a.hash[4] > b.hash[4])
        return false;
    else if(a.hash[5] < b.hash[5])
        return true;
    else if(a.hash[5] > b.hash[5])
        return false;
    else if(a.hash[6] < b.hash[6])
        return true;
    else if(a.hash[6] > b.hash[6])
        return false;
    else if(a.hash[7] < b.hash[7])
        return true;
    else if(a.hash[7] > b.hash[7])
        return false;
    else if(a.hash[8] < b.hash[8])
        return true;
    else if(a.hash[8] > b.hash[8])
        return false;
    else if(a.hash[9] < b.hash[9])
        return true;
    else if(a.hash[9] > b.hash[9])
        return false;
    else
        return true;
}

void HashGen::hashSort(int hashSortThreadID, uint32_t begin, uint32_t end)
{
    std::sort(this->memory_buffer.begin()+begin, this->memory_buffer.begin()+end, my_cmp);

    std::unique_lock<std::mutex> lck(this->mtx_sort);
    this->sorted_range_queue.push({begin, end});
    lck.unlock();
}


void HashGen::hashSortThread(int hashSortThreadID)
{
    uint32_t scheduleID = 0;
    while(true)
    {
        // wait permit signal
        std::mutex mtx;
        std::unique_lock<std::mutex> lck(mtx);
        this->cv_hashSort_permit.wait(lck, [&]{return this->hashSort_permit_vector[scheduleID];});

        // initial
        uint32_t batchsize = this->scheduled_size / this->num_threads_sort;
        uint32_t begin = hashSortThreadID * batchsize;
        uint32_t end = (hashSortThreadID + 1) * batchsize;
        if(hashSortThreadID == this->num_threads_sort - 1)
            end = this->scheduled_size;

        // do the task
        this->hashSort(hashSortThreadID, begin, end);
        scheduleID++;

        // send finish signal
        this->hashSort_status_vector[hashSortThreadID] = true;
        this->cv_hashSort_finish.notify_all();

        if(scheduleID == this->hashSort_permit_vector.size())
            break;
    }
}

void HashGen::hashWrite(int hashWriteThreadID, uint32_t begin, uint32_t end, int fileID)
{
    std::string filename = "sorted_data_part_" + std::to_string(fileID+1) + ".bin";
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    file.write(reinterpret_cast<const char*>(&this->memory_buffer[begin]), (end - begin) * sizeof(Record));
    file.close();
}


void HashGen::hashWriteThread(int hashWriteThreadID)
{
    uint32_t scheduleID = 0;
    while(true)
    {
        std::unique_lock<std::mutex> lck(this->mtx_file);
        this->cv_hashWrite_permit.wait(lck, [&]{return this->hashWrite_permit_vector[scheduleID];});

        while(true)
        {
            std::unique_lock<std::mutex> lck(this->mtx_write);
            if(this->sorted_range_queue.empty())
            {
                scheduleID++;
                this->hashWrite_status_vector[hashWriteThreadID] = true;
                this->cv_hashWrite_finish.notify_all();
                break;
            }
            int fileID = this->fileID;
            uint32_t begin = this->sorted_range_queue.front().first;
            uint32_t end = this->sorted_range_queue.front().second;
            this->sorted_range_queue.pop();
            this->fileID++;
            lck.unlock();
            this->hashWrite(hashWriteThreadID, begin, end, fileID);
        }

        if(scheduleID == this->hashWrite_permit_vector.size())
            break;
    }
}

bool HashGen::checkStatus(std::vector<bool>& v)
{
    for(uint32_t i=0; i<v.size(); i++)
    {
        if(v[i] == false)
            return false;
    }

    return true;
}

void HashGen::hashMonitor()
{
    // need waitting code
    while(true)
    {
        // initalize
        this->scheduled_size = this->schedule[this->scheduleID];
        this->hashGen_status_vector.assign(this->num_threads_hash, false);
        this->hashSort_status_vector.assign(this->num_threads_sort, false);
        this->hashWrite_status_vector.assign(this->num_threads_write, false);

        // send hashGen permission signal
        this->hashGen_permit_vector[this->scheduleID] = true;
        this->cv_hashGen_permit.notify_all();

        // wait for hashGen finished signal
        std::mutex mtx_hashGen_finish;
        std::unique_lock<std::mutex> lck_hashGen_finsh(mtx_hashGen_finish);
        this->cv_hashGen_finish.wait(lck_hashGen_finsh, [this]{return this->checkStatus(this->hashGen_status_vector);});

        // send hashSort permission signal
        this->hashSort_permit_vector[this->scheduleID] = true;
        this->cv_hashSort_permit.notify_all();

        // wait for hashGen finished signal
        std::mutex mtx_hashSort_finish;
        std::unique_lock<std::mutex> lck_hashSort_finsh(mtx_hashSort_finish);
        this->cv_hashSort_finish.wait(lck_hashSort_finsh, [this]{return this->checkStatus(this->hashSort_status_vector);});
        
        // send hashWrite premission signal
        this->hashWrite_permit_vector[this->scheduleID] = true;
        this->cv_hashWrite_permit.notify_all();

        // wait for hashWrite finished signal
        std::mutex mtx_hashWrite_finish;
        std::unique_lock<std::mutex> lck_hashWrite_finsh(mtx_hashWrite_finish);
        this->cv_hashWrite_finish.wait(lck_hashWrite_finsh, [this]{return this->checkStatus(this->hashWrite_status_vector);});

        if(this->scheduleID == this->schedule.size() - 1)
        {
            this->hash_exit = true;
            break;
        }
        this->scheduleID++;
    }
}

void HashGen::launch()
{
    this->init();

    // hash create thread
    for(int i=0; i<this->num_threads_hash; i++)
    {
        std::thread hashGen_th(&HashGen::hashGenThread, this, i);
        this->thread_pool_vector.push_back(std::move(hashGen_th));
    }

    // hash sort thread
    for(int i=0; i<this->num_threads_sort; i++)
    {
        std::thread hashSort_th(&HashGen::hashSortThread, this, i);
        this->thread_pool_vector.push_back(std::move(hashSort_th));
    }

    // hash write thread
    for(int i=0; i<this->num_threads_write; i++)
    {
        std::thread hashWrite_th(&HashGen::hashWriteThread, this, i);
        this->thread_pool_vector.push_back(std::move(hashWrite_th));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // hash monitor
    std::thread monitor_th(&HashGen::hashMonitor, this);
    this->thread_pool_vector.push_back(std::move(monitor_th));

    // hash print
    std::thread print_th(&HashGen::hashPrint, this);
    this->thread_pool_vector.push_back(std::move(print_th));

    for (auto &t:this->thread_pool_vector)
    {
        if(t.joinable())
            t.join();
    }
}

void HashGen::hashPrint()
{
    int total_files = std::ceil(this->filesize / this->memory_size) * this->num_threads_sort;
    float complete;
    float ETA;
    float avg_hashgen;
    int fileID_last_second = 0;
    float MB = (float)this->memory_size / this->num_threads_sort;
    while(this->debug_model)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));       // sleep 1 second
        if(this->hash_exit)
            break;

        // calculate
        complete = (float)this->fileID / total_files * 100.0;
        avg_hashgen = (float)this->fileID / this->print_index;
        ETA = (float)(total_files - this->fileID) / avg_hashgen;

        //print
        printf("[%u][HASHING]: %.2f%% completed, ", this->print_index, complete);
        printf("ETA %.1f seconds, ", ETA);
        printf("%d/%d flushes, ", this->fileID, total_files);
        printf("%.1f MB/sec", (float)(this->fileID - fileID_last_second) * MB);
        printf("\n");

        fileID_last_second = this->fileID;
        this->print_index++;
    }
}

int HashGen::get_total_sorted_files()
{
    return this->fileID;
}

int HashGen::get_print_index()
{
    return this->print_index;
}

