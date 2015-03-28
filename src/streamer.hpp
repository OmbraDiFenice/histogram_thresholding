#ifndef MY_STREAMER_HPP
#define MY_STREAMER_HPP

#include "ff/node.hpp"
#include "task.hpp"

class streamer : public ff::ff_node {
	// parametri di ingresso del programma
	int streamlen;
	task **tasks;

	public:
		streamer(task **tasks, int streamlen) : tasks(tasks), streamlen(streamlen) {}
		void* svc(void* empty_task);
};

#endif
