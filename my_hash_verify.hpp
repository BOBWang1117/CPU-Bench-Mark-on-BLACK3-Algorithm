#ifndef MY_HASH_VERIFY_HPP
#define MY_HASH_VERIFY_HPP

#include <cstring>
#include <fstream>
#include <thread>

#include "my_verify_command.hpp"

#define NONCE_SIZE 6
#define HASH_SIZE 10
#define DATA_BLOCK_UNIT  64 * 1024   // generate 1 MB Record data [64 = 1024/sizeof(Record)]

// Structure to hold a 16-byte record
struct Record
{
    uint8_t hash[HASH_SIZE];        // hash value as byte array
    uint8_t nonce[NONCE_SIZE];      // Nonce value as byte array
};


class HashVerify
{
private:
    char filename[FILE_NAME_MAX_LENGTH];     // -f <filename>: Specify the filename
    int printFirst = -1;                     // -p <num_records>: Specify the number of records to print from head
    int printLast = -1;                      // -r <num_records>: Specify the number of records to print from tail
    bool verify_records = false;             // -v <verify records>:                                                         [Default: False]
    int verifying_valid = -1;                 // -b <verifying valid>
    bool debug_model = true;                 // -d <debug_model>: turns on debug mode with true, off with false              [Default: True]

    void Verify_Records();
    void print_thread();
    uint32_t filesize;
    uint32_t total_lines;
    uint32_t verify_record_index;
    int struct_size = sizeof(Record);
    bool print_exit = false;

    void Print_First();
    void Print_Last();
    void Verifying_Valid();

public:
    HashVerify(const char* filename, int printFirst, int printLast, bool verify_records, int verifying_valid,bool debug_model);
    ~HashVerify();
    void launch();

};

#endif