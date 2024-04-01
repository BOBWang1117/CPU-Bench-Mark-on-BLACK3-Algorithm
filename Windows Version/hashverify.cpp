#include <iostream>
#include "my_verify_command.hpp"
#include "my_hash_verify.hpp"

int main(int argc, char* argv[])
{
    VerifyCommand myCommand(argc, argv);
    myCommand.command();
    CommandLine commandLine = myCommand.get_commandLine();

    const char* filename = commandLine.filename;                // -f <filename>: Specify the filename
    int printFirst = commandLine.printFirst;                    // -p <num_records>: Specify the number of records to print from head
    int printLast = commandLine.printLast;                      // -r <num_records>: Specify the number of records to print from tail
    bool verify_records = commandLine.verify_records;           // -v <bool> verify hashes from file, off with false, on with true
    int random_records = commandLine.random_records;            // -b <int>: 
    bool debug_model = commandLine.debug_model;                 // -d <debug_model>: turns on debug mode with true, off with false              [Default: True]

    HashVerify hashVerify(filename, printFirst, printLast, verify_records, random_records, debug_model);
    hashVerify.launch();

    return 0;
}