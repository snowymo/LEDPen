#pragma once

#include <vector>
#include <thread>
#include <memory>
#include <functional>

#include <zmq.hpp>
#include "zhelpers.hpp"

//  Each worker task works on one request at a time and sends a random number
//  of replies back, with random delays between replies:

class server_worker {
public:
	server_worker(zmq::context_t &ctx, int sock_type)
		: ctx_(ctx),
		worker_(ctx_, sock_type)
	{}

	void work();

private:
	zmq::context_t &ctx_;
	zmq::socket_t worker_;
};
