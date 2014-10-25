/*
 * Device.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#include "Device.h"

/* standard includes */
#include <string>
#include <sstream>

/* C++11 includes */

/* Framework includes */

/* project includes */

using namespace std;



Device::Device()
		: host(""), port(0), name("Anonymous")
{
}

Device::Device(const std::string& host, const int& port)
		: host(host), port(port), name(host + ':' + to_string(port))
{
}

Device::Device(const std::string& name, const std::string& host, const int& port)
		: Device(host, port)
{
	this->name = name;
}

Device::~Device() {
}

std::ostream& operator <<(std::ostream& o, const Device& dev) {
	o << dev.name << " (" << dev.host << ":" << dev.port << ")";
	return o;
}

Device::operator std::string() const {
	stringstream ss;
	ss << *this;
	return ss.str();
}

const char* operator +(const char* text, const Device& dev) {
	return (string(text) + (dev.operator std::string())).c_str();
}
