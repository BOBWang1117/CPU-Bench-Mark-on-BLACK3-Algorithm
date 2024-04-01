#include <iostream>

#include "my_command.hpp"
#include "my_hash_generation.hpp"
#include "my_hash_sort.hpp"
/***
 * ./main -h
 * Help:
 *      -t <num_threads_hash>: Specify the number of threads in hashing operation   [Default: 1]
 *      -o <num_threads_sort>: Specify the number of threads in sorting operation   [Default: 1]
 *      -i <num_threads_write>: Specify the number of threads in writing operation  [Default: 1]
 *      -f <filename>: Specify the filename
 *      -m <memory_size>: Specify the maximum memory allowed to use (MB)            [Default: 128]
 *      -s <filesize>: Specify the hash file size (MB)                              [Default: 1024]
 *      -d <debug_model>: turns on debug mode with true, off with false             [Default: True]
 *      -h <display>: Display this help message
***/

int main(int argc, char* argv[])
{
    auto started = std::chrono::high_resolution_clock::now();

    MyCommand myCommand(argc, argv);
    myCommand.command();
    CommandLine commandLine = myCommand.get_commandLine();

    int num_threads_hash = commandLine.num_threads_hash;
    int num_threads_sort = commandLine.num_threads_sort;
    int num_threads_write = commandLine.num_threads_write;
    const char* filename = commandLine.filename;
    int memory_size = commandLine.memory_size;
    int filesize = commandLine.filesize;
    bool debug_model = commandLine.debug_model;

    int index = 1;
    // generate the hash file
    HashGen hashGen(num_threads_hash, num_threads_write, filename, memory_size, filesize, debug_model, &index);
    hashGen.launch();
    
    HashSort hashSort(num_threads_sort, filename, memory_size, filesize, debug_model, &index);
    hashSort.launch();

    auto done = std::chrono::high_resolution_clock::now();

    if(!debug_model)
    {
        printf("hashgen t%d o%d i%d m%d s%d ", num_threads_hash, num_threads_sort, num_threads_write, memory_size, filesize);
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count()<<std::endl;
    }
    else
    {
        std::cout <<"Completed "<<filesize<<" MB file "<<filename<<" in "<< std::chrono::duration_cast<std::chrono::seconds>(done-started).count()<<" seconds"<<std::endl;
    }

    

    return 0;
}