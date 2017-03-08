#include "Server_Task.h"


server_task::server_task() : ctx_(1),
frontend_(ctx_, ZMQ_ROUTER),
backend_(ctx_, ZMQ_DEALER)
{

}

void server_task::run()
{
	frontend_.bind("tcp://*:5570");
	backend_.bind("inproc://backend");

	std::vector<server_worker *> worker;
	std::vector<std::thread *> worker_thread;
	for (int i = 0; i < kMaxThread; ++i) {
		worker.push_back(new server_worker(ctx_, ZMQ_DEALER));

		worker_thread.push_back(new std::thread(std::bind(&server_worker::work, worker)));
		worker_thread->detach();
	}

	try {
		zmq::proxy(frontend_, backend_, nullptr);
	}
	catch (std::exception &e) {}

	for (int i = 0; i < kMaxThread; ++i) {
		delete worker;
		delete worker_thread;
	}
}
