#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <vector>
#include "functions.h"
#include "features.csv"
#include "csv_util.h"

using namespace cv;
using namespace std;

enum Filter {
	PREVIEW, THRESHOLD, MORPH, CC, FEATURES, TRAINING, CLASSIFY, KNN_CLASSIFIER
};

int main(int argc, char *argv[]) {
	
	int quit = 0;

	/*  check for sufficient arguments 
    if (argc < 2) {
        std::cout << "Please enter: \n[argv0]: ./project3 \n"
            "[argv1]: csv file path \n"
            "[argv2]: still image path (if exists) \n" << std::endl;
        exit(-1);
    }
	

	// check for image path
    cv::Mat img = cv::imread(argv[3]);
	if (img.empty()) {
        std::cout << "Not a valid image file" << std::endl;
        return -1;
    }
	*/

	// declare variables for image capture
	char label[256]; // a string for image capture file
	int frameid = 0;
	char buffer[256];
	std::vector<int> pars;
	pars.push_back(5);

	// initialize video
	cv::VideoCapture *capdev;
	const char* window = "converted image";
    cv::namedWindow(window, cv::WINDOW_AUTOSIZE);

	// open the video device
	capdev = new cv::VideoCapture(1);
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
		//cv::imshow("original image", frame); // display original image

		// test if video capture  was successful
        if( frame.empty() ) {
            printf("frame is empty\n");
            break;
        }
		
	    // create destination output
        cv::Mat convertedImage; 
		convertedImage = frame;		

		// initialize vector for training feature set
    	std::vector<double> training_featureset;

		// initialize csv file variables
		std::fstream fileStream;
		fileStream.open(argv[1]);
		char outputfile[256]; 
		strcpy(outputfile, argv[1]);
		
		// initialize vectors for the read function to push data into
    	std::vector<char *> obj_labels; // vector for object label names
    	std::vector<std::vector<double>> db_featureset; // vector for feature set data
		
		/** if-else ladder for computing tasks 1-5:
		 * thresholding
		 * morphological filtering
		 * connected components
		 * moments
		 * extract features into a database
		 **/
		if (filterState == PREVIEW) {
			// original image
			convertedImage = frame;
		}
		else if (filterState == THRESHOLD) {
			// call thresholding algorithm function 
			threshold(frame, convertedImage);
        }
		else if (filterState == MORPH) {
			// call morphological filtering function
			morphological(frame, convertedImage);
		}
		else if (filterState == CC) {
			// call connected components function
			conn_comp(frame, convertedImage);
		}
		else if (filterState == FEATURES) {
			// call features function 
			features(frame, convertedImage, training_featureset);
		}
		else if (filterState == TRAINING) {
			// call training_set function
			// compute features, write features to csv file
			training(frame, convertedImage, outputfile);
			
			// return back to original image after writing data to the csv for a single object
			// press the 'n' keypress again to enter training mode and write data for each new object
			filterState = FEATURES;
		}
		else if (filterState == CLASSIFY) {
			// read csv file and obtain vectors for object names and object featuresets
			read_image_data_csv( outputfile, obj_labels, db_featureset, 0 );

			// initialize vector for unknown object featureset
			std::vector<double> unknown_featureset;
			
			// compute unknown object featureset
			features(frame, convertedImage, unknown_featureset);

			// call classify function to find best object match  
			classify(frame, convertedImage, unknown_featureset, obj_labels, db_featureset);

		}
		else if (filterState == KNN_CLASSIFIER) {
			// read csv file and obtain vectors for object names and object featuresets
			read_image_data_csv( outputfile, obj_labels, db_featureset, 0 );

			// initialize vector for unknown object featureset
			std::vector<double> unknown_featureset;
			
			// compute unknown object featureset
			features(frame, convertedImage, unknown_featureset);

			// call KNN function to find best object match 
			KNN(frame, convertedImage, unknown_featureset, obj_labels, db_featureset);
		}
		

		// load video
		cv::imshow(window, convertedImage); // display filtered image

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
		else if (key == 't') {
		  filterState = THRESHOLD;
		}
		else if (key == 'm') {
		  filterState = MORPH;
		}
		else if (key == 'c') {
		  filterState = CC;
		}		
		else if (key == 'f') {
		  filterState = FEATURES;
		}
		else if (key == 'n') {
		  filterState = TRAINING;
		}
		else if (key == 'd') {
			filterState = CLASSIFY;
		}
		else if (key == 'k') {
			filterState = KNN_CLASSIFIER;
		}
	}
	
	// terminate the video capture
	printf("Terminating\n");
	delete capdev;
	
	return(0);
}
