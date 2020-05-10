#include "BMPReader.h"

BMPReader::BMPReader(const char* file_name) {
	BMPReader::read(file_name);
}

void BMPReader::read(const char* file_name) {

	std::ifstream in(file_name, std::ios_base::binary);
	in.read((char*)&header, sizeof(header));
	in.read((char*)&header_info, sizeof(header_info));
	//change_padding(); TODO : write method to calculate padding
	//calculate_capacities(); and capacities
	data.resize(header_info.width * header_info.height);
	padd_vec.resize(padding);
	for (auto i = 0; i < header_info.height; ++i) {
		in.read((char*)(data.data() + i * header_info.width), header_info.width * sizeof(BMPPixel));
		in.read((char*)padd_vec.data(), padd_vec.size() * sizeof(BMPPixel));
	}
	header.file_size += (data.size() + padd_vec.size() * header_info.height) * sizeof(BMPPixel);
	in.close();
}