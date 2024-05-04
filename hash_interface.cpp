#include "hash_interface.hpp"

HashInterface::HashInterface()
{

}

HashInterface::~HashInterface()
{

}

int intRand(const int & min, const int & max) {
    static thread_local std::mt19937 generator(
            std::random_device{}() +
            std::hash<std::thread::id>{}(std::this_thread::get_id()) +
            std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void HashInterface::generateNonce(Record* record)
{
    for(int i=0; i<NONCE_SIZE; i++)
    {
        int random = intRand(0, 127);
        (*record).nonce[i] = RANDOM_TABLE[random];
    }
}

void HashInterface::generateHash(Record* record)
{
    blake3_hasher hasher;
    blake3_hasher_init(&hasher);
    blake3_hasher_update(&hasher, (*record).nonce, NONCE_SIZE);
    uint8_t output[BLAKE3_OUT_LEN]={0};
    blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);

    for(int i=0; i<HASH_SIZE; i++)
    {
        (*record).hash[i] = output[i];
    }
}

void HashInterface::getHash(Record* record)
{
    // step 1: generate nonce code and set in record
    this->generateNonce(record);

    //step 2: generate hash code and set in record
    this->generateHash(record);
}