#include "my_external_sort.hpp"

MyExternalSort::MyExternalSort(const int num_threads_sort, const char* filename, const int filesize, int total_sorted_file, bool debug_model, int print_index)
{
    this->num_threads_sort = num_threads_sort;
    std::strcpy(this->filename, filename);
    this->filesize = filesize;
    this->total_sorted_file = total_sorted_file;
    this->debug_model = debug_model;
    this->print_index = print_index;
}

MyExternalSort::~MyExternalSort()
{
}



void MyExternalSort::init()
{
    if(this->total_sorted_file % 2 == 0)
        this->flushes = this->total_sorted_file - FILE_HEAP_THRESHOLD;
    else
        this->flushes = this->total_sorted_file - FILE_HEAP_THRESHOLD + 1;
    if(this->debug_model)
        printf("sort initial started, expecting %d flushes for %d buckets...\n", this->flushes, this->total_sorted_file);
    for(int i=0; i<this->total_sorted_file; i++)
    {
        std::string filename = "sorted_data_part_" + std::to_string(i+1) + ".bin";
        filename_queue.push(filename);
    }
}

bool MyExternalSort::my_cmp(const Record &a, const Record &b)
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

std::streampos MyExternalSort::getFileSize(std::string filename)
{
    std::streampos fsize = 0;
    std::ifstream file(filename, std::ios::binary);

    fsize = file.tellg();
    file.seekg(0, std::ios::end);
    fsize = file.tellg();
    file.seekg(0, std::ios::beg);
    file.close();

    return fsize;
}

void MyExternalSort::externalSort(int threadID, std::string filename1, std::string filename2, int fileID)
{
    std::string mini_file = "external_data_part_" + std::to_string(fileID+1) +".bin";
    std::ifstream file1(filename1, std::ios::binary);
    std::ifstream file2(filename2, std::ios::binary);
    std::ofstream file3(mini_file, std::ios::binary | std::ios::trunc);

    Record record_1 = {0};
    Record record_2 = {0};
    int struct_size = sizeof(Record);
    file1.read((char*)&record_1, struct_size);
    file2.read((char*)&record_2, struct_size);

    while(true)
    {
        if(!file1.eof() && !file2.eof())
        {
            if(this->my_cmp(record_1, record_2))
            {
                file3.write(reinterpret_cast<const char*>(&record_1), struct_size);
                file1.read((char*)&record_1, struct_size);
            }
            else
            {
                file3.write(reinterpret_cast<const char*>(&record_2), struct_size);
                file2.read((char*)&record_2, struct_size);
            }
        }
        else if(!file1.eof())
        {
            file3.write(reinterpret_cast<const char*>(&record_1), struct_size);
            file1.read((char*)&record_1, struct_size);
        }
        else if(!file2.eof())
        {
            file3.write(reinterpret_cast<const char*>(&record_2), struct_size);
            file2.read((char*)&record_2, struct_size);
        }
        else
        {
            break;
        }
    }

    file1.close();
    file2.close();
    file3.close();

    // remove file1 and file2
    std::remove(filename1.c_str());
    std::remove(filename2.c_str());

    std::unique_lock<std::mutex> lck(mtx);
    this->filename_queue.push(mini_file);
    lck.unlock();
}

void MyExternalSort::externalSortThread(int threadID)
{
    while(true)
    {
        std::unique_lock<std::mutex> lck(this->mtx);
        if(this->filename_queue.size() <= FILE_HEAP_THRESHOLD)      // FILE_HEAP_THRESHOLD = 32
        {
            lck.unlock();
            this->external_sort_exit = true;
            break;
        }
        std::string filename1 = this->filename_queue.front();
        this->filename_queue.pop();
        std::string filename2 = this->filename_queue.front();
        this->filename_queue.pop();
        int fileID = this->fileID;
        this->fileID++;
        lck.unlock();
        this->externalSort(threadID, filename1, filename2, fileID);
        // break;
    }
}

struct priorityQueueCompare {
    bool operator()(PII const& p1, PII const& p2)
    {
        if(p1.first.hash[0] > p2.first.hash[0])
            return true;
        else if(p1.first.hash[0] < p2.first.hash[0])
            return false;
        else if(p1.first.hash[1] > p2.first.hash[1])
            return true;
        else if(p1.first.hash[1] < p2.first.hash[1])
            return false;
        else if(p1.first.hash[2] > p2.first.hash[2])
            return true;
        else if(p1.first.hash[2] < p2.first.hash[2])
            return false;
        else if(p1.first.hash[3] > p2.first.hash[3])
            return true;
        else if(p1.first.hash[3] < p2.first.hash[3])
            return false;
        else if(p1.first.hash[4] > p2.first.hash[4])
            return true;
        else if(p1.first.hash[4] < p2.first.hash[4])
            return false;
        else if(p1.first.hash[5] > p2.first.hash[5])
            return true;
        else if(p1.first.hash[5] < p2.first.hash[5])
            return false;
        else if(p1.first.hash[6] > p2.first.hash[6])
            return true;
        else if(p1.first.hash[6] < p2.first.hash[6])
            return false;
        else if(p1.first.hash[7] > p2.first.hash[7])
            return true;
        else if(p1.first.hash[7] < p2.first.hash[7])
            return false;
        else if(p1.first.hash[8] > p2.first.hash[8])
            return true;
        else if(p1.first.hash[8] < p2.first.hash[8])
            return false;
        else if(p1.first.hash[9] > p2.first.hash[9])
            return true;
        else if(p1.first.hash[9] < p2.first.hash[9])
            return false;
        else
            return false;
    }
};

