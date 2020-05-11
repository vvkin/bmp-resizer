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