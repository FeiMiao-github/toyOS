#include <iostream>
#include <fstream>

int main(int arg, char** argv) {
    if (arg <= 1) {
        exit(EXIT_FAILURE);
    }

    std::fstream out;
    out.open(argv[1], std::ios::out | std::ios::binary | std::ios::in);
    out.seekp(0x1fe, std::ios::beg);
    std::cout << "write pointer: " << out.tellp();
    out << (char)0x55;
    out << (char)0xaa;
    out.close();
    return 0;
}