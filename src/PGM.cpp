#include "PGM.hpp"

using namespace std;

unsigned char* PGM::alloc_matrix(int width, int height) {
	unsigned char *m = new unsigned char[width*height];
	
	memset(m, 0, sizeof(unsigned char)*width*height);	

	return m;
}

PGM::PGM() {
	output_folder = "output/";
	width = height = maxval = 0;
	image = NULL;
}

PGM::PGM(const PGM& pgm) {
	output_folder = pgm.output_folder;
	width = pgm.width;
	height = pgm.height;
	maxval = pgm.maxval;

	image = alloc_matrix(width, height);
	for(int i = 0; i < width*height; i++) {
		image[i] = pgm.image[i];
	}
}

PGM::~PGM() {
	if(image != NULL) {
		delete image;
	}
}

void PGM::load(string filename) {
	ifstream file(filename, ifstream::in);

	if(file.fail()) {
		cerr << "Error: unable to open " << filename << " for reading" << endl;
		return;
	}

	string magic_number;
	file >> magic_number;
	if(magic_number != "P5") {
		cerr << "Error: " << filename << " is not a valid pgm format" << endl;
		return;
	}

	// read width, height and maxval, in this order
	// skipping blanks and comment lines (starting with '#')
	string strbuf;
	int *values[] = {&width, &height, &maxval};
	for(int i=0; i < 3; i++) {
		while(isspace(file.peek()) != 0) file.get();// consume blank characters
		file >> strbuf;
		if(strbuf[0] == '#') { // ignore comment lines
			getline(file, strbuf);
			i--;
		}
		else {
			*values[i] = stoi(strbuf);
		}
	}

	if(width < 0 || height < 0 || maxval < 0 || maxval > 255) {
		cerr << "Error: " << filename << " is not a valid pgm format" << endl;
		return;
	}

	// allocate memory
	image = alloc_matrix(width, height);

	// read image data
	file.read((char*)image, width*height);

	file.close();
}

void PGM::write(string filename) {
	filename = output_folder + filename;

	ofstream file(filename, ostream::out);

	if(file.fail()) {
		cerr << "Error: unable to open " << filename << " for writing" << endl;
		return;
	}

	// write header
	file << "P5" << endl;
	file << "# Generated using PGM class by Stefano Stoduto" << endl;
	file << width << " " << height << endl;
	file << maxval << endl;

	// write image data
	file.write((char*)image, width*height);

	file.close();
}
