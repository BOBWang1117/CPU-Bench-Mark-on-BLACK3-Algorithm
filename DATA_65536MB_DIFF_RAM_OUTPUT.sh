echo "hashgen: Filesize 63356, Memory 1024/8192/32768" > DATA_65536MB_DIFF_RAM_OUTPUT.log

# experiments 1
./hashgen -t 16 -o 16 -i 16 -f data.bin -m 1024 -s 65536 -d false >> DATA_65536MB_DIFF_RAM_OUTPUT.log
echo "" >> DATA_65536MB_DIFF_RAM_OUTPUT.log

# experiments 2
./hashgen -t 16 -o 16 -i 16 -f data.bin -m 8192 -s 65536 -d false >> DATA_65536MB_DIFF_RAM_OUTPUT.log
echo "" >> DATA_65536MB_DIFF_RAM_OUTPUT.log

# experiments 3
./hashgen -t 16 -o 16 -i 16 -f data.bin -m 16384 -s 65536 -d false >> DATA_65536MB_DIFF_RAM_OUTPUT.log
echo "" >> DATA_65536MB_DIFF_RAM_OUTPUT.log

# experiments 4
./hashgen -t 16 -o 16 -i 16 -f data.bin -m 32768 -s 65536 -d false >> DATA_65536MB_DIFF_RAM_OUTPUT.log
echo "" >> DATA_65536MB_DIFF_RAM_OUTPUT.log