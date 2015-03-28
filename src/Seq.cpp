#include <iostream>
#include <ff/utils.hpp>
#include "task.hpp"
#include "util.hpp"

using namespace std;

void usage(char* exe) {
	cerr << "usage: \n\t"<<exe<<" <img_name> <threshold(%)> <streamlen>" << endl;
	cerr << endl << "<threshold> must be in [0,100]" << endl;
	cerr << "<streamlen> must be a positive integer" << endl;
}

int main(int argc, char* argv[]) {
	if(argc < 4) {
		usage(argv[0]);
		return 1;
	}

	char* filename = argv[1];
	int threshold = atoi(argv[2]);
	int streamlen = atoi(argv[3]);

	if(threshold < 0 || threshold > 100 || streamlen < 0) {
		usage(argv[0]);
		return 1;
	}

	task **tasks = load(threshold, filename, streamlen);

	ff::ffTime(ff::START_TIME);
	for(int i = 0; i < streamlen; i++) {
		task *t = tasks[i];
		
		t->compute_frequencies();
		t->find_grey_threshold();
		//t->grey_threshold = 129;
		t->compute_histogram();

	}
	ff::ffTime(ff::STOP_TIME);
	
	// print "service time"
	cout << ff::ffTime(ff::GET_TIME) / streamlen << endl;

	//write(tasks, streamlen);

	return 0;
}
