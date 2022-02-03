#include <cstdio>
#import <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "filter.h"

using namespace cv;
using namespace std;

enum Filter {
	PREVIEW, GRAY, ALTGRAY, BLUR, SOBELX, SOBELY, GRADIENT, QUANT, CARTOON, INVERT
};

int main(int argc, char *argv[]) {
	int quit = 0;

	// declare variables for image capture
	char label[256]; // a string for image capture file
	int frameid = 0;
	char buffer[256];
	std::vector<int> pars;
	pars.push_back(5);

	// initialize video
	cv::VideoCapture *capdev;
	const char* window = "Main Window";
        cv::namedWindow(window, cv::WINDOW_AUTOSIZE);

	// open the video device
	capdev = new cv::VideoCapture(0);
	if( !capdev->isOpened() ) {
		printf("Unable to open video device\n");
		return(-1);
	}

	// get some properties of the image
	cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
		       (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));

	printf("Expected size: %d %d\n", refS.width, refS.height);

	// create instance of filterState to keep track of states	
	Filter filterState = Filter::PREVIEW;

	// loop for various filter keys
	for(;!quit;) {
		cv::Mat frame;
                *capdev >> frame; // get a new frame from the camera, treat as a stream

		// test if video capture  was successful
                if( frame.empty() ) {
                  printf("frame is empty\n");
                  break;
                }
		
	        // create destination output
        	cv::Mat convertedImage; 

		// if-else ladder for filter and corresponding cv::Mat converted image output
		if (filterState == PREVIEW) {
		  convertedImage = frame;
		}
		else if (filterState == GRAY) {
		  cv::Mat gray_image;
		  cv::cvtColor(frame, gray_image, cv::COLOR_BGR2GRAY);
		  convertedImage = gray_image;
                }
		else if (filterState == ALTGRAY) {
		  convertedImage = alt_greyscale(frame, convertedImage);
		}
		else if (filterState == BLUR) {
		  convertedImage = blur5x5(frame, convertedImage);
		}
		
		// load video
		cv::imshow(window, convertedImage);

		// wait for keypress
                int key = waitKey(1);

		// if-else ladder for keypress and corresponding filter
		if (key == 'q') {
		  quit = 1;
		  break;
		}
		else if (key == ' ') {
		  filterState = PREVIEW;
		}
		else if (key == 's') {
		  sprintf(buffer, "%s.%03d.png", label, frameid++);
                  cv::imwrite(buffer, convertedImage, pars);
                  printf("Image written: %s\n", buffer);
                }
		else if (key == 'g') {
		  filterState = GRAY;
		}
		else if (key == 'h') {
		  filterState = ALTGRAY;
		}
		else if (key == 'b') {
		  filterState = BLUR;
		}		
		
		/* ************************************
		// Alternative switch case for keypress (didn't work) 

		switch(last) {
		case 'q':
		{ 
		  quit = 1;
		  break;
		}
		case 's': // capture a photo if the user hits s
		{
		  sprintf(buffer, "%s.%03d.png", label, frameid++);
		  cv::imwrite(buffer, convertedImage, pars);
		  printf("Image written: %s\n", buffer);
		  break;
		}
		case 'g': // display greyscale live video
		{
		  cv::cvtColor(frame, convertedImage, cv::COLOR_BGR2GRAY);	  
		  break;
		}
		case 'h': // display alternative greyscale
		{
		  convertedImage = alt_greyscale(frame, convertedImage);
		  break; 
		}
		default: // 0 to get to original video
		{
		  convertedImage = frame;
		}
		}
		
		cv::imshow(window, convertedImage);
		
		key = waitKey(1);
  	      	if (key != -1)
          	{
		  last = key;
		}
		************************************** */
	}
	
	// terminate the video capture
	printf("Terminating\n");
	delete capdev;
	
	return(0);
}
