/*
 * Utils.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#include "Utils.h"

/* standard includes */
#include <string>
#include <vector>

/* C++11 includes */

/* Framework includes */

/* project includes */

using namespace std;
namespace Utils {

std::vector<std::string> split(const string& str, const string& delimiters) {
	vector<string> ret;
	split(str, ret, delimiters);
	return ret;
}

void split(const string& str, vector<string>& tokens, const string& delimiters)
{
	/* source: http://www.oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html */

    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

}  // namespace Utils
