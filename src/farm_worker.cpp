#include "farm_worker.hpp"

void* farm_worker::svc(void *input_task) {
	task *t = (task*) input_task;
	
	t->compute_frequencies();
	t->find_grey_threshold();
	t->compute_histogram();

	return GO_ON;
}
