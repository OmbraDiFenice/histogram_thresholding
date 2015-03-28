#ifndef MY_FARM_WORKER_HPP
#define MY_FARM_WORKER_HPP

#include <ff/node.hpp>

#include "task.hpp"

class farm_worker : public ff::ff_node {
	public:
		void* svc(void *input_task);
};

#endif
