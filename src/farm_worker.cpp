#include "farm_worker.hpp"

void* farm_worker::svc(void *input_task) {
	task *t = (task*) input_task;
	
	t->compute_frequencies();
	t->find_grey_threshold();
	//t->grey_threshold = 129;
	t->compute_histogram();
	//usleep(55145);
	return GO_ON;
}
