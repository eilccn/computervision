#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
	char label[256];
	int quit = 0;
	int frameid = 0;
	char buffer[256];
	std::vector<int> pars;

	pars.push_back(5);

	cv::VideoCapture *capdev;

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

	cv::namedWindow("Video", 1); // identifies a window?
	cv::Mat frame;

	
	for(;!quit;) {
		*capdev >> frame; // get a new frame from the camera, treat as a stream

		if( frame.empty() ) {
		  printf("frame is empty\n");
		  break;
		}
		
		cv::imshow("Video", frame);
		
		// see if there is a waiting keystroke
		int key = cv::waitKey(10);

		switch(key) {
		case 'q':
		    quit = 1;
		    break;

		    
		case 's': // capture a photo if the user hits s
		    sprintf(buffer, "%s.%03d.png", label, frameid++);
		    cv::imwrite(buffer, frame, pars);
		    printf("Image written: %s\n", buffer);
		    break;
		
		case 'g': // display greyscale live video
		    cv::Mat grayscale;
		    cvtColor(frame, grayscale, cv::COLOR_BGR2GRAY);  
		    cv::imshow("Video", grayscale);
		        
 
		    if (waitKey(30) == 27)
		    {
		    	break;
		    }
		    
		}


	}

	// terminate the video capture
	printf("Terminating\n");
	delete capdev;

	return(0);
}
