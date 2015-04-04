#include "task.hpp"

using namespace std;

task::task(int threshold, string filename) {
	input_image.load(filename);

	int size = input_image.maxval;
	freq = new int[size];
	memset(freq, 0, sizeof(int)*size);

	// convert the percentage to the corresponding number of pixel
	input_threshold = (double) input_image.width * (double) input_image.height / 100.0 * (double) threshold;
	grey_threshold = 0;
}

task::task(const task& copy) : input_image(copy.input_image){
	int size = input_image.maxval;
	freq = new int[size];
	for(int i = 0; i < size; i++) {
		freq[i] = copy.freq[i];
	}

	input_threshold = copy.input_threshold;
	grey_threshold = copy.grey_threshold;
}

task::~task() {
	delete freq;
}

void task::compute_frequencies() {
	int len = input_image.height * input_image.width;
	unsigned char *image = input_image.image;

	for(int i = 0; i < len; i++) {
		freq[(int)image[i]]++;
	}
}

void task::find_grey_threshold() {
	double sum = 0;
	int i;
	for(i = input_image.maxval-1; i >= 0 && sum < input_threshold; i--) {
		sum += freq[i];
	}
	grey_threshold = i;
}

void task::compute_histogram() {
	int len = input_image.height * input_image.width;
	unsigned char *image = input_image.image;
	unsigned char white = (unsigned char) input_image.maxval;
	unsigned char black = 0;
	
	for(int i = 0; i < len; i++) {
		if((int) input_image.image[i] > grey_threshold) {
			input_image.image[i] = white;
		}
		else {
			input_image.image[i] = black;
		}
	}
}

void task::write_image(string filename) {
	input_image.write(filename);
}
