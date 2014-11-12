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
#include <string>

/* C++11 includes */
#include <condition_variable>
#include <mutex>
#include <thread>
#include <memory>

/* Framework includes */

/* project includes */

class ImageHelper;

typedef std::shared_ptr<ImageHelper> ImageHelperPtr;


class ImageHelper {

	static ImageHelperPtr instance;

	bool terminated;
	bool isChanged;

	std::thread* thread;

	std::mutex imageLock;
	std::condition_variable imageCV;

	std::map<std::string, std::shared_ptr<cv::Mat>> images;

	ImageHelper();
	ImageHelper(ImageHelper&) = delete;
	ImageHelper(ImageHelper&&) = delete;

	~ImageHelper();

public:

	static ImageHelperPtr get();
	static void dispose();


	void run();

	void show(cv::Mat& image, const std::string& winName);

	void terminate();
};


#endif /* IMAGEHELPER_H_ */
