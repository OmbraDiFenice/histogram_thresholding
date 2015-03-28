#include <iostream>

#include <ff/farm.hpp>

#include "util.hpp"
#include "streamer.hpp"
#include "farm_worker.hpp"

using namespace ff;

void usage(char* exe) {
	std::cerr << "usage: \n\t"<<exe<<" <img_name> <threshold(%)> <streamlen> <nw>" << std::endl;
	std::cerr << std::endl << "<threshold> must be in [0,100]" << std::endl;
	std::cerr << "<streamlen> and <nw> must be positive integers" << std::endl;
}

int main(int argc, char* argv[]) {
	if(argc < 5) {
		usage(argv[0]);
		return 1;
	}

	char* filename = argv[1];
	int threshold = atoi(argv[2]);
	int streamlen = atoi(argv[3]);
	int nworkers = atoi(argv[4]);

	if(threshold < 0 || threshold > 100 || streamlen <= 0 || nworkers <= 0) {
		usage(argv[0]);
		return 1;
	}

	task **tasks = load(threshold, filename, streamlen);

	// farm
	ff_farm<> farm;
	std::vector<ff_node*> farm_workers;
	for(int i = 0; i < nworkers; i++) {
		farm_workers.push_back(new farm_worker);
	}
	farm.add_workers(farm_workers);
	farm.add_emitter(new streamer(tasks, streamlen));

	// execute
	ffTime(START_TIME);
	if (farm.run_and_wait_end()<0) {
		error("executing parallel application\n");
		return -1;
	}
	ffTime(STOP_TIME);

	//write(tasks, streamlen);

	// print service time
	double global_worker_time = ffTime(GET_TIME); //farm.ffwTime();
	std::cout << global_worker_time / streamlen << std::endl;
	farm.ffStats(std::cout);
	
	return 0;
}
