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
typedef std::shared_ptr<std::thread> ThreadPtr;

struct DisplayThread {
	std::string winName;
	ImageHelperPtr instance;
	ThreadPtr thread;

	~DisplayThread();
};

class ImageHelper {
	static std::map<std::string, std::shared_ptr<DisplayThread>> threads;

	bool terminated;
	bool isChanged;

	const std::string winName;

	cv::Mat image;

	std::mutex imageLock;
	std::condition_variable imageCV;



	ImageHelper(const std::string& winName);
	ImageHelper(ImageHelper&) = delete;
	ImageHelper(ImageHelper&&) = delete;

public:

	static ImageHelperPtr init(const std::string& winName);
	static ImageHelperPtr get(const std::string& winName);
	static void dispose(const std::string& winName);

	static void dispose();

	void run();

	void show(cv::Mat& image);

	void terminate();
};


#endif /* IMAGEHELPER_H_ */
