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
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/* C++11 includes */
#include <memory> // shared_ptr

/* Framework includes */
#include "JpegMessage.h"
#include "TakePictureMessage.h"
#include "TimesyncRequestMessage.h"
#include "TimesyncResponseMessage.h"

/* project includes */
#include "Log.h"
#include "Utils.h"


using namespace std;

const std::string mainWinName = "Main image";

const std::string TAG = "SMEyeLDesktop";


SMEyeLDesktop::SMEyeLDesktop() {
	tracker.init("tracker.ini");
}

SMEyeLDesktop::~SMEyeLDesktop() {
	ImageHelper::dispose();
}

void SMEyeLDesktop::run() {
	cout << "Running!" << endl;

	while (true) {
		prompt();
		string input;
		std::getline(cin, input);
		if (cin.eof()) {
			print("Goodbye!");
			break;
		}

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
		} else if (command == "disconnect") {
			handle_disconnect(args);
		} else if (command == "loglevel") {
			handle_loglevel(args);
		} else if (command == "tp" || command == "takepicture") {
			handle_takepicture(args);
		} else if (command == "ls" || command == "listdevices") {
			handle_listdevices(args);
		} else if (command == "ts") {
			handle_ts(args);
		} else if (command == "tsf") {
			handle_ts_findLed(args);
		} else if (command == "pv") {
			handle_process_video(args);
		} else {
			print("Unrecognized command. Type 'help' for the list of commands!");
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

void SMEyeLDesktop::handle_disconnect(Args& args) {
	auto it = connections.find(args[1]);

	if (it == connections.end()) {
		Log::d("SMEyeLDesktop", "Device not found, can't disconnect.");
		return;
	}

	if (it->second->isConnected()) {
		Log::d("SMEyeLDesktop", "Device is connected, disconnecting.");
	}

	connections.erase(it);
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
		print("Current log level is ", false);
		switch (Log::currentLevel()) {
			case Log::Verbose: print("Verbose"); break;
			case Log::Debug: print("Debug"); break;
			case Log::Info: print("Info"); break;
			case Log::Warning: print("Warning"); break;
			case Log::Error: print("Error"); break;
			case Log::Fatal: print("Fatal"); break;
			default: print("UNSPECIFIED!!!"); break;
		}

	} else { // wrong argument list
		print("Wrong argument list!\nUsage:\tloglevel (v|d|i|w|e|f)\n\tloglevel (V|D|I|W|E|F)");
		return;
	}
}

/**
 * tp <device name> <dest filename> [<desired timestamp>]
 */
void SMEyeLDesktop::handle_takepicture(Args& args) {
	if (args.size() < 3 || args.size() > 4) {
		print("Wrong argument list!\nUsage:\ttp <device name> <dest filename> [<desired timestamp>]");
		return;
	}

	TakePictureMessage msg;

	if (args.size() == 4) {		// immediate picture request
		msg.setDesiredTimestamp(std::atoll(args[2].c_str()));
	}

	if (! connections.count(args[1])) {
		print("Unknown device name!");
		return;
	}

	shared_ptr<Connection> conn(connections[args[1]]);
	conn->sendMessage(&msg, std::function<void(JsonMessagePtr)>());
}

void SMEyeLDesktop::handle_listdevices(Args& args) {
	print("Connected devices:");
	for (auto & c : connections) {
		if (c.second->isConnected()) {
			print(c.second->getDevice());
		}
	}
}

void SMEyeLDesktop::processFindled(JsonMessagePtr msg) {
	using namespace cv;

	cout << "processing findled" << endl;
	shared_ptr<JpegMessage> jpegMsg = dynamic_pointer_cast<JpegMessage>(msg);
	if (jpegMsg.get() != nullptr) {
		shared_ptr<cv::Mat> rgb(new cv::Mat(480, 640, CV_8UC3));
		jpegMsg->Decode(rgb.get());


		tracker.processFrame(*(rgb.get()));

		Point2i led = tracker.getLastPoint();

		Log::d("processfindled", "Point is (" + std::to_string(led.x) + "," + std::to_string(led.y) + ")");

		circle(*(rgb.get()), led, 20, Scalar(255, 0, 0), 2, 8, 0);

		ImageHelper::get()->show(*(rgb.get()), mainWinName);
	}
}


/**
 * ts <device>
 */
void SMEyeLDesktop::handle_ts_findLed(Args& args) {
	TakePictureMessage msg;

	if (! connections.count(args[1])) {
		print("Unknown device name!");
		return;
	}

	shared_ptr<Connection> conn(connections[args[1]]);
	using std::placeholders::_1;
	conn->sendMessage(&msg, bind(&SMEyeLDesktop::processFindled, this, _1));
}

void SMEyeLDesktop::handle_process_video(Args& args) {
	if (args.size() != 2) {
		print("Wrong argument list!\nUsage:\tpv <filename>");
		return;
	}

	LaserPointerTracker tracker;
	tracker.init("tracker.ini");
	tracker.setShowWindows(false);

	cv::VideoCapture capturer(args[1]);
	cv::Mat frame;

	std::vector<int> status;
	bool isStarted = false;

	while (capturer.read(frame)) {
		tracker.reset();
		tracker.processFrame(frame);
		tracker.processFrame(frame);

		Point2i last = tracker.getLastPoint();

		if (!isStarted && last != Point2i(0, 0)) {
			isStarted = true;
		}

		if (isStarted) {
			if (last == Point2i(0, 0)) {
				status.push_back(0);
			} else {
				status.push_back(1);
			}
		}

//		Log::d("ProcessVideo", "(" + std::to_string(last.x) + "," + std::to_string(last.y) + ")");
	}

	stringstream ss;
	for (auto& it : status) {
		ss << it << " ";
	}

	Log::d("ProcessVideo", ss.str());
}

/**
 * Command handler for time synchronization. Sends a TimesyncRequestMessage
 * to the device found in args, or emits a warning if no device is found.
 */
void SMEyeLDesktop::handle_ts(Args& args) {
	TimesyncRequestMessage msg;

	if (! connections.count(args[1])) {
		print("Unknown device name!");
		return;
	}

	shared_ptr<Connection> conn(connections[args[1]]);
	using std::placeholders::_1;
	conn->sendMessage(&msg, bind(&SMEyeLDesktop::processTimesyncResponse, this, _1));
}

/**
 * Processes the time synchronization response message. Iterates over the
 * received brightness-timestamp pairs and prints them to stdout.
 */
void SMEyeLDesktop::processTimesyncResponse(JsonMessagePtr msg) {
	shared_ptr<TimesyncResponseMessage> tsMsg = dynamic_pointer_cast<TimesyncResponseMessage>(msg);
	if (tsMsg.get() != nullptr) {
		stringstream ss;
		for (auto& it : tsMsg->getValues()) {
			ss << it.brightness << "@" << it.timestamp << " ";
		}

		Log::d("ProcessTimesync", ss.str());
	}
}

void SMEyeLDesktop::onMessageReceived(JsonMessagePtr msg) {
	cout << "Received:" << endl << msg->toString() << endl;

	shared_ptr<JpegMessage> jpegMsg = dynamic_pointer_cast<JpegMessage>(msg);
	if (jpegMsg.get() != nullptr) {
		shared_ptr<cv::Mat> image(new cv::Mat(480, 640, CV_8UC3));
		jpegMsg->Decode(image.get());
		ImageHelper::get()->show(*(image.get()), mainWinName);
	}
}

bool SMEyeLDesktop::addDevice(Device& device) {
	auto c = shared_ptr<Connection>(new Connection(device, this));
	connections.insert(ConnectionMap::value_type(c->getDevice().getName(), c));

	return c->isConnected();
}

bool SMEyeLDesktop::addDevice(const std::string& host, const int& port) {
	Device d(host, port);
	return addDevice(d);
}
