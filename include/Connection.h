/*
 * Connection.h
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

/* standard includes */
#include <string>
#include <vector>

/* C++11 includes */
#include <functional>
#include <thread>

/* Framework includes */
#include <JsonMessage.h>
#include <PlatformSpecifics.h>

/* project includes */
#include "Device.h"
#include "OnMessageReceivedListener.h"
#include "Typedefs.h"

/**
 * Represents a connection to a specific device.
 * Connects on creation.
 * Once connected it can deliver JsonMessage objects to the device and wait
 * for a response in a non-blocking way, that is a new std::thread is created
 * for each sent message.
 *
 * The received message can be processed either by an OnMessageReceivedListener
 * that was passed in the constructor,
 * or by a Callable object eg. an std::function passed to the receive method.
 */
class Connection {

	/** The device at the other end of this connection. */
	const Device device;

	/** Fallback callback, used if the Callable in the receive method is empty. */
	OnMessageReceivedListener* callback;

	/** The socket handler used to communicate with the device. */
	SOCKET socket;

	/** Flag to indicate the state of this connection. */
	bool connected;

	/**
	 * Container to hold the spawned threads,
	 * to be able to stop and free them in the destructor.
	 */
	std::vector<std::thread> threads;

	/**
	 * Uses the platform specific implementation to acquire a socket to the device,
	 * and sets the connected flag according to the result.
	 */
	bool connect();

	/**
	 * Runs on a separate thread for each message.
	 * Reads from the stream char-by-char, parses the received message,
	 * and calls the callback with the result message, or, if the callback argument is
	 * empty, calls the instance-specific callback.
	 */
	void receive(std::function<void(JsonMessagePtr)> callback);


public:
	/**
	 * Public ctor, takes a Device object that holds the address of the device
	 * to connect to, and an optional callback that should be called if no other
	 * callbacks are specified.
	 */
	Connection(Device& device, OnMessageReceivedListener* callback = nullptr);

	/** Should close the socket! */
	virtual ~Connection();

	/**
	 * Sends the message to the device and spawns a thread to receive and process
	 * the response.
	 *
	 * The callback parameter can be empty (std::function<void(JsonMessagePtr)>())
	 * and in that case the instance-specific callback will be called.
	 */
	void sendMessage(JsonMessage *msg, std::function<void(JsonMessagePtr)> callback);


	bool isConnected() const { return connected; }

	const Device& getDevice() const { return device; }
	SOCKET getSocket() const { return socket; }
};

#endif /* CONNECTION_H_ */
