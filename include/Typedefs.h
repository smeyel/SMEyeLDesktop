/*
 * Typedefs.h
 *
 *  Created on: Nov 2, 2014
 *      Author: akos
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

/* standard includes */

/* C++11 includes */
#include <functional> 	// std:function<T>
#include <memory> 		// std::shared_ptr

/* Framework includes */
#include <JsonMessage.h>

/* project includes */


typedef std::shared_ptr<JsonMessage> JsonMessagePtr;

typedef std::function<void(JsonMessagePtr)> MessageReceivedCallback;

#endif /* TYPEDEFS_H_ */
