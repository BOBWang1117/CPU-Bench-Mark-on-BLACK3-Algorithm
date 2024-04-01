#include <iostream>
#include <fstream>
#include <stdint.h>
#include <random>
#include "./blake3/blake3.h"
#include <thread>

#define NONCE_SIZE 6
#define HASH_SIZE 10



struct Record
{
    uint8_t hash[HASH_SIZE];        // hash value as byte array
    uint8_t nonce[NONCE_SIZE];      // Nonce value as byte array
};

int main()
{
    // Record* r = new Record[5];
    // recordUnitBlock(r);

    // std::ofstream outFile("test.bin", std::ios::binary | std::ios::app);
    // for(int i=0; i<5; i++)
    // {
    //     outFile.write(reinterpret_cast<const char*>(&(r[i])), sizeof(Record));
    // }

    // Record record_1={{0x9f,0xc3,0x7c,0x30,0x34,0x07,0x06,0x07,0x08,0x09},{0x9f,0xc3,0x7c,0x30,0x34,0x07}};
    // Record record_2={{0x9f,0xc3,0x7c,0x30,0x34,0x07,0x06,0x07,0x08,0x09},{0x9f,0xc3,0x7c,0x30,0x34,0x07}};
    // Record record_3={{0x9f,0xc3,0x7c,0x30,0x34,0x07,0x06,0x07,0x08,0x09},{0x9f,0xc3,0x7c,0x30,0x34,0x07}};
    // Record record_4={{0x9f,0xc3,0x7c,0x30,0x34,0x07,0x06,0x07,0x08,0x09},{0x9f,0xc3,0x7c,0x30,0x34,0x07}};

    // ofstream outFile("data.bin", ios::binary);
    // outFile.write(reinterpret_cast<const char*>(&record_1), sizeof(record_1));
    // outFile.write(reinterpret_cast<const char*>(&record_2), sizeof(record_2));
    // outFile.write(reinterpret_cast<const char*>(&record_3), sizeof(record_3));
    // outFile.write(reinterpret_cast<const char*>(&record_4), sizeof(record_4));
    // outFile.close();

    // Record record_in[5] = {0};
    // // unsigned int size = 0;
    // std::ifstream infile("test.bin", std::ios::binary);
    // infile.read((char*) &record_in[0], sizeof(Record));
    // infile.read((char*) &record_in[1], sizeof(Record));
    std::ifstream inFile("data.bin", std::ios::binary);
    inFile.seekg(0, std::ios::end); // Move the file pointer to the end of the file
    std::streampos fileLength = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    int struct_size = sizeof(Record);
    int array_size = fileLength/struct_size;
    std::cout<<array_size<<std::endl;
    Record* record = new Record[array_size];
    for(int i=0; i<array_size; i++)
    {
        inFile.read((char*) &record[i], struct_size);
    }
    inFile.close();

    for(int i=0; i<array_size; i++)
    {
        if(i == 14088952 || i == 14088953 || i == 14088954 || i == 14088955)
        {
            std::cout<<"hash: ";
            for(int j=0; j<HASH_SIZE; j++)
            {
                printf("%02x", record[i].hash[j]);
            }
            std::cout<<", nonce: ";
            for(int j=0; j<NONCE_SIZE; j++)
            {
                printf("%02x", record[i].nonce[j]);
            }
            std::cout<<std::endl;
        }
    }

    return 0;
}
