#ifndef MY_VERIFY_COMMAND_HPP
#define MY_VERIFY_COMMAND_HPP

#include <iostream>
#include <cstring>
#include <fstream>

#define FILE_NAME_MAX_LENGTH 64

struct CommandLine
{
    char filename[FILE_NAME_MAX_LENGTH];     // -f <filename>: Specify the filename
    int printFirst = -1;                     // -p <num_records>: Specify the number of records to print from head
    int printLast = -1;                      // -r <num_records>: Specify the number of records to print from tail
    bool verify_records = false;             // -v <verify records>:                                                         [Default: False]
    bool debug_model = true;                 // -d <debug_model>: turns on debug mode with true, off with false              [Default: True]
    int random_records = -1;                 // -b <verifying valid>

    int record_size = 16;
    int hash_size = 10;
    int nonce_size = 6;
};


class VerifyCommand{
public:
    VerifyCommand(int argc, char* argv[]);
    ~VerifyCommand();
    void command();
    CommandLine get_commandLine();

private:
    int argc;
    char** argv;
    CommandLine commandLine;

    void printCommand();
    bool isDigit(int index);
    void usage();
    void _f(int index);
    void _p(int index);
    void _r(int index);
    void _v(int index);
    void _d(int index);
    void _b(int index);
    void _h();
};


#endif