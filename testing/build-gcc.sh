cd ~
rm cpp-lib -r
mkdir cpp-lib
echo "copying files"
cp /mnt/c/code/cpp-lib/{includes,impl,testing} ./cpp-lib -r
cd cpp-lib
echo "compiling"
g++ -o testing/testing.elf -g -Wall -Wextra -Wpedantic -Wconversion -std=c++2a **/*.cpp -lpthread
