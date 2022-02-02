#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char *argv[]) {
	// declare variables
	char label[256]; // a string for image capture file
	int quit = 0;
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
	
	// create destination output
        cv::Mat convertedImage;

	// loop for various filter keys
	for(;!quit;) {
		cv::Mat frame;
                *capdev >> frame; // get a new frame from the camera, treat as a stream


		// test if video capture  was successful
                if( frame.empty() ) {
                  printf("frame is empty\n");
                  break;
                }
		
		/*	
		// loop over all rows
		for(i=0; i<src.rows; i++) {
		  // loop over all of columns
		  for (j=0; j<src.cols, j++) {
		    // apply filter and write result to destination
		    dst.at<cv::Vec3b>(i, j) = applyFilter( src, i, j ); // fxn returns pixel (cv::Vec3b)
		  }
		} 
		*/
		
		int last = waitKey(0);	
	
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
		  alt_greyscale(frame, convertedImage);
		}
		default: // 0 to get to original video
		{
		  convertedImage = frame;
		}
		}
		
		cv::imshow(window, convertedImage);
		
		int key = waitKey(1);
        	if (key != -1)
          	{
		  last = key;
		}
	}
	
	// terminate the video capture
	printf("Terminating\n");
	delete capdev;
	
	return(0);
}
