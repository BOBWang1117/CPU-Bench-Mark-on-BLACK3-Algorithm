#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>

#define NONCE_SIZE 6
#define HASH_SIZE 10
// Structure to hold a 16-byte record
struct Record
{
    uint8_t hash[HASH_SIZE];        // hash value as byte array
    uint8_t nonce[NONCE_SIZE];      // Nonce value as byte array
};

void print_hash(std::vector<Record>& v);

int main(int argc, char* argv[])
{

    std::ifstream file(argv[1], std::ios::binary);
    file.seekg(0, file.end);
    uint32_t item_total = file.tellg() / sizeof(Record);
    file.seekg(0, file.beg);

    std::vector<Record> v;
    v.resize(item_total);
    file.read((char*) &v[0], item_total * sizeof(Record));

    print_hash(v);

    return 0;
}

void print_hash(std::vector<Record>& v)
{
    for(uint32_t j=0; j<v.size(); j++)
    {
        printf("HASH: ");
        for(int i=0; i<HASH_SIZE; i++)
        {
            printf("%02x", v[j].hash[i]);
        }
        printf(", Nonce: ");
        for(int i=0; i<NONCE_SIZE; i++)
        {
            printf("%02x", v[j].nonce[i]);
        }
        printf("\n");
    }
}
