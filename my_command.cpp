#include "my_command.hpp"

using namespace std;

MyCommand::MyCommand(int argc, char* argv[])
{
    this->argc = argc;
    this->argv = argv;
}

MyCommand::~MyCommand()
{
    // free(this->commandLine.filename);
}
void MyCommand::command()
{
    if(this->argc < 2)
    {
        this->usage();
        exit(-1);
    }
    // this->printCommand();

    int index = 1;
    while(index < this->argc)
    {
        if(argv[index][0] != '-' || std::strlen(argv[index]) != 2)
        {
            std::cout<<"FATAL: Cannot find benchmark '"<<argv[index]<<"': no such built-in test, file or module"<<std::endl;
            exit(-2);
        }
        char parameter = argv[index][1];
        switch (parameter)
        {
        case 't':
            this->_t(index);
            index++;
            break;
        case 'o':
            this->_o(index);
            index++;
            break;
        case 'i':
            this->_i(index);
            index++;
            break;
        case 'f':
            this->_f(index);
            index++;
            break;
        case 'm':
            this->_m(index);
            index++;
            break;
        case 's':
            this->_s(index);
            index++;
            break;
        case 'd':
            this->_d(index);
            index++;
            break;
        case 'h':
            this->_h();
            exit(0);
        
        default:
            std::cout<<"FATAL: Cannot find benchmark '"<<argv[index]<<"': no such built-in test, file or module"<<std::endl;
            exit(-3);
        }
        index++;
    }
    if(this->commandLine.debug_model)
        this->printCommand();

}

void MyCommand::usage()
{
    std::cout<<"Usage: "<<std::endl;
    std::cout<<"   ./hashgen -h"<<std::endl;
    std::cout<<"   ./hashgen -t 16 -o 1 -i 1 -f data.bin -m 16 -s 1024 -d true\n"<<std::endl;
}

bool MyCommand::isDigit(int index)
{
    int len = std::strlen(this->argv[index+1]);
    for(int i=0; i<len; i++)
    {
        if(!std::isdigit(this->argv[index+1][i]))
            return false;    // The string contain char
    }
    return true;
}

void MyCommand::_t(int index)
// -t <num_threads_hash>: Specify the number of threads in hashing operation   [Default: 1]
{
    if(!isDigit(index))
    {
        std::cout<<"Invalid Parameter [-t]"<<std::endl;
        std::cout<<"   The parameter shoule be an integer, but ["<<this->argv[index+1]<<"] contains charactor"<<std::endl;
        exit(-4);
    }
    int num_threads_hash = std::atoi(this->argv[index+1]);
    this->commandLine.num_threads_hash=num_threads_hash;
}

void MyCommand::_o(int index)
// -o <num_threads_sort>: Specify the number of threads in sorting operation   [Default: 1]
{
    if(!isDigit(index))
    {
        std::cout<<"Invalid Parameter [-t]"<<std::endl;
        std::cout<<"   The parameter shoule be an integer, but ["<<this->argv[index+1]<<"] contains charactor"<<std::endl;
        exit(-5);
    }
    int num_threads_sort = std::atoi(this->argv[index+1]);
    this->commandLine.num_threads_sort=num_threads_sort;
}

void MyCommand::_i(int index)
// -i <num_threads_write>: Specify the number of threads in writing operation  [Default: 1]
{
    if(!isDigit(index))
    {
        std::cout<<"Invalid Parameter [-t]"<<std::endl;
        std::cout<<"   The parameter shoule be an integer, but ["<<this->argv[index+1]<<"] contains charactor"<<std::endl;
        exit(-6);
    }
    int num_threads_write = std::atoi(this->argv[index+1]);
    this->commandLine.num_threads_write=num_threads_write;
}

void MyCommand::_f(int index)
// -f <filename>: Specify the filename
{
    int len = std::strlen(this->argv[index+1]);
    if(len > FILE_NAME_MAX_LENGTH)
    {
        std::cout<<"Invalid Parameter [-f]:"<<std::endl;
        std::cout<<"   The length of file name must shorter than 64 bytes"<<std::endl;
    }
    std::strcpy(this->commandLine.filename, this->argv[index+1]);
}

