#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <Poco/Thread.h>
#include "twitter.h"

using namespace std;

//#define PORT 48120


class Server {
public:
	static Server* get_instance() {
		if (n == 0)
			instance = new Server();

		return instance;
	}

	void connect();
	string read_socket();
	string write_socket(string s);

private:
	Server() { n++; }

	static int n;
	static Server* instance;
};


#endif