void MyExternalSort::externalHeap()
{
    std::vector <std::ifstream *> myFiles;
    std::vector<std::string> del_filename_vector;
    while(!this->filename_queue.empty())
    {
        std::string filename = this->filename_queue.front();
        del_filename_vector.push_back(filename);
        this->filename_queue.pop();
        std::ifstream* file = new std::ifstream(filename, std::ios::in | std::ios::binary);
        myFiles.push_back(file);
    }
    int struct_size = sizeof(Record);
    std::ofstream outFile(this->filename, std::ios::binary | std::ios::trunc);
    std::priority_queue<PII, std::vector<PII>, priorityQueueCompare> ds;

    for(uint32_t i=0; i<myFiles.size(); i++)
    {
        Record record;
        if(!(*myFiles[i]).eof())
            (*myFiles[i]).read((char*) &record, struct_size);
        ds.push({record,i});
    }

    int MB = 1024*1024/sizeof(Record);
    for(int i=0; i<this->filesize; i++)
    {
        for(int j=0; j<MB; j++)
        {
            PII p = ds.top();
            outFile.write(reinterpret_cast<const char*>(&p.first), struct_size);
            ds.pop();
            if(!(*myFiles[p.second]).eof())
            {
                Record record;
                (*myFiles[p.second]).read((char*) &record, struct_size);
                ds.push({record, p.second});
            }
        }
        this->records_pos++;
    }

    outFile.close();

    for(uint32_t i=0; i<myFiles.size(); i++)
    {
        (*myFiles[i]).close();
        delete myFiles[i];
        // std::string filename = "part_" + std::to_string(i+1) + ".bin";
        std::remove(del_filename_vector[i].c_str());
    }
    this->external_heap_exit = true;

}

void MyExternalSort::launch()
{
    this->init();
    // hash sort thread
    for(int i=0; i<this->num_threads_sort; i++)
    {
        std::thread externalSort_th(&MyExternalSort::externalSortThread, this, i);
        this->thread_pool_vector.push_back(std::move(externalSort_th));
    }

    // // exeternal sort print
    if(this->total_sorted_file > FILE_HEAP_THRESHOLD)
    {
        std::thread externalSort_print_th(&MyExternalSort::externalSortPrint, this);
        this->thread_pool_vector.push_back(std::move(externalSort_print_th));
    }

    for (auto &t:this->thread_pool_vector)
    {
        if(t.joinable())
            t.join();
    }

    std::thread externalHeap_print_th(&MyExternalSort::externalHeapPrint, this);
    this->externalHeap();

    if(externalHeap_print_th.joinable())
        externalHeap_print_th.join();
}

void MyExternalSort::externalSortPrint()
{
    int total_files = this->flushes;
    float complete;
    float ETA;
    float avg_hashgen;
    int print_index_last_second = this->print_index;
    // float MB = (float)this->memory_size / this->num_threads_sort;
    while(this->debug_model)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));       // sleep 1 second
        if(this->external_sort_exit)
            break;

        // calculate
        complete = (float)this->fileID / total_files * 100.0;
        avg_hashgen = (float)this->fileID / (this->print_index-print_index_last_second);
        ETA = (float)(total_files - this->fileID) / avg_hashgen;

        //print
        printf("[%u][SORT(INIT)]: %.2f%% completed, ", this->print_index, complete);
        printf("ETA %.1f seconds, ", ETA);
        printf("%d/%d flushes", this->fileID, total_files);
        printf("\n");

        this->print_index++;
    }
}

void MyExternalSort::externalHeapPrint()
{
    int total_files = this->filesize;
    float complete;
    float ETA;
    float avg_hashgen;
    int fileID_last_second = 0;
    int print_index_last_second = this->print_index;
    while(this->debug_model)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));       // sleep 1 second
        if(this->external_heap_exit)
            break;

        // calculate
        complete = (float)this->records_pos / total_files * 100.0;
        avg_hashgen = (float)this->records_pos / (this->print_index-print_index_last_second);
        ETA = (float)(total_files - this->records_pos) / avg_hashgen;

        //print
        printf("[%u][MERGE]: %.2f%% completed, ", this->print_index, complete);
        printf("ETA %.1f seconds, ", ETA);
        printf("%d/%d flushes, ", this->records_pos, total_files);
        printf("%.1f MB/sec", (float)(this->records_pos - fileID_last_second));
        printf("\n");

        fileID_last_second = this->records_pos;
        this->print_index++;
    }
}

