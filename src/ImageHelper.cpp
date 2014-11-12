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


ImageHelper::ImageHelper()
		: terminated(false), isChanged(false)
{
	thread = new std::thread(std::bind(&ImageHelper::run, this));
}

ImageHelper::~ImageHelper() {
	if (thread != nullptr) {
		this->terminate();
		thread->join();
		thread = nullptr;
	}
}

void ImageHelper::run() {

	std::unique_lock<std::mutex> lock(imageLock);

	while (! terminated) {
		// wait until the image is changed or we are  being terminated.
		imageCV.wait(lock, [&]() { return isChanged || terminated; });

		isChanged = false;

		for (auto& it : images) {
			cv::namedWindow(it.first);
			cv::imshow(it.first, *(it.second.get()));
			cv::waitKey(100);
		}

	}

	cv::destroyAllWindows();
}

void ImageHelper::show(cv::Mat& image, const std::string& winName) {
	std::unique_lock<std::mutex> lock(imageLock);
	std::shared_ptr<cv::Mat> localImage = std::make_shared<cv::Mat>(image);

	auto it = images.find(winName);

	if (it != images.end()) {
		it->second = localImage;
	} else {
		images.insert(std::map<std::string, std::shared_ptr<cv::Mat>>::value_type(winName, localImage));
	}

	isChanged = true;
	imageCV.notify_one();
}

void ImageHelper::terminate() {
	terminated = true;
	imageCV.notify_one();
}

ImageHelperPtr ImageHelper::instance;

ImageHelperPtr ImageHelper::get() {
	if (! instance) {
		/**
		 * Nasty hack to permit std::make_shared to create an instance of ImageHelper
		 * though the ImageHelper ctor is private.
		 * source: http://stackoverflow.com/a/25069711/1119508
		 */
		struct make_shared_enabler : public ImageHelper {};
		instance = std::make_shared<make_shared_enabler>();
	}
	return instance;
}

void ImageHelper::dispose() {
	if (instance) {
		instance = nullptr;
	}
}
