#include "my_hash_sort.hpp"



HashSort::HashSort(int num_threads_sort, const char* filename, int memory_size, int filesize, bool debug_model, int* index)
{
    this->num_threads_sort = num_threads_sort;
    std::strcpy(this->filename, filename);
    this->memory_size = memory_size;
    this->filesize = filesize;
    this->debug_model = debug_model;
    this->index = index;
}

HashSort::~HashSort()
{
}

void HashSort::splitFile(const std::string & inputFile, const std::string & outputPrefix, int chunkSize)
{
    this->file_splite = true;
    std::ifstream input(inputFile, std::ios::binary);
    uint32_t filesize = this->filesize * 1024 *1024;
    int numChunks = (filesize + chunkSize - 1) / chunkSize;
    this->total_files = numChunks;
    int struct_size = sizeof(Record);
    int index = 0;
    int counter = 1;
    std::vector <std::ofstream *> myFiles;
    uint32_t line = filesize/16;
    for(int i=0; i<numChunks; i++)
    {
        std::string filename = "part_" + std::to_string(i+1) + ".bin";
        this->q_tempFile.push(filename);
        std::ofstream* file = new std::ofstream(filename, std::ios::out | std::ios::binary);
        myFiles.push_back(file);
    }
    int chunkLine = chunkSize /16;
    for(uint32_t i=0; i<line; i++)
    {
        Record record;
        input.read((char*) &record, struct_size);
        (*myFiles[index]).write(reinterpret_cast<const char*>(&record), struct_size);
        if(counter == chunkLine)
        {
            (*myFiles[index]).close();
            index++;
            counter = 0;
        }
        counter++;
    }
    input.close();
    for(int i=0; i<numChunks; i++)
    {
        delete myFiles[i];
    }
}

int compare(const void *a, const void *b)
{
    Record *RecordA = (Record *)a;
    Record *RecordB = (Record *)b;
    if(RecordA->hash[0] > RecordB->hash[0])
        return 1;
    else if(RecordA->hash[0] < RecordB->hash[0])
        return -1;
    else if(RecordA->hash[1] > RecordB->hash[1])
        return 1;
    else if(RecordA->hash[1] < RecordB->hash[1])
        return -1;
    else if(RecordA->hash[2] > RecordB->hash[2])
        return 1;
    else if(RecordA->hash[2] < RecordB->hash[2])
        return -1;
    else if(RecordA->hash[3] > RecordB->hash[3])
        return 1;
    else if(RecordA->hash[3] < RecordB->hash[3])
        return -1;
    else if(RecordA->hash[4] > RecordB->hash[4])
        return 1;
    else if(RecordA->hash[4] < RecordB->hash[4])
        return -1;
    else if(RecordA->hash[5] > RecordB->hash[5])
        return 1;
    else if(RecordA->hash[5] < RecordB->hash[5])
        return -1;
    else if(RecordA->hash[6] > RecordB->hash[6])
        return 1;
    else if(RecordA->hash[6] < RecordB->hash[6])
        return -1;
    else if(RecordA->hash[7] > RecordB->hash[7])
        return 1;
    else if(RecordA->hash[7] < RecordB->hash[7])
        return -1;
    else if(RecordA->hash[8] > RecordB->hash[8])
        return 1;
    else if(RecordA->hash[8] < RecordB->hash[8])
        return -1;
    else if(RecordA->hash[9] > RecordB->hash[9])
        return 1;
    else if(RecordA->hash[9] < RecordB->hash[9])
        return -1;
    else
        return 0;
}

void HashSort::sort_start(std::string tempFilename)
{
    std::ifstream inFile(tempFilename, std::ios::binary);
    inFile.seekg(0, std::ios::end); // Move the file pointer to the end of the file
    std::streampos fileLength = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    int struct_size = sizeof(Record);
    int array_size = fileLength/struct_size;
    Record* record = new Record[array_size];
    // std::cout<<fileLength<<std::endl;
    for(int i=0; i<array_size; i++)
    {
        inFile.read((char*) &record[i], struct_size);
    }
    inFile.close();

    std::qsort(record, array_size, struct_size, compare);

    //write back
    std::ofstream outFile(tempFilename, std::ios::binary);
    for(int i=0; i<array_size; i++)
    {
        outFile.write(reinterpret_cast<const char*>(&record[i]), struct_size);
    }
    outFile.close();
    delete[] record;
    mtx.lock();
    (this->sorted_files)++;
    mtx.unlock();
}

