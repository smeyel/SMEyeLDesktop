/*
 * Connection.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#include "Connection.h"

/* standard includes */
#include <iostream>
#include <string>

/* C++11 includes */

/* Framework includes */

/* project includes */
#include "Log.h"


using namespace std;


Connection::Connection(Device& device)
		: device(device), socket(-1), connected(false)
{
	if (connect()) {
		Log::i("Connection", "Connected to " + this->device);
	} else {
		Log::w("Connection", "Couldn't connect to " + this->device);
	}
}

Connection::~Connection() {
	if (connected) {
		PlatformSpecifics::getInstance()->CloseSocket(socket);
		Log::d("Connection", "Disconnected from " + device);
	}
}

bool Connection::connect() {
	socket = PlatformSpecifics::getInstance()->Connect(device.getHost().c_str(), device.getPort());
	connected = (socket != -1);
	return connected;
}

void Connection::sendMessage(JsonMessage* msg, int socket) {
	string json = msg->toString();
}
