
#include "server.h"
/*
template<class Type> void Server::FixSizeLockQueue<Type>::put(Type const & s) {
	lock();
		if (data.size() == size) 
			data.pop_front();
		data.push_back(s);
	unlock();
} /* End of 'Server::FixSizeLockQueue::put' function */