void HashSort::sort_thread(int threadID)
{
    // std::cout<<this->q_tempFile.size()<<std::endl;
    while(!this->q_tempFile.empty())
    {
        mtx.lock();
        std::string tempFilename = this->q_tempFile.front();
        this->q_tempFile.pop();
        mtx.unlock();
        this->sort_start(tempFilename);
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

void HashSort::mergeFiles()
{
    this->file_merging_start = true;
    std::vector <std::ifstream *> myFiles;
    for(int i=0; i<this->total_files; i++)
    {
        std::string filename = "part_" + std::to_string(i+1) + ".bin";
        std::ifstream* file = new std::ifstream(filename, std::ios::in | std::ios::binary);
        myFiles.push_back(file);
    }
    int struct_size = sizeof(Record);
    std::ofstream outFile(this->filename, std::ios::binary);
    std::priority_queue<PII, std::vector<PII>, priorityQueueCompare> ds;
    // Record* record = new Record[this->total_files];
    for(int i=0; i<this->total_files; i++)
    {
        Record record;
        if(!(*myFiles[i]).eof())
            (*myFiles[i]).read((char*) &record, struct_size);
        ds.push({record,i});
    }

    int items = this->filesize * 1024 * 1024 / 16;
    for(int i=0; i<items; i++)
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

    outFile.close();

    for(int i=0; i<this->total_files; i++)
    {
        (*myFiles[i]).close();
        delete myFiles[i];
        std::string filename = "part_" + std::to_string(i+1) + ".bin";
        std::remove(filename.c_str());
    }
    this->print_exit = true;
}

void HashSort::launch()
{
    std::thread print_th(&HashSort::print_thread, this);
    //split the file first
    this->chunkSize = this->memory_size * 1024 * 1024 / this->num_threads_sort;
    while(true)     // make sure chunkSize can be divide by 16 [sizeof(Record)]
    {
        if(this->chunkSize % 16 ==0)
        {
            break;
        }
        this->chunkSize--;
    }
    this->splitFile(this->filename, "part_", this->chunkSize);
    // std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    this->file_splite = false;
    for(int i=0; i<this->num_threads_sort; i++)
    {
        std::thread sort_th(&HashSort::sort_thread, this, i);
        this->threadVector.push_back(std::move(sort_th));
    }

    // std::thread print_th(&HashSort::print_thread, this);
    // this->threadVector.push_back(std::move(print_th));

    for (auto &t:this->threadVector)
    {
        t.join();
    }
    this->mergeFiles();
    print_th.join();
}

void HashSort::print_thread()
{
    int time = 0;
    int complete_files_per_second = 0;
    float chunkSize_MB = this->chunkSize / 1024 / 1024;
    if(this->debug_model)
        std::cout<<"sort started, expecting "<<this->total_files<<" for sorting files"<<std::endl;

    while(this->debug_model)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));       // sleep 1 second
        if(this->print_exit == true)
        {
            break;
        }
        float completed = 100.0 * this->sorted_files / this->total_files;
        complete_files_per_second =  this->sorted_files - complete_files_per_second;
        printf("[%d][SORTING]: ", *index+time);
        if(completed < 1.0)
        {
            printf("Allocating Memory");
        }
        else if(this->file_merging_start)
        {
            printf("Files Merging");
        }
        else if(this->file_splite)
        {
            printf("Files Spliting");
        }
        else
        {
            printf("%.1f%% completed, ", completed);
            printf("ETA %.1f seconds, ", (float)(this->total_files-this->sorted_files)/(this->sorted_files/(time)));
            printf("%d/%d flushes, ", this->sorted_files, this->total_files);
            printf("%.1f MB/s",complete_files_per_second * chunkSize_MB);
        }
        printf("\n");
        time++;
        complete_files_per_second = this->sorted_files;
    }
    *index = *index + time;
}


