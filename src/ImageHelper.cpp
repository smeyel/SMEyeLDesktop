/*
 * ImageHelper.cpp
 *
 *  Created on: Nov 9, 2014
 *      Author: akos
 */

#include "ImageHelper.h"

/* standard includes */
#include <string>

/* C++11 includes */

/* Framework includes */

/* project includes */
#include "Log.h"


ImageHelper::ImageHelper() : terminated(false), isChanged(false)
{
}

void ImageHelper::run() {
	cv::namedWindow(winName);

	std::unique_lock<std::mutex> lock(imageLock);

	while (! terminated) {
		// wait until the image is changed or we are  being terminated.
		imageCV.wait(lock, [&]() { return isChanged || terminated; });

		isChanged = false;

		cv::imshow(winName, image);
		cv::waitKey(100);
	}

	cv::destroyWindow(winName);
}

void ImageHelper::show(cv::Mat& image) {
	std::unique_lock<std::mutex> lock(imageLock);
	this->image = image; // copy!
	isChanged = true;
	imageCV.notify_one();
}

void ImageHelper::terminate() {
	terminated = true;
	imageCV.notify_one();
}
