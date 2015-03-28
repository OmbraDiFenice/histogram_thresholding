#include "streamer.hpp"

void* streamer::svc(void* empty_task) {
	for(int i = 0; i < streamlen; i++) {
		ff_send_out(tasks[i]);
	}

	return NULL;
}
