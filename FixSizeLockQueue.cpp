#include "server.h"

void Server::FixSizeLockQueue::put(const std::string & s) {
	lock();
		if (data.size() == size) 
			data.pop_front();
		data.push_back(s);
	unlock();
} /* End of 'Server::FixSizeLockQueue::put' function */

