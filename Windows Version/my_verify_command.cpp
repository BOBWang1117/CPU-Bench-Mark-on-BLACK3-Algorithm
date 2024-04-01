#include "my_verify_command.hpp"

using namespace std;

VerifyCommand::VerifyCommand(int argc, char* argv[])
{
    this->argc = argc;
    this->argv = argv;
}

VerifyCommand::~VerifyCommand()
{
    // free(this->commandLine.filename);
}
void VerifyCommand::command()
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
        case 'f':
            this->_f(index);
            index++;
            break;
        case 'p':
            this->_p(index);
            index++;
            break;
        case 'r':
            this->_r(index);
            index++;
            break;
        case 'v':
            this->_v(index);
            index++;
            break;
        case 'd':
            this->_d(index);
            index++;
            break;
        case 'b':
            this->_b(index);
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

void VerifyCommand::usage()
{
    std::cout<<"Usage: "<<std::endl;
    std::cout<<"   ./hashgen -h"<<std::endl;
    std::cout<<"   ./hashverify -d true -f data.bin -v true"<<std::endl;
    std::cout<<"   ./hashverify -f data.bin -p 8"<<std::endl;
    std::cout<<"   ./hashverify -f data.bin -b 10"<<std::endl;
}

bool VerifyCommand::isDigit(int index)
{
    int len = std::strlen(this->argv[index+1]);
    for(int i=0; i<len; i++)
    {
        if(!std::isdigit(this->argv[index+1][i]))
            return false;    // The string contain char
    }
    return true;
}

void VerifyCommand::_f(int index)
// -f <filename>: Specify the filename
{
    int len = std::strlen(this->argv[index+1]);
    if(len > FILE_NAME_MAX_LENGTH)
    {
        std::cout<<"Invalid Parameter [-f]:"<<std::endl;
        std::cout<<"   The length of file name must shorter than 64 bytes"<<std::endl;
    }
    std::strcpy(this->commandLine.filename, this->argv[index+1]);
    std::ifstream file;
    file.open(this->commandLine.filename);
    if(!file)
    {
        std::cout << "File does not exists" << std::endl;
        exit(-1);
    }
}

void VerifyCommand::_p(int index)
// -p <print first elements>     [Default: 8]
{
    if(!isDigit(index))
    {
        std::cout<<"Invalid Parameter [-p]"<<std::endl;
        std::cout<<"   The parameter shoule be an integer, but ["<<this->argv[index+1]<<"] contains charactor"<<std::endl;
        exit(-7);
    }
    int print_first = std::atoi(this->argv[index+1]);
    this->commandLine.printFirst=print_first;
}

void VerifyCommand::_r(int index)
{
    if(!isDigit(index))
    {
        std::cout<<"Invalid Parameter [-r]"<<std::endl;
        std::cout<<"   The parameter shoule be an integer, but ["<<this->argv[index+1]<<"] contains charactor"<<std::endl;
        exit(-7);
    }
    int print_last = std::atoi(this->argv[index+1]);
    this->commandLine.printLast=print_last;
}

void VerifyCommand::_v(int index)
// -v <verify records>:         [Default: True]
{
    int len = std::strlen(this->argv[index+1]);
    char temp[len+1];
    std::strcpy(temp, this->argv[index+1]);
    if(strcmp(temp, "true")==0)
    {
        this->commandLine.verify_records = true;
    }
    else if(strcmp(temp, "false")==0)
    {
        this->commandLine.verify_records = false;
    }
    else
    {
        std::cout<<"Invalid Parameter [-v]:"<<std::endl;
        std::cout<<"   The parameter can only be [true] or [false]"<<std::endl;
        exit(-10);
    }
}

void VerifyCommand::_d(int index)
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

void VerifyCommand::_b(int index)
// -b <verifying random records>   [Default: 10]
{
    if(!isDigit(index))
    {
        std::cout<<"Invalid Parameter [-b]"<<std::endl;
        std::cout<<"   The parameter shoule be an integer, but ["<<this->argv[index+1]<<"] contains charactor"<<std::endl;
        exit(-7);
    }
    int random_records = std::atoi(this->argv[index+1]);
    this->commandLine.random_records=random_records;
}

void VerifyCommand::_h()
{
    std::cout<<"Help:"<<std::endl;
    std::cout<<"  -f <filename>: Specify the filename"                                                       <<std::endl;
    std::cout<<"  -p <num_records>: Specify the number of records to print from head    [Default: 8]"        <<std::endl;
    std::cout<<"  -r <num_records>: Specify the number of records to print from tail    [Default: 8]"        <<std::endl;
    std::cout<<"  -v <bool>: verify hashes from file, off with false, on with true      [Default: True]"     <<std::endl;
    std::cout<<"  -d <debug_model>: turns on debug mode with true, off with false       [Default: True]"     <<std::endl;
    std::cout<<"  -h <display>: Display this help message"                                                   <<std::endl;
}

void VerifyCommand::printCommand()
{
    if(this->commandLine.debug_model)
        std::cout<<"DEBUG=true"<<std::endl;
    else
        std::cout<<"DEBUG=false"<<std::endl;    // meaningless
    std::cout<<"FILENAME: "<<this->commandLine.filename<<std::endl;
    if(this->commandLine.verify_records)
        std::cout<<"VERIFY_RECORDS=true"<<std::endl;
    else
        std::cout<<"VERIFY_RECORDS=false"<<std::endl;

    // "Record": structure to hold a 16-byte record
    std::cout<<"RECORD_SIZE: "<<this->commandLine.record_size<<std::endl;
    std::cout<<"HASH_SIZE: "<<this->commandLine.hash_size<<std::endl;
    std::cout<<"NONCE_SIZE: "<<this->commandLine.nonce_size<<std::endl;
}

CommandLine VerifyCommand::get_commandLine()
{
    return this->commandLine;
}