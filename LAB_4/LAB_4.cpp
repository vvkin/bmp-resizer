#include "BMPReader.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cout << "Incorrect number of args\n";
        return 0;
    }
    BMPReader bitmapFile(argv[1]);
    std::cout << "Resizing image " << atof(argv[3]) << " times\n";
    bitmapFile.resize(argv[2], atof(argv[3]));
    std::cout << "Done\nResult written to " << argv[2] << '\n';
}

