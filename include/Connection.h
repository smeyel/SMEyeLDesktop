/*
 * Connection.h
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

/* standard includes */
#include <string>
#include <vector>

/* C++11 includes */
#include <functional>
#include <thread>

/* Framework includes */
#include <JsonMessage.h>
#include <PlatformSpecifics.h>

/* project includes */
#include "Device.h"
#include "OnMessageReceivedListener.h"
#include "Typedefs.h"

class Connection {

	const Device device;
	OnMessageReceivedListener* callback;

	SOCKET socket;
	bool connected;


	std::vector<std::thread> threads;


	bool connect();

	void receive();


public:
	Connection(Device& device, OnMessageReceivedListener* callback);
	virtual ~Connection();

	void sendMessage(JsonMessage *msg);
	bool isConnected() const { return connected; }

	const Device& getDevice() const { return device; }
	SOCKET getSocket() const { return socket; }
};

#endif /* CONNECTION_H_ */
