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
#include <thread>

/* Framework includes */
#include <PlatformSpecifics.h>
#include <JsonMessage.h>
#include "LaserPointerTracker.h"

/* project includes */
#include "Connection.h"
#include "ImageHelper.h"
#include "OnMessageReceivedListener.h"
#include "Typedefs.h"

typedef std::map<std::string, std::shared_ptr<Connection> > ConnectionMap;
typedef std::vector<std::string> Args;

#define PROMPT "> "

/**
 * The main program that operates a command prompt and handles the commands.
 */
class SMEyeLDesktop : OnMessageReceivedListener {

	/** The active connections, indexed with the name of the device. */
	ConnectionMap connections;


	LaserPointerTracker tracker;

	void prompt(std::string msg = "");
	void print(std::string msg = "", bool newline = true);

	void handle_connect(Args& args);
	void handle_disconnect(Args& args);
	void handle_loglevel(Args& args);
	void handle_takepicture(Args& args);
	void handle_listdevices(Args& args);
	void handle_ts(Args& args);
	void handle_ts_findLed(Args& args);
	void handle_process_video(Args& args);

	void processFindled(JsonMessagePtr msg);
	void processTimesyncResponse(JsonMessagePtr);


public:
	SMEyeLDesktop();
	virtual ~SMEyeLDesktop();

	void run();

	bool addDevice(const std::string& host, const int& port = 6000);
	bool addDevice(Device& device);

	void onMessageReceived(JsonMessagePtr msg);
};

#endif /* SMEYELDESKTOP_H_ */
