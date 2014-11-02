/*
 * OnMessageReceivedListener.h
 *
 *  Created on: Nov 2, 2014
 *      Author: akos
 */

#ifndef ONMESSAGERECEIVEDLISTENER_H_
#define ONMESSAGERECEIVEDLISTENER_H_

/* standard includes */

/* C++11 includes */

/* Framework includes */

/* project includes */
#include "Typedefs.h"

class OnMessageReceivedListener {
public:

	virtual void onMessageReceived(JsonMessagePtr msg) = 0;


	/* To suppress warning. */
	virtual ~OnMessageReceivedListener() {}
};


#endif /* ONMESSAGERECEIVEDLISTENER_H_ */