void MyCommand::_m(int index)
// -m <memory_size>: Specify the maximum memory allowed to use (MB)            [Default: 128]
{
    if(!isDigit(index))
    {
        std::cout<<"Invalid Parameter [-t]"<<std::endl;
        std::cout<<"   The parameter shoule be an integer, but ["<<this->argv[index+1]<<"] contains charactor"<<std::endl;
        exit(-7);
    }
    int memory_size = std::atoi(this->argv[index+1]);
    this->commandLine.memory_size=memory_size;
}

void MyCommand::_s(int index)
// -s <filesize>: Specify the hash file size (MB)                              [Default: 1024]
{
    if(!isDigit(index))
    {
        std::cout<<"Invalid Parameter [-t]"<<std::endl;
        std::cout<<"   The parameter shoule be an integer, but ["<<this->argv[index+1]<<"] contains charactor"<<std::endl;
        exit(-9);
    }
    int filesize = std::atoi(this->argv[index+1]);
    this->commandLine.filesize=filesize;
}

void MyCommand::_d(int index)
// -d <debug_model>: turns on debug mode with true, off with false             [Default: True]
{
    int len = std::strlen(this->argv[index+1]);
    char temp[len+1];
    std::strcpy(temp, this->argv[index+1]);
    if(strcmp(temp, "true")==0)
    {
        this->commandLine.debug_model = true;
    }
    else if(strcmp(temp, "false")==0)
    {
        this->commandLine.debug_model = false;
    }
    else
    {
        std::cout<<"Invalid Parameter [-d]:"<<std::endl;
        std::cout<<"   The parameter can only be [true] or [false]"<<std::endl;
        exit(-10);
    }
}

void MyCommand::_h()
{
    std::cout<<"Help:"<<std::endl;
    std::cout<<"  -t <num_threads_hash>: Specify the number of threads in hashing operation   [Default: 1]"     <<std::endl;
    std::cout<<"  -o <num_threads_sort>: Specify the number of threads in sorting operation   [Default: 1]"     <<std::endl;
    std::cout<<"  -i <num_threads_write>: Specify the number of threads in writing operation  [Default: 1]"     <<std::endl;
    std::cout<<"  -f <filename>: Specify the filename"                                                          <<std::endl;
    std::cout<<"  -m <memory_size>: Specify the maximum memory allowed to use (MB)            [Default: 128]"   <<std::endl;
    std::cout<<"  -s <filesize>: Specify the hash file size (MB)                              [Default: 1024]"  <<std::endl;
    std::cout<<"  -d <debug model>: turns on debug mode with true, off with false             [Default: True]"  <<std::endl;
    std::cout<<"  -h <display>: Display this help message"                                                      <<std::endl;
}

void MyCommand::printCommand()
{
    std::cout<<"NUM_THREADS_HASH: "<<this->commandLine.num_threads_hash<<std::endl;
    std::cout<<"NUM_THREADS_SORT: "<<this->commandLine.num_threads_sort<<std::endl;
    std::cout<<"NUM_THREADS_WRITE: "<<this->commandLine.num_threads_write<<std::endl;
    std::cout<<"FILENAME: "<<this->commandLine.filename<<std::endl;
    std::cout<<"MEMORY_SIZE: "<<this->commandLine.memory_size<<std::endl;
    std::cout<<"FILESIZE: "<<this->commandLine.filesize<<std::endl;
    // std::cout<<"DEBUG_MODEL: "<<this->commandLine.debug_model<<std::endl;

    // "Record": structure to hold a 16-byte record
    std::cout<<"RECORD_SIZE: "<<this->commandLine.record_size<<std::endl;
    std::cout<<"HASH_SIZE: "<<this->commandLine.hash_size<<std::endl;
    std::cout<<"NONCE_SIZE: "<<this->commandLine.nonce_size<<std::endl;
}

CommandLine MyCommand::get_commandLine()
{
    return this->commandLine;
}