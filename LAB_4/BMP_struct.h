#pragma once
#include <stdint.h>

typedef struct {
    uint32_t size{ 0 };
    int32_t width{ 0 };
    int32_t height{ 0 };
    uint16_t planes{ 1 };
    uint16_t bit_count{ 24 };
    uint32_t compression{ 0 };
    uint32_t size_image{ 0 };
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };
    uint32_t colors_important{ 0 };
} BMPInfoHeader;

#pragma pack(push, 1)
typedef struct {
    uint16_t file_type{ 0x4D42 };
    uint32_t file_size{ 0 };
    uint16_t reserved1{ 0 };
    uint16_t reserved2{ 0 };
    uint32_t offset_data{ 0 };
} BMPFileHeader;
#pragma pack(pop)

typedef struct {
    uint8_t red{ 0 };
    uint8_t green{ 0 };
    uint8_t blue{ 0 };
} BMPPixel;

