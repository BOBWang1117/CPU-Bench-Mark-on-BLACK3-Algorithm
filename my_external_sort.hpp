#ifndef MY_EXTERNAL_SORT_HPP
#define MY_EXTERNAL_SORT_HPP

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <fstream>

#include "my_command.hpp"
#include "hash_interface.hpp"

#define FILE_HEAP_THRESHOLD 32
#define PII std::pair<Record, uint32_t>

class MyExternalSort
{
private:
    int num_threads_sort;
    char filename[FILE_NAME_MAX_LENGTH]; // -f <filename>: Specify the filename
    int filesize;
    int total_sorted_file;
    bool debug_model;
    int print_index;

    // initial
    int flushes;
    std::queue<std::string> filename_queue;
    void init();

    // thread
    int fileID = 0;
    std::mutex mtx;
    std::vector<std::thread> thread_pool_vector;
    bool my_cmp(const Record &a, const Record &b);
    std::streampos getFileSize(std::string filename);
    void externalSort(int threadID, std::string filename1, std::string filename2, int fileID);
    void externalSortThread(int threadID);
    void externalSortPrint();
    void externalHeapPrint();

    // external heap
    uint32_t total_records;
    uint32_t records_pos = 0;
    bool external_sort_exit = false;
    bool external_heap_exit = false;
    void externalHeap();

public:
    MyExternalSort(const int num_threads_sort, const char* filename, const int filesize, int total_sorted_file, bool debug_model, int print_index);
    ~MyExternalSort();

    void launch();
};

#endif