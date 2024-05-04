#ifndef MY_HASHGEN_HPP
#define MY_HASHGEN_HPP

#include <cstring>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <algorithm>
#include <queue>

#include "my_command.hpp"
#include "hash_interface.hpp"

class HashGen
{
private:
    // command line
    int num_threads_hash;
    int num_threads_sort;
    int num_threads_write;
    char filename[FILE_NAME_MAX_LENGTH];
    int memory_size;
    int filesize;
    bool debug_model;

    // initialize
    std::vector<uint32_t> schedule;
    std::vector<Record>memory_buffer;
    std::vector<bool>hashGen_status_vector;
    std::vector<bool>hashSort_status_vector;
    std::vector<bool>hashWrite_status_vector;
    std::vector<bool>hashGen_permit_vector;
    std::vector<bool>hashSort_permit_vector;
    std::vector<bool>hashWrite_permit_vector;

    void init();
    void initThreadPool();
    void initScheduleList();
    void initMemoryBuffer();

    // threads
    int fileID = 0;
    int print_index = 1;
    uint32_t scheduleID = 0;
    uint32_t scheduled_size;
    bool hash_exit = false;
    std::vector<std::thread> thread_pool_vector;
    std::condition_variable cv_hashGen_permit;
    std::condition_variable cv_hashGen_finish;
    std::condition_variable cv_hashSort_permit;
    std::condition_variable cv_hashSort_finish;
    std::condition_variable cv_hashWrite_permit;
    std::condition_variable cv_hashWrite_finish;
    std::mutex mtx_sort;
    std::mutex mtx_write;
    std::mutex mtx_file;
    std::queue<std::pair<uint32_t, uint32_t>> sorted_range_queue;

    void hashGen(int hashGenThreadID, uint32_t begin, uint32_t end);
    void hashGenThread(int hashGenThreadID);
    void hashSort(int hashGenThreadID, uint32_t begin, uint32_t end);
    void hashSortThread(int hashGenThreadID);
    void hashWrite(int hashWriteThreadID, uint32_t begin, uint32_t end, int fileID);
    void hashWriteThread(int hashWriteThreadID);
    bool checkStatus(std::vector<bool>& v);
    void hashMonitor();
    void hashPrint();



public:
    HashGen(const int num_threads_hash, const int num_threads_sort, const int num_threads_write, const char* filename, const int memory_size, const int filesize, bool debug_model);
    ~HashGen();

    void launch();
    int get_total_sorted_files();
    int get_print_index();

};


#endif