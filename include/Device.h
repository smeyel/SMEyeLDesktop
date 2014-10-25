/*
 * Device.h
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#ifndef DEVICE_H_
#define DEVICE_H_

/* standard includes */
#include <iostream>
#include <string>

/* C++11 includes */

/* Framework includes */

/* project includes */

class Device {

	friend std::ostream& operator<<(std::ostream& o, const Device& dev);

	std::string host;
	int port;

	std::string name;

public:

	Device();
	Device(const std::string& host, const int& port = 6000);
	Device(const std::string& name, const std::string& host, const int& port = 6000);
	virtual ~Device();

	const std::string& getHost() const { return host; }

	void setHost(const std::string& host) { this->host = host; }

	const std::string& getName() const { return name; }

	void setName(const std::string& name) { this->name = name; }

	int getPort() const { return port; }

	void setPort(int port) { this->port = port; }

	operator std::string() const;
};

std::ostream& operator<<(std::ostream& o, const Device& dev);
const char* operator+(const char* text, const Device& dev);

#endif /* DEVICE_H_ */
