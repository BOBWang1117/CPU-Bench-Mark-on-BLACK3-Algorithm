#include "my_hash_verify.hpp"

HashVerify::HashVerify(const char* filename, int printFirst, int printLast, bool verify_records, int verifying_valid, bool debug_model)
{
    std::strcpy(this->filename, filename);          // -f <filename>: Specify the filename
    this->printFirst = printFirst;                  // -p <num_records>: Specify the number of records to print from head
    this->printLast = printLast;                    // -r <num_records>: Specify the number of records to print from tail
    this->verify_records = verify_records;          // -v <verify records>:                                                         [Default: False]
    this->verifying_valid = verifying_valid;        // -b <verifying valid>
    this->debug_model = debug_model;                // -d <debug_model>: turns on debug mode with true, off with false              [Default: True]
}

HashVerify::~HashVerify()
{

}

bool compare(Record const& slow, Record const& quick)
{
    if(slow.hash[0] < quick.hash[0])
        return true;
    else if(slow.hash[0] > quick.hash[0])
        return false;
    else if(slow.hash[1] < quick.hash[1])
        return true;
    else if(slow.hash[1] > quick.hash[1])
        return false;
    else if(slow.hash[2] < quick.hash[2])
        return true;
    else if(slow.hash[2] > quick.hash[2])
        return false;
    else if(slow.hash[3] < quick.hash[3])
        return true;
    else if(slow.hash[3] > quick.hash[3])
        return false;
    else if(slow.hash[4] < quick.hash[4])
        return true;
    else if(slow.hash[4] > quick.hash[4])
        return false;
    else if(slow.hash[5] < quick.hash[5])
        return true;
    else if(slow.hash[5] > quick.hash[5])
        return false;
    else if(slow.hash[6] < quick.hash[6])
        return true;
    else if(slow.hash[6] > quick.hash[6])
        return false;
    else if(slow.hash[7] < quick.hash[7])
        return true;
    else if(slow.hash[7] > quick.hash[7])
        return false;
    else if(slow.hash[8] < quick.hash[8])
        return true;
    else if(slow.hash[8] > quick.hash[8])
        return false;
    else if(slow.hash[9] < quick.hash[9])
        return true;
    else if(slow.hash[9] > quick.hash[9])
        return false;
    else
        return true;
}

void HashVerify::Verify_Records()
{
    std::ifstream inFile(this->filename, std::ios::binary);
    inFile.seekg(0, std::ios::end);
    this->filesize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    // int struct_size = sizeof(Record);
    this->total_lines = filesize / sizeof(Record);

    Record slow;
    inFile.read((char*) &slow, this->struct_size);
    Record quick;
    this->verify_record_index = 1;
    while(true)
    {
        if(this->verify_record_index>=this->total_lines)
        {
            break;
        }
        inFile.read((char*) &quick, this->struct_size);
        if(!compare(slow, quick))
        {
            std::cout<<"Read "<<this->verify_record_index<<"/"<<this->total_lines<<" line data wrong [exit(-1])"<<std::endl;
            exit(-1);
        }
        slow = quick;

        this->verify_record_index = this->verify_record_index + 1;
    }
    // for(this->verify_record_index=1; this->verify_record_index<this->total_lines; this->verify_record_index++)
    // {
    //     inFile.read((char*) &quick, this->struct_size);
    //     if(!compare(slow, quick))
    //     {
    //         std::cout<<"Read "<<this->verify_record_index<<"/"<<this->total_lines<<" line data wrong [exit(-1])"<<std::endl;
    //         exit(-1);
    //     }
    //     slow = quick;
    // }
    this->print_exit = true;
}

void HashVerify::Print_First()
{
    std::cout<<"Printing first "<<this->printFirst<<" of file '"<<this->filename<<"'..."<<std::endl;
    std::ifstream inFile(this->filename, std::ios::binary);

    for(int i=0; i<this->printFirst; i++)
    {
        Record record;
        inFile.read((char*) &record, this->struct_size);
        printf("[%3d] Hash: ", i*16);
        
        for(int j=0; j<HASH_SIZE; j++)
        {
            printf("%02x", record.hash[j]);
        }
        std::cout<<" : ";
        for(int j=0; j<NONCE_SIZE; j++)
        {
            printf("%02x", record.nonce[j]);
        }
        printf("\n");
    }
}
void HashVerify::Print_Last()
{
    std::cout<<"Printing tail "<<this->printLast<<" of file '"<<this->filename<<"'..."<<std::endl;
    std::ifstream inFile(this->filename, std::ios::binary);

    for(int i=0; i<this->printLast; i++)
    {
        Record record;
        inFile.seekg(-16*(i+1), std::ios::end);
        inFile.read((char*) &record, this->struct_size);

        printf("[%u] Hash: ", this->filesize - (i+1)*16);
        for(int j=0; j<HASH_SIZE; j++)
        {
            printf("%02x", record.hash[j]);
        }
        std::cout<<" : ";
        for(int j=0; j<NONCE_SIZE; j++)
        {
            printf("%02x", record.nonce[j]);
        }
        printf("\n");
    }
}
void HashVerify::Verifying_Valid(){}

void HashVerify::launch()
{
    if(this->verify_records)
    {
        std::thread print_th(&HashVerify::print_thread, this);
        this->Verify_Records();
        print_th.join();
        std::cout<<"Read "<<this->filesize<<" bytes and found all records are sorted."<<std::endl;
    }
    else if(this->printFirst != -1)
    {
        this->Print_First();
    }
    else if(this->printLast != -1)
    {
        this->Print_Last();
    }
    else if(this->verifying_valid != -1)
    {   
        this->Verifying_Valid();
    }
}

void HashVerify::print_thread()
{
    int time = 1;
    uint32_t complete_per_second = 0;
    uint32_t Mbytes_per_second = 0;
    while(this->debug_model)
    {
        if(this->print_exit)
        {
            break;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));       // sleep 1 second
        Mbytes_per_second = (this->verify_record_index - complete_per_second) / 64 / 1024;      // 64 = 1024/16 [sizeof(Record)]
        printf("[%d][VERIFY]: ", time);
        printf("%.2f%% completed, ", 100.0*this->verify_record_index/this->total_lines);
        printf("ETA %.1f seconds, ", 1.0*(this->total_lines-this->verify_record_index)/(this->verify_record_index/time));
        std::cout<<this->verify_record_index<<"/"<<this->total_lines<<" read, ";
        // printf("%lld/%lld read, ", this->verify_record_index, this->total_lines);
        printf("%d MB/sec", Mbytes_per_second);
        printf("\n");
        complete_per_second = this->verify_record_index;
        time++;
    }
}