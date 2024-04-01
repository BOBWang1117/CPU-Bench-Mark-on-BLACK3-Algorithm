# **CS553 Cloud Computing: Assignment 4**

Illinois Institute of Technology

-----

>  Student:
>
>  [@Hongbo Wang](https://github.com/BOBWang1117)
>
>  

## **Environment:**

- Chameleon Cloud (CHI@TACC)

- CC-Ubuntu22.04


## **Details**

- Type: Assignment
- Professor: Ioan Raicu
- Developing Language: C++
- Project Name: Assignment 4
- Time: March/30/2024
- Description: In this project, the BLACK3 algorithm is employed to generate hash codes for the evaluation of hardware performance. The experiments consist of two components: one involves generating a 1024 MBytes file, while the other entails generating a larger 65536 MB (64GB) file. During the creation of the 1024 MB file, a maximum memory allocation of 128 MB is permitted. Hashing, sorting, and writing operations are executed utilizing 1, 4, and 16 threads respectively, resulting in a total of 27 sub-experiments. Similarly, when generating the 65536 MB file, the maximum memory allocation is increased to 1024 MB, while the other parameters remain consistent with the initial experiment.

## **Hardware:** 

Chameleon Cloud:

- CPU: 2 $\times$ Intel(R) Xeon(R) Platinum 8380 CPU @ 2.30GHz

- Memory: 16 $\times$ 16GB (256GB) of Micron HMA82GR7DJR8N-XN 16GB DDR4-3200 2Rx8 DIMM

- Disk: Micron MTFDDAK480TDS-1AW1ZABYY 5300 PRO 480GB, SATA

- Network: BCM5720 - Dual-Port 1GBASE-T

## **Install package:**

- [BLAKE3](https://github.com/BLAKE3-team/BLAKE3): BLACK3 Algorithm

- [BLAKE3_IN_C](https://github.com/BLAKE3-team/BLAKE3/tree/master/c): Installed library

## **Command Line Parameters**

hashgen:

- -t <num_threads_hash>: Specify the number of threads in hashing operation  [Default: 1]

- -o <num_threads_sort>: Specify the number of threads in sorting operation  [Default: 1]

- -i <num_threads_write>: Specify the number of threads in writing operation  [Default: 1]

- -f <filename>: Specify the filename

- -m <memory_size>: Specify the maximum memory allowed to use (MB) [Default: 128]

- -s <filesize>: Specify the hash file size (MB) [Default: 1024]

- -d <debug_model>: turns on debug mode with true, off with false  [Default: True]

- -h <help>: print helping manual page

hashverify:

- -f <filename>: Specify the filename

- -p <num_records>: Specify the number of records to print from head

- -r <num_records>: Specify the number of records to print from tail

- -v <verify_records>: [Default: False]

- -d <debug_model>: turns on debug mode with true, off with false  [Default: True]

- -b <verifying_valid>

- -h <help>: print helping manual page

## **Run program:**

1. Run make (Linux)

2. Running single command: 

    - Example: ./hashgen -t 16 -o 16 -i 16 -f data.bin -m 128 -s 1024 -d true

    - Example: ./hashverify -d true -f data.bin -v true


2. Batch Running Method:

    - Example: ./batch_1024.bash
