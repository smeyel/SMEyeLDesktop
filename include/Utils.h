/*
 * Utils.h
 *
 *  Created on: Oct 25, 2014
 *      Author: akos
 */

#ifndef UTILS_H_
#define UTILS_H_

/* standard includes */
#include <string>
#include <vector>

/* C++11 includes */

/* Framework includes */

/* project includes */

namespace Utils {

std::vector<std::string> split(const std::string& str, const std::string& delimiters = " ");

void split(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ");


}  // namespace Utils

#endif /* UTILS_H_ */
