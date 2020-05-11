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
    BMPPixel get_pixel(uint32_t, uint32_t);
    void set_pixel(int32_t, int32_t, uint8_t, uint8_t, uint8_t);
    float lerp(float, float, float);
    float blerp(float, float, float, float, float, float);
    void change_padding();
    void calculate_capacities();
};