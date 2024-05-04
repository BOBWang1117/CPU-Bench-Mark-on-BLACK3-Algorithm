#include <iostream>
#include <chrono>

#include "my_command.hpp"
#include "my_hashgen.hpp"
#include "my_external_sort.hpp"

int main(int argc, char* argv[])
{
    auto started = std::chrono::high_resolution_clock::now();
    MyCommand myCommand(argc, argv);
    myCommand.command();
    CommandLine commandLine = myCommand.get_commandLine();
    int num_threads_hash = commandLine.num_threads_hash;        // -t <num_threads_hash>:
    int num_threads_sort = commandLine.num_threads_sort;        // -o <num_threads_sort>
    int num_threads_write = commandLine.num_threads_write;      // -i <num_threads_write>
    const char* filename = commandLine.filename;                // -f <filename>
    int memory_size = commandLine.memory_size;                  // -m <memory_size>
    int filesize = commandLine.filesize;                        // -s <filesize>
    bool debug_model = commandLine.debug_model;                 // -d <debug_model>

    // Generate the hashcode and store them into "filename" file.
    HashGen hashGen(num_threads_hash, num_threads_sort, num_threads_write, filename, memory_size, filesize, debug_model);
    hashGen.launch();

    int total_sorted_files = hashGen.get_total_sorted_files();
    int print_index = hashGen.get_print_index();
    MyExternalSort myExternalSort(num_threads_sort, filename, filesize, total_sorted_files, debug_model, print_index);
    myExternalSort.launch();

    
    auto done = std::chrono::high_resolution_clock::now();
    auto total_time_cost_in_second = (float)std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count()/1000;

    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count()<<std::endl;
    if(debug_model)
        printf("Completed %d MB file %s in %.2f seconds\n", filesize, filename, total_time_cost_in_second);
    else
        printf("hashgen t%d o%d i%d m%d s%d %.2f\n", num_threads_hash, num_threads_sort, num_threads_write, memory_size, filesize, (float)total_time_cost_in_second);
    return 0;
}