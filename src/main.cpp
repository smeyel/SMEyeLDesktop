/*
 * main.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

/* standard includes */
#include <iostream>

/* C++11 includes */
#include <memory> // shared_ptr

/* Framework includes */

/* project includes */
#include "SMEyeLDesktop.h"

using namespace std;

int main() {

	cout << "Starting SMEyeLDesktop app." << endl;

	SMEyeLDesktop app;
	app.run();

	app.addDevice("192.168.1.107");
}

