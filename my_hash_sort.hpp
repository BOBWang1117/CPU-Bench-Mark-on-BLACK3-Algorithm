#ifndef MY_HASH_SORT_HPP
#define MY_HASH_SORT_HPP

#include "my_hash_generation.hpp"
#include <algorithm>

#define PII std::pair<Record, int>
class HashSort
{
private:
    //command line
    int num_threads_sort;
    char filename[FILE_NAME_MAX_LENGTH];
    int memory_size;
    int filesize;        // If filesize is 1024 (1G), the total number of data should be 64*1024*1024 (2^26)
    bool debug_model;

    //split file
    std::queue<std::string> q_tempFile;
    int chunkSize;
    int total_files;
    int sorted_files = 0;

    //sort
    int* index;
    std::mutex mtx;
    std::vector<std::thread> threadVector;
    bool print_exit = false;
    bool file_merging_start = false;
    bool file_splite = false;
    void splitFile(const std::string & inputFile, const std::string & outputPrefix, int chunkSize);
    void print_thread();
    void sort_thread(int threadID);
    void sort_start(std::string tempFilename);
    void mergeFiles();
public:
    HashSort(int num_threads_sort, const char* filename, int memory_size, int filesize, bool debug_model, int* index);
    ~HashSort();
    void launch();
};


#endif