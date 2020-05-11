#include "BMPReader.h"

int main(){
	BMPReader bmp_file("img\\balls.bmp");
	bmp_file.write("out.bmp");
}

