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

/* C++11 includes */

/* Framework includes */
#include <PlatformSpecifics.h>

/* project includes */
#include "Device.h"

class Connection {

	const Device device;

	SOCKET socket;
	bool connected;

	bool connect();
public:
	Connection(Device& device);
	virtual ~Connection();

	bool isConnected() const { return connected; }

	const Device& getDevice() const { return device; }
};

#endif /* CONNECTION_H_ */
