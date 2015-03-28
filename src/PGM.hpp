#ifndef MY_PGM
#define MY_PGM

#include <iostream>
#include <fstream>
#include <string>
#include <cstring> // for memset

class PGM {
	std::string output_folder;

	unsigned char* alloc_matrix(int width, int height);

	public:
		int width, height;
		int maxval;
		unsigned char *image;
		
		PGM();
		PGM(const PGM& pgm);
		~PGM();
		
		void load(std::string filename);
		void write(std::string filename);
};

#endif
