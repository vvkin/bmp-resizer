#include "BMPReader.h"

BMPReader::BMPReader(const char* file_name) {
	BMPReader::read(file_name);
}

BMPReader::BMPReader(uint32_t height, uint32_t width) {
	header_info.height = height;
	header_info.width = width;
	change_padding();
	calculate_capacities();
	data.resize(header_info.width * header_info.height);
	padd_vec.resize(padding, BMPPixel{ 0,0,0 });
	header.file_size += (data.size() + padd_vec.size()) * sizeof(BMPPixel);
}

void BMPReader::read(const char* file_name) {

	std::ifstream in(file_name, std::ios_base::binary);
	in.read((char*)&header, sizeof(header));
	in.read((char*)&header_info, sizeof(header_info));
	change_padding();
	calculate_capacities();
	data.resize(header_info.width * header_info.height);
	padd_vec.resize(padding);
	for (auto i = 0; i < header_info.height; ++i) {
		in.read((char*)(data.data() + i * header_info.width), header_info.width * sizeof(BMPPixel));
		in.read((char*)padd_vec.data(), padd_vec.size() * sizeof(BMPPixel));
	}
	header.file_size += (data.size() + padd_vec.size() * header_info.height) * sizeof(BMPPixel);
	in.close();
}
void BMPReader::change_padding() {
	for (padding = 0; (header_info.width + padding) % 4 != 0; ++padding);
}

void BMPReader::calculate_capacities() {
	header_info.size = sizeof(BMPInfoHeader);
	header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
	header.file_size = header.offset_data;
}
void BMPReader::write(const char* file_name) {
	std::ofstream out(file_name, std::ios_base::binary);
	out.write((char*)&header, sizeof(header));
	out.write((char*)&header_info, sizeof(header_info));
	for (auto i = 0; i < header_info.height; ++i) {
		out.write((char*)(data.data() + i * header_info.width), header_info.width * sizeof(BMPPixel));
		out.write((char*)padd_vec.data(), padd_vec.size() * sizeof(BMPPixel));
	}
	out.close();
}
BMPPixel BMPReader::get_pixel(uint32_t x, uint32_t y) {
	return data[x * header_info.width + y];
}

void BMPReader::set_pixel(int32_t x, int32_t y, uint8_t redComponent, uint8_t greenComponent, uint8_t blueComponent) {
	BMPPixel* toChange = &data[x * header_info.width + y];
	toChange->red = redComponent;
	toChange->green = greenComponent;
	toChange->blue = blueComponent;
}

float BMPReader::lerp(float a, float b, float c) {
	return a + c * (b - a);
}

float BMPReader::blerp(float c00, float c10, float c01, float c11, float tx, float ty) {
	return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}