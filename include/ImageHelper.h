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


/**
 * A singleton helper class to simplify image displaying.
 *
 * One can call ImageHelper::get()->show(cv::Mat& image, std::string windowName) to display the image
 * in image in a window named windowName. Multiple calls with the same window name will refresh the
 * image shown in that window.
 *
 * The main program should call dispose if there will be no more images to display.
 */
class ImageHelper {

	/** The singleton instance. */
	static ImageHelperPtr instance;

	/** Flag to terminate the thread. */
	bool terminated;

	/**
	 * Flag to indicate that an image was really changed,
	 * and it's not just a spurious awakening.
	 */
	bool isChanged;

	/** The thread that owns the GUI and displays the images. */
	std::thread* thread;

	/** Lock on this before modifying the images map. */
	std::mutex imageLock;

	/** Wait for a notification on this object. */
	std::condition_variable imageCV;

	/** A map ow window names and image matrices that are displayed. */
	std::map<std::string, std::shared_ptr<cv::Mat>> images;

	ImageHelper();
	ImageHelper(ImageHelper&) = delete;
	ImageHelper(ImageHelper&&) = delete;

	~ImageHelper();

	/**
	 * An infinite loop running on the separate thread,
	 * waiting for changes and displaying the images.
	 */
	void run();

public:

	/** Get the singleton instance. Initialize it if it doesn't exist yet. */
	static ImageHelperPtr get();

	/** Dispose of the instance and call it's dtor. */
	static void dispose();


	/**
	 * Displays the image in a window named winName.
	 * Refreshes the window contents if a window with the same name exists.
	 */
	void show(cv::Mat& image, const std::string& winName);

	/** Signals the thread to terminate and destroy the windows. */
	void terminate();
};


#endif /* IMAGEHELPER_H_ */
