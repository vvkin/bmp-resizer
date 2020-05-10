#pragma once
#include "BMP_struct.h"
#include <fstream>
#include <vector>

class BMPReader
{
    BMPInfoHeader header_info;
    BMPFileHeader header;
    std::vector<BMPPixel> data;
    int32_t padding;
    std::vector<BMPPixel> padd_vec;

public:
    BMPReader(const char*);
    BMPReader(uint32_t, uint32_t);
    void write(const char*);
    void resize(const char*, float);
private:
    void read(const char*);
};