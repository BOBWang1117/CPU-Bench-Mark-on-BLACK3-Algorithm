#ifndef MY_HASH_GENERATION_HPP
#define MY_HASH_GENERATION_HPP

#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <random>
#include <queue>
#include <fstream>

#include "my_command.hpp"
#include "./blake3/blake3.h"

#define NONCE_SIZE 6
#define HASH_SIZE 10
#define DATA_BLOCK_UNIT  64 * 1024   // generate 1 MB Record data [64 = 1024/sizeof(Record)]

// Structure to hold a 16-byte record
struct Record
{
    uint8_t hash[HASH_SIZE];        // hash value as byte array
    uint8_t nonce[NONCE_SIZE];      // Nonce value as byte array
};

class HashGen
{
private:
    // command line
    int num_threads_hash;
    int num_threads_write;
    char filename[FILE_NAME_MAX_LENGTH];
    int memory_size;
    int filesize;        // If filesize is 1024 (1G), the total number of data should be 64*1024*1024 (2^26)
    bool debug_model;

    // thread parameter
    int* index;
    std::size_t repository_size;
    int item_total;
    std::mutex mtx;
    std::mutex producer_count_mtx;
    std::mutex consumer_count_mtx;
    std::condition_variable repo_not_full;
    std::condition_variable repo_not_empty;
    std::queue<Record*> item_buffer;
    int produced_item_counter = 0;
    int consumed_item_counter = 0;
    std::vector<std::thread> threadVector;
    bool print_exit = false;
    bool hashing_merge_files = false;
    void produce_item();
    void producer_thread();
    void consume_item(std::ofstream& outFile);
    void comsumer_thread(int threadID);
    void print_thread();
    void mergeFiles();

public:
    HashGen(int num_threads_hash, int num_threads_write, const char* filename, int memory_size, int filesize, bool debug_model, int* index);
    ~HashGen();

    void launch();
};



#endif