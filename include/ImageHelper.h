/*
 * ImageHelper.h
 *
 *  Created on: Nov 9, 2014
 *      Author: akos
 */

#ifndef IMAGEHELPER_H_
#define IMAGEHELPER_H_

/* standard includes */
#include <opencv2/opencv.hpp>

/* C++11 includes */
#include <condition_variable>
#include <mutex>
#include <thread>

/* Framework includes */

/* project includes */

class ImageHelper {
	bool terminated;

	cv::Mat image;

	const std::string winName = "Image";

	std::mutex imageLock;
	std::condition_variable imageCV;

	bool isChanged;


public:
	ImageHelper();
	ImageHelper(ImageHelper&) = delete;
	ImageHelper(ImageHelper&&) = delete;

	void run();
	void operator()() { run(); }

	void show(cv::Mat& image);

	void terminate();
};


#endif /* IMAGEHELPER_H_ */
