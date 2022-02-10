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
		convertedImage = frame;		

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
		  greyscale(frame, convertedImage);
		}
		else if (filterState == BLUR) {
		  blur5x5(frame, convertedImage);
		}
		else if (filterState == SOBELX) {
	          cv::Mat sobelx;
		  sobelx.create(frame.size(), frame.type());
		  sobelX3x3(frame, sobelx);
	          convertScaleAbs(sobelx, convertedImage);
		}
		else if (filterState == SOBELY) {
		  cv::Mat sobely;
		  sobely.create(frame.size(), frame.type());
		  sobelY3x3(frame, sobely);
		  convertScaleAbs(sobely, convertedImage);
		}
		else if (filterState == GRADIENT) {
		  cv::Mat sx;
		  sx.create(frame.size(), CV_16SC3);
		  sobelX3x3(frame, sx);

		  cv::Mat sy;
		  sy.create(frame.size(), CV_16SC3);
		  sobelY3x3(frame, sy);

		  cv::Mat mag;
	          mag.create(frame.size(), frame.type());
		  magnitude(sx, sy, mag);
		  convertScaleAbs(mag, convertedImage); 
		}
		else if (filterState == QUANT) {
		  blurQuantize(frame, convertedImage, 15);
		}
		else if (filterState == INVERT) {
		  invert(frame, convertedImage);
		}
		else if (filterState == CARTOON) {
		  cartoon(frame, convertedImage, 15, 20);
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
		else if (key == 'x') {
		  filterState = SOBELX;
		}
		else if (key == 'y') {
		  filterState = SOBELY;
		}				
		else if (key == 'm') {
		  filterState = GRADIENT;
		}
		else if (key == 'l') {
		  filterState = QUANT;
		}
		else if (key == 'i') {
		  filterState = INVERT;
		}
		else if (key == 'c') {
		  filterState = CARTOON;
		}
	}
	
	// terminate the video capture
	printf("Terminating\n");
	delete capdev;
	
	return(0);
}
