/*
 * Log.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#include "Log.h"

/* standard includes */
#include <iostream>
#include <string>

using namespace std;

Log::Levels Log::level = Log::Verbose;

void Log::log(Levels level, string tag, string msg) {
	if (level >= Log::level) {
		switch (level) {
			case Verbose: 	cout << 'V'; break;
			case Debug: 	cout << 'D'; break;
			case Info: 		cout << 'I'; break;
			case Warning: 	cout << 'W'; break;
			case Error: 	cout << 'E'; break;
			case Fatal: 	cout << 'F'; break;
			default:
				break;
		}
		cout << " - " << tag << ": " << msg << endl;
	}
}
