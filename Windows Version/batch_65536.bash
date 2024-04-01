echo "hashgen output filesize: 65536 Mbyte" > output_65536.txt

# experiments 1
./hashgen -t 1 -o 1 -i 1 -f data.bin -m 1024 -s 65536 -d false >> output_65536.log
echo "" >> output_65536.txt

# experiments 2
./hashgen -t 1 -o 1 -i 4 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 3
./hashgen -t 1 -o 1 -i 16 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 4
./hashgen -t 1 -o 4 -i 1 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 5
./hashgen -t 1 -o 4 -i 4 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 6
./hashgen -t 1 -o 4 -i 16 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 7
./hashgen -t 1 -o 16 -i 1 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 8
./hashgen -t 1 -o 16 -i 4 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 9
./hashgen -t 1 -o 16 -i 16 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 10
./hashgen -t 4 -o 1 -i 1 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 11
./hashgen -t 4 -o 1 -i 4 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 12
./hashgen -t 4 -o 1 -i 16 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 13
./hashgen -t 4 -o 4 -i 1 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 14
./hashgen -t 4 -o 4 -i 4 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 15
./hashgen -t 4 -o 4 -i 16 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 16
./hashgen -t 4 -o 16 -i 1 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 17
./hashgen -t 4 -o 16 -i 4 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 18
./hashgen -t 4 -o 16 -i 16 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 19
./hashgen -t 16 -o 1 -i 1 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 20
./hashgen -t 16 -o 1 -i 4 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 21
./hashgen -t 16 -o 1 -i 16 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 22
./hashgen -t 16 -o 4 -i 1 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 23
./hashgen -t 16 -o 4 -i 4 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 24
./hashgen -t 16 -o 4 -i 16 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 25
./hashgen -t 16 -o 16 -i 1 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 26
./hashgen -t 16 -o 16 -i 4 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt

# experiments 27
./hashgen -t 16 -o 16 -i 16 -f data.bin -m 1024 -s 65536 -d false >> output_65536.txt
echo "" >> output_65536.txt