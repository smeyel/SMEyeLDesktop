/*
 * SMEyeLDesktop.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#include "SMEyeLDesktop.h"

/* standard includes */
#include <iostream>

/* C++11 includes */
#include <memory> // shared_ptr

/* Framework includes */

/* project includes */


using namespace std;


SMEyeLDesktop::SMEyeLDesktop() {
	// TODO Auto-generated constructor stub

}

SMEyeLDesktop::~SMEyeLDesktop() {
	// TODO Auto-generated destructor stub
}

void SMEyeLDesktop::run() {
	cout << "Running!" << endl;
}

bool SMEyeLDesktop::addDevice(const std::string& host, const int& port) {
	Device d(host, port);
	auto c = shared_ptr<Connection>(new Connection(d));
	connections.insert(ConnectionMap::value_type(c->getDevice().getName(), c));

	return c->isConnected();
}
