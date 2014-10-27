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
#include <vector>

/* C++11 includes */
#include <memory> // shared_ptr

/* Framework includes */
#include <PlatformSpecifics.h>

/* project includes */
#include "Connection.h"

typedef std::map<std::string, std::shared_ptr<Connection> > ConnectionMap;
typedef std::vector<std::string> Args;

#define PROMPT "> "

class SMEyeLDesktop {

	ConnectionMap connections;

	void prompt(std::string msg = "");
	void print(std::string msg, bool newline = true);

	void handle_connect(Args& args);
	void handle_loglevel(Args& args);
	void handle_takepicture(Args& args);

public:
	SMEyeLDesktop();
	virtual ~SMEyeLDesktop();

	void run();

	bool addDevice(const std::string& host, const int& port = 6000);
	bool addDevice(Device& device);
};

#endif /* SMEYELDESKTOP_H_ */
