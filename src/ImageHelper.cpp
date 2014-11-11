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


ImageHelper::ImageHelper(const std::string& winName)
		: terminated(false), isChanged(false), winName(winName)
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

std::map<std::string, std::shared_ptr<DisplayThread>> ImageHelper::threads;

ImageHelperPtr ImageHelper::init(const std::string& winName) {
	if (threads.count(winName)) {
		// already initialized.
		return get(winName);
	}

	/**
	 * Nasty hack to permit std::make_shared to create an instance of ImageHelper
	 * though the ImageHelper ctor is private.
	 * source: http://stackoverflow.com/a/25069711/1119508
	 */
	struct make_shared_enabler : public ImageHelper {
	public:
		make_shared_enabler(const std::string& winName) : ImageHelper(winName) {}
	};

	typedef std::shared_ptr<DisplayThread> DisplayThreadPtr;

	DisplayThreadPtr dtp = std::make_shared<DisplayThread>();

	dtp->winName = winName;
	dtp->instance = std::make_shared<make_shared_enabler>(winName);
	dtp->thread = std::make_shared<std::thread>(std::bind(&ImageHelper::run, dtp->instance));

	threads.insert(std::map<std::string, DisplayThreadPtr>::value_type(winName, dtp));

	return dtp->instance;
//	return get(winName);
}

ImageHelperPtr ImageHelper::get(const std::string& winName) {
	auto it = threads.find(winName);

	if (it == threads.end()) {
		// doesn't exist -> init a new one.
		return init(winName);
	} else {
		return it->second->instance;
	}
}

void ImageHelper::dispose(const std::string& winName) {
	auto it = threads.find(winName);

	if (it == threads.end()) {
		Log::w("ImageHelper", "Can't dispose '" + winName + "'! It's not found!");
		return;
	} else {
		threads.erase(it);
		Log::d("ImageHelper", "Disposed '" + winName + "' instance and thread.");
	}
}

void ImageHelper::dispose() {
	/* Deleting the DisplayThread objects is enough, as it's members
	 * are shared_ptrs which when deleted, clean up after themselves.
	 * And the DisplayThread dtor handles the terminating of the thread.
	 */
	threads.clear();
}

DisplayThread::~DisplayThread() {
	instance->terminate();
	thread->join();
}
