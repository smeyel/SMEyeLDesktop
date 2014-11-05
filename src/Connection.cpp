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
#include <vector>

/* C++11 includes */

/* Framework includes */

/* project includes */
#include "Log.h"


using namespace std;


Connection::Connection(Device& device, OnMessageReceivedListener* callback)
		: device(device), callback(callback), socket(-1), connected(false)
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

	// will kill all still running threads.
}

bool Connection::connect() {
	socket = PlatformSpecifics::getInstance()->Connect(device.getHost().c_str(), device.getPort());
	connected = (socket != -1);
	return connected;
}

void Connection::receive(function<void(JsonMessagePtr)> callback) {
	cout << "receiving" << endl;
	vector<char> buffer;

	char c;
	while (PlatformSpecifics::getInstance()->recv(socket, &c, 1, 0)) {
		if (c != '#') {
			buffer.push_back(c);
		} else {
			break;
		}
	}

	string msgString(buffer.begin(), buffer.end());

	if (! msgString.empty()) {
		JsonMessagePtr msg(JsonMessage::parse(msgString.c_str()));

		msg->readAuxIfNeeded(socket);

		if (callback) {
			// callback is not empty.
			callback(msg);
		} else {
			// Callback is empty, use instance-level callback
			this->callback->onMessageReceived(msg);
		}

	} else {
		Log::e("Connection", "Empty message received from " + device.getName() + "!");
	}
}

void Connection::sendMessage(JsonMessage* msg, std::function<void(JsonMessagePtr)> callback) {
	if (connected) {
		string json = msg->toString(true);
		int result = PlatformSpecifics::getInstance()->send(socket, json.c_str(), json.size(), 0);
		if (result == json.size()) {
			Log::v("Connection", "Message sent to " + device.getName());
			threads.push_back(thread(&Connection::receive, this, callback));
		} else {
			Log::e("Conection", "Couldn't send message to " + device.getName() + "! Message: " + msg->toString());
		}
	} else {
		Log::e("Conection", "Couldn't send message to " + device.getName() + "! Not connected!");
	}
}
