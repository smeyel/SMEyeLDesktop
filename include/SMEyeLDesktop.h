/*
 * SMEyeLDesktop.h
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#ifndef SMEYELDESKTOP_H_
#define SMEYELDESKTOP_H_

/* standard includes */
#include <iostream>
#include <map>

/* C++11 includes */
#include <memory> // shared_ptr

/* Framework includes */
#include <PlatformSpecifics.h>

/* project includes */
#include "Connection.h"

typedef std::map<std::string, std::shared_ptr<Connection> > ConnectionMap;

class SMEyeLDesktop {

	ConnectionMap connections;

public:
	SMEyeLDesktop();
	virtual ~SMEyeLDesktop();

	void run();

	bool addDevice(const std::string& host, const int& port = 6000);
//	bool addDevice(Device device);
};

#endif /* SMEYELDESKTOP_H_ */
