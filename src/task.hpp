#ifndef MY_TASK
#define MY_TASK

#include <string>
#include <cstring> // for memset

#include "PGM.hpp"

// Main computation structure
class task {

	public:
		PGM input_image;
		int input_threshold; // number of pixel needed to exceed the threshold

		int* freq; // grey shades frquencies
		int grey_threshold; // index of the threshold grey shade
	
		task(int threshold, std::string filename);
		task(const task& copy);
		~task();

		void compute_frequencies();
		void find_grey_threshold();
		void compute_histogram();

		void write_image(std::string filename);
};

#endif
