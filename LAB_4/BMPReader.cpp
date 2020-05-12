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

void BMPReader::resize(const char* new_file, float scale) {
	int32_t new_height = (header_info.height * scale);
	int32_t new_width = (header_info.width * scale);
	new_width -= (new_width % 4 != 1) ? (new_width % 4) : 0;
	BMPReader new_image(new_height, new_width);
	for (auto x = 0; x < new_height; ++x) {
		float gx = (float)x / (new_height) * (header_info.height - 1);
		for (auto y = 0; y < new_width; ++y) {

			float gy = (float)y / (new_width) * (header_info.width - 1);
			int gxi = (int)gx;
			int gyi = (int)gy;

			BMPPixel c00 = get_pixel(gxi, gyi);
			BMPPixel c10 = get_pixel(gxi + 1, gyi);
			BMPPixel c01 = get_pixel(gxi, gyi + 1);
			BMPPixel c11 = get_pixel(gxi + 1, gyi + 1);

			uint8_t red = (uint8_t)blerp(c00.red, c10.red, c01.red, c11.red, gx - gxi, gy - gyi);
			uint8_t green = (uint8_t)blerp(c00.green, c10.green, c01.green, c11.green, gx - gxi, gy - gyi);
			uint8_t blue = (uint8_t)blerp(c00.blue, c10.blue, c01.blue, c11.blue, gx - gxi, gy - gyi);
			new_image.set_pixel(x, y, red, green, blue);
		}
	}
	new_image.write(new_file);
}