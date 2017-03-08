#pragma once

#include <vector>
#include <thread>
#include <memory>
#include <functional>

#include <zmq.hpp>
#include "zhelpers.hpp"

//  This is our server task.
//  It uses the multithreaded server model to deal requests out to a pool
//  of workers and route replies back to clients. One worker can handle
//  one request at a time but one client can talk to multiple workers at
//  once.

class server_task {
public:
	server_task();

	enum { kMaxThread = 5 };

	void run();

private:
	zmq::context_t ctx_;
	zmq::socket_t frontend_;
	zmq::socket_t backend_;
};
