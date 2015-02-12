
#include "server.h"

int main() {
	Server s("127.0.0.1", 1234, 3);
	s.run();
	return 0;
} /* End of 'main' function */
