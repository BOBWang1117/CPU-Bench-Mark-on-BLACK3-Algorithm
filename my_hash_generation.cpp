#include "my_hash_generation.hpp"
/****
 * This class is the impletement method of hash generation 
 * 
 * 
****/

uint8_t RANDOM_TABLE[UINT8_MAX] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe,
};

HashGen::HashGen(int num_threads_hash, int num_threads_write, const char* filename, int memory_size, int filesize, bool debug_model, int* index)
{
    this->num_threads_hash = num_threads_hash;      // -t <num_threads_hash>
    this->num_threads_write = num_threads_write;    // -i <num_threads_write>
    std::strcpy(this->filename, filename);          // -f <filename>
    this->memory_size = memory_size;                // -m <memory_size>
    this->filesize = filesize;                      // -s <filesize>
    this->debug_model = debug_model;                // -d <debug_model>

    this->repository_size = memory_size;            // The max size of queue<Record> (MByte)
    this->item_total = filesize;
    std::ofstream outFile(filename, std::ios::binary);
    this->index = index;
    // for(int i=0; i<num_threads_write; i++)
    // {
    //     std::string filename = std::to_string(i) + ".bin";
    //     std::ofstream outFile(filename, std::ios::binary);
    // }
}

HashGen::~HashGen()
{
    // this->outFile.close();
}

int intRand(const int & min, const int & max) {
    static thread_local std::mt19937* generator = nullptr;
    if (!generator) generator = new std::mt19937(clock() + std::hash<std::thread::id>{}(std::this_thread::get_id()));
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(*generator);
}

void recordUnitBlock(Record* recordBlock)
{
    for(int i=0; i<DATA_BLOCK_UNIT; i++)
    {
        for(int j=0; j<NONCE_SIZE; j++)
        {
            int random = intRand(0, 255);
            recordBlock[i].nonce[j] = RANDOM_TABLE[random];
        }

        blake3_hasher hasher;
        blake3_hasher_init(&hasher);
        size_t n = NONCE_SIZE + 1;
        blake3_hasher_update(&hasher, recordBlock[i].nonce, n);
        uint8_t output[BLAKE3_OUT_LEN]={0};
        blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);

        for(int j=0; j<HASH_SIZE; j++)
        {
            recordBlock[i].hash[j] = output[j];
        }
    }
}

void HashGen::produce_item()
{
    std::unique_lock<std::mutex> lck(this->mtx);

    while(this->repository_size == this->item_buffer.size())
    {
        // std::cout << "Producer is waiting for an empty slot..." << std::endl;
        this->repo_not_full.wait(lck);    // wait for repository size > 0 condiction happening
    }

    //generate a 1MByte Record data
    Record* unitBlock = new Record[DATA_BLOCK_UNIT];
    recordUnitBlock(unitBlock);
    item_buffer.push(unitBlock);
    repo_not_empty.notify_all();
    lck.unlock();
}

void HashGen::producer_thread()
{
    bool ready_to_exit = false;
    while (true)
    {
        std::unique_lock<std::mutex> lock(this->producer_count_mtx);
        if (this->produced_item_counter < this->item_total)
        {
            ++this->produced_item_counter;
            this->produce_item();
        }
        else
        {
            ready_to_exit = true;
        }
 
        // lock.unlock();
 
        if (ready_to_exit == true)
        {
            break;
        }
    }

    // std::cout << "Producer thread " << std::this_thread::get_id()
    //     << " is exiting..." << std::endl;
}

void HashGen::consume_item(std::ofstream& outFile)
{
    std::unique_lock<std::mutex> lck(mtx);

    // item buffer is empty, just wait here.
    while (this->item_buffer.size() == 0)
    {
        // std::cout << "Consumer is waiting for items..." << std::endl;
        repo_not_empty.wait(lck);
    }

    Record* r = this->item_buffer.front();
    this->item_buffer.pop();
    lck.unlock();
    
    //write data into file
    int len = sizeof(Record);
    for(int i=0; i<DATA_BLOCK_UNIT; i++)
    {
        outFile.write(reinterpret_cast<const char*>(&(r[i])), len);
    }
    delete[] r;
    repo_not_full.notify_all();
}

void HashGen::comsumer_thread(int threadID)
{
    bool read_to_exit = false;
    std::string miniFilename = "hashing_"+std::to_string(threadID+1) + ".bin";
    std::ofstream outFile(miniFilename, std::ios::binary | std::ios::app);
    while (true)
    {
        std::unique_lock<std::mutex> lck(consumer_count_mtx);
        if (consumed_item_counter < item_total)
        {
            consume_item(outFile);
            ++consumed_item_counter;
        }
        else
        {
            read_to_exit = true;
        }
 
        if (read_to_exit == true)
        {
            break;
        }
    }
    outFile.close();
 
    // std::cout << "Consumer thread " << std::this_thread::get_id()
    //     << " is exiting..." << std::endl;
}

void HashGen::print_thread()
{
    // int index = 1;
    float generate_per_second = 0.0;
    float eta = 0.0;
    while(this->debug_model)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));       // sleep 1 second
        if (this->print_exit == true)
        {
            break;
        }
        float completed = (float)this->consumed_item_counter/(float)this->item_total * 100.0;
        generate_per_second = this->consumed_item_counter - generate_per_second;
        eta = (float)(this->item_total - this->consumed_item_counter) / ((this->consumed_item_counter)/(*index));
        printf("[%d][HASHING]: ", *this->index);
        if(completed<1.00)
        {
            printf("Allocting Memory");
        }
        else if(this->hashing_merge_files)
        {
            printf("File Merging");
        }
        else
        {
            printf("%.2f%% completed, ", completed);
            printf("ETA %.1f seconds, ", eta);
            printf("%d/%d flushed, ", this->consumed_item_counter, this->item_total);
            printf("%.1f MB/sec", generate_per_second);
        }
        printf("\n");
        (*index)++;
        generate_per_second = this->consumed_item_counter;
    }
}

void HashGen::launch()
{
    for(int i=0; i<this->num_threads_hash; i++)
    {
        std::thread producer_th(&HashGen::producer_thread, this);
        this->threadVector.push_back(std::move(producer_th));
    }

    for(int i=0; i<this->num_threads_write; i++)
    {
        std::thread producer_th(&HashGen::comsumer_thread, this, i);
        this->threadVector.push_back(std::move(producer_th));
    }

    std::thread print_th(&HashGen::print_thread, this);
    // this->threadVector.push_back(std::move(print_th));

    for (auto &t:this->threadVector)
    {
        t.join();
    }

    this->mergeFiles();
    print_th.join();
}

void HashGen::mergeFiles()
{
    this->hashing_merge_files = true;
    std::ofstream finalFile(this->filename, std::ios_base::binary);
    for(int i=0; i<this->num_threads_write; i++)
    {
        std::string miniFilename = "hashing_"+std::to_string(i+1) + ".bin";
        std::ifstream temp(miniFilename, std::ios_base::binary);
        finalFile << temp.rdbuf();
        temp.close();
        std::remove(miniFilename.c_str());
    }
    finalFile.close();
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    this->print_exit = true;
}