/*
 * SMEyeLDesktop.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#include "SMEyeLDesktop.h"

/* standard includes */
#include <iostream>
#include <string>
#include <algorithm>

/* C++11 includes */
#include <memory> // shared_ptr

/* Framework includes */

/* project includes */
#include "Log.h"
#include "Utils.h"


using namespace std;


SMEyeLDesktop::SMEyeLDesktop() {
	// TODO Auto-generated constructor stub

}

SMEyeLDesktop::~SMEyeLDesktop() {
	// TODO Auto-generated destructor stub
}

void SMEyeLDesktop::run() {
	cout << "Running!" << endl;

	while (true) {
		prompt();
		string input;
		std::getline(cin, input);

		Args args = Utils::split(input);

		if (args.size() == 0) {
			print("Invalid command! type 'help' for the list of commands!");
			continue;
		}

		string& command = args[0];

		if (command == "exit") {
			print("Goodbye!");
			break;
		} else if (command == "connect") {
			handle_connect(args);
		} else if (command == "loglevel") {
			handle_loglevel(args);
		}
	}
}

void SMEyeLDesktop::prompt(std::string msg) {
	cout << PROMPT << msg;
}

void SMEyeLDesktop::print(std::string msg, bool newline) {
	cout << msg;
	if (newline) {
		cout << endl;
	}
}

void SMEyeLDesktop::handle_connect(Args& args) {
	Device d;

	if (args.size() == 2) {			// just host
		d = Device(args[1]);
	} else if (args.size() == 3) {	// just host & port
		d = Device(args[1], std::stoi(args[2]));
	} else if (args.size() == 4) {	// name, host, port
		d = Device(args[1], args[2], std::stoi(args[3]));
	} else { 						// wrong argument list
		print("Wrong argument list!\nUsage:\tconnect [name] host [port]");
		return;
	}

	addDevice(d);
}

void SMEyeLDesktop::handle_loglevel(Args& args) {
	if (args.size() == 2) {
		char newLevel = args[1][0];
		// lowercase input
		newLevel = tolower(newLevel);
		Log::Levels level;

		switch (newLevel) {
			case 'v': level = Log::Verbose;	break;
			case 'd': level = Log::Debug;	break;
			case 'i': level = Log::Info;	break;
			case 'w': level = Log::Warning;	break;
			case 'e': level = Log::Error;	break;
			case 'f': level = Log::Fatal;	break;
			default:  level = Log::Verbose; break;
		}
		Log::setLevel(level);
		print("Current log level is " + Log::currentLevel());

	} else { // wrong argument list
		print("Wrong argument list!\nUsage:\tloglevel (v|d|i|w|e|f)\n\t\tloglevel (V|D|I|W|E|F)\n\t\tloglevel 1-6");
		return;
	}
}

bool SMEyeLDesktop::addDevice(Device& device) {
	auto c = shared_ptr<Connection>(new Connection(device));
	connections.insert(ConnectionMap::value_type(c->getDevice().getName(), c));

	return c->isConnected();
}

bool SMEyeLDesktop::addDevice(const std::string& host, const int& port) {
	Device d(host, port);
	return addDevice(d);
}
