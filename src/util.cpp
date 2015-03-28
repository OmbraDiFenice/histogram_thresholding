#include "util.hpp"

using namespace std;

task** load(int threshold, string filename, int streamlen) {
	task **tasks = new task*[streamlen];

	task blueprint(0, threshold, filename);
	for(int i = 0;i < streamlen;i++) {
		tasks[i] = new task(i, blueprint);
	}

	return tasks;
}

void write(task **results, int streamlen) {
	for(int i = 0;i < streamlen;i++) {
		string out_filename("Hist");
		out_filename += to_string(i);
		out_filename +=".pgm";

		results[i]->write_image(out_filename);

		delete results[i];
	}
	
	delete results;
}
