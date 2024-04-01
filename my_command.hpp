#ifndef MY_COMMAND_HPP
#define MY_COMMAND_HPP

#include <iostream>
#include <cstring>

#define FILE_NAME_MAX_LENGTH 64

struct CommandLine
{
    int num_threads_hash = 1;       // -t <num_threads_hash>: Specify the number of threads in hashing operation   [Default: 1]
    int num_threads_sort = 1;       // -o <num_threads_sort>: Specify the number of threads in sorting operation   [Default: 1]
    int num_threads_write = 1;      // -i <num_threads_write>: Specify the number of threads in writing operation  [Default: 1]
    char filename[FILE_NAME_MAX_LENGTH]; // -f <filename>: Specify the filename
    int memory_size = 128;          // -m <memory_size>: Specify the maximum memory allowed to use (MB)            [Default: 128]
    int filesize = 1024;            // -s <filesize>: Specify the hash file size (MB)                              [Default: 1024]
    bool debug_model = true;        // -d <debug_model>: turns on debug mode with true, off with false             [Default: True]
    
    int record_size = 16;
    int hash_size = 10;
    int nonce_size = 6;
};


class MyCommand{
public:
    MyCommand(int argc, char* argv[]);
    ~MyCommand();
    void command();
    CommandLine get_commandLine();

private:
    int argc;
    char** argv;
    CommandLine commandLine;

    void printCommand();
    bool isDigit(int index);
    void usage();
    void _t(int index);
    void _o(int index);
    void _i(int index);
    void _f(int index);
    void _m(int index);
    void _s(int index);
    void _d(int index);
    void _h();
};


#endif