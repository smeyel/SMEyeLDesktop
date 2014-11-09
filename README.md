# SMEyeLDesktop

Desktop client for the AndroidTracker in the SMEyeL project.

## Usage

Run SMEyeLDesktop from terminal (or preferrably wrap it in [rlwrap](https://github.com/hanslub42/rlwrap) like this:`rlwrap ./SMEyeLDesktop`).

Once running it works as a shell, with the following recognized commands:

#### connect
`connect <device name> <ip-address> <port>`

Tries to connect to the specified device. The application can connect to multiple devices simultaneously (by multiple executions of this command).

#### loglevel
`loglevel (v|d|i|w|e|f)`

Sets the loging level to the specified one (the letters stand for the usual levels, from _verbose_ to _fatal_).

#### takepicture or tp
`tp <device name> <filename>`

Sends the specified device a [JsonMessage](https://github.com/smeyel/Framework/blob/akos/libCommunication/include/JsonMessage.h) subclass instructing it to take a picture and send it back.
The <filename> prameter is yet unused.

#### ts
`ts <device name>`

Instructs the specified device to take a picture and send it back, but instead of simply displaying it, performs some transformations in it, in order to find the time synchronization LED.

_Only available on the **ts_findLed** branch!_

## How to compile

This repository is a valid Eclipse C++ project, so it can be imported to Eclipse, and run from there.

Also, there's a Makefile, that can perform the necessary steps to produce an executable.

#### Dependencies
* A C++11 capable compiler (tested with g++)
* The OpenCV installed (tested with OpenCV 2.4.9)
* The proper version of the [Framework repository](https://github.com/smeyel/Framework/tree/akos) (currently the _akos_ branch)

  Specific dependencies in the Framework:
  * libCommunication
  * libLogConfigTime
  * libPlatformSpecifics
  
* The latest version of the [Tracking repository](https://github.com/smeyel/Tracking/tree/cba489d1aae229cbea83fd7611b5d8b3c1f5fb04)
