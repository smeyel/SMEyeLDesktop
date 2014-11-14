/*
 * Log.h
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <string>

/**
 * Simple logging class using the common logging levels.
 * Easy to use, all-static implementation.
 * Just call Log::x(TAG, MESSAGE); where 'x' is the first character of the desired loglevel.
 */
class Log {

public:
	enum Levels {
		Verbose,
		Debug,
		Info,
		Warning,
		Error,
		Fatal
	};

	/**
	 * After this call, only events having a loglevel greater
	 * or equal to 'level' will be printed.
	 */
	static void setLevel(Levels level) {
		Log::level = level;
	}

	static Levels currentLevel() {
		return Log::level;
	}

	static void log(Levels level, std::string tag, std::string msg);

	static void v(std::string tag, std::string msg) { Log::log(Verbose, tag, msg); }
	static void d(std::string tag, std::string msg) { Log::log(Debug, tag, msg); }
	static void i(std::string tag, std::string msg) { Log::log(Info, tag, msg); }
	static void w(std::string tag, std::string msg) { Log::log(Warning, tag, msg); }
	static void e(std::string tag, std::string msg) { Log::log(Error, tag, msg); }
	static void f(std::string tag, std::string msg) { Log::log(Fatal, tag, msg); }

private:
	static Levels level;
};


#endif /* LOG_H_ */
