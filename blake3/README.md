# **BLACK3 Interface Running Manual**

## **BLACK3 Algorithm**

- [BLAKE3](https://github.com/BLAKE3-team/BLAKE3): BLACK3 Algorithm

- [BLAKE3 Offical Manual](https://github.com/BLAKE3-team/BLAKE3-specs/blob/master/blake3.pdf): BLACK3 paper

## **Replace Code**

- Location: blake3.h

- Details: 

    "#define BLAKE3_OUT_LEN 32" -> "#define BLAKE3_OUT_LEN 10"

## **Running (Windows)**

- Step 1:  Prepare a nonce file (Example: myFile)

- Step 2: Generate excutive file:

    - Method 1: 
        ```bash
        gcc -O3 -o example example.c blake3.c blake3_dispatch.c blake3_portable.c blake3_sse2_x86-64_windows_gnu.S blake3_sse41_x86-64_windows_gnu.S blake3_avx2_x86-64_windows_gnu.S blake3_avx512_x86-64_windows_gnu.S
        ```

    - Method 2:
        ```bash
        mingw32-make 
        ```

- Step 3:
```bash
Get-Content myFile | .\example
```

## **Running (Linux)**

- Step 1:  Prepare a nonce file (Example: myFile)

- Step 2: Generate excutive file:

    - Method 1: 
        ```bash
        gcc -O3 -o example example.c blake3.c blake3_dispatch.c blake3_portable.c blake3_sse2_x86-64_unix.S blake3_sse41_x86-64_unix.S blake3_avx2_x86-64_unix.S blake3_avx512_x86-64_unix.S
        ```

    - Method 2:
        ```bash
        make 
        ```

- Step 3:
```bash
./example < myFile
```