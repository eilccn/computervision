/*
Eileen Chang
CS5330 
Spring 2022

Main file for camera calibration
*/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/dictionary.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "functions.h"

using namespace cv;
using namespace std;

int main( int argc, char *argv[] ) {

    // check for sufficient arguments
    if (argc < 1) {
        printf("Please enter enough arguments");
        return(-1);
    }

    // create aruco markers
    //createArucoMarkers();

    /* initialize variables */
    int quit =0;
    cv::Mat frame;
    cv::Mat convertedImage;
    convertedImage = frame;

    // chessboard parameters
    const float calibrationSquareDimension = 0.01778f; // meters
    const float arucoSquareDimension = 0.1016f; // meters
    cv::Size patternsize( 6, 9 );   

    // camera matrix
    cv::Mat cameraMatrix = cv::Mat::eye(3,3, CV_64FC1);
    // distance coefficients
    cv::Mat distanceCoeffs;
    // extract paths of individual images 
    std::vector<Mat> savedImages; 
    // image points found by findChessboardCorners
    std::vector<cv::Vec2f> corners; 
    // vector of vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f> > cornerList, rejectedCandidates;
    // 3D world points, constructed
    std::vector<cv::Vec3f> points; 
    // vector of vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Vec3f> > pointList; 

    // variables for image capture
	char label[256]; // a string for image capture file
	int frameid = 0;
	char buffer[256];
	std::vector<int> pars;
	pars.push_back(5);

    /* video capture */
    // open video device
    cv::VideoCapture *capdev; // opens video capture with eppoccam on mobile device
    capdev = new cv::VideoCapture(0);
    if(!capdev -> isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    int framesPerSecond = 20;
    cv::namedWindow("Main Window", cv::WINDOW_AUTOSIZE);

    /* loop for various functions */
    for(;!quit;) {

        *capdev >> frame; 

        if(frame.empty()) {
            printf("frame is empty\n");
            break;
        }
        

        /* detect and extract chessboard corners */
        bool found = false;
        found = findChessboardCorners(frame, patternsize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

        if(found) {
            /* refine pixel coordinates of 2D points for more accurracy */
            cv::Mat gray;
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            cv::TermCriteria criteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 30, 0.001);
            cv::cornerSubPix(gray, corners, cv::Size(11,11), cv::Size(-1,-1), criteria);

            /* draw to chessboard */
            frame.copyTo(convertedImage);
            drawChessboardCorners(convertedImage, patternsize, corners, found);
            
            imshow("Main Window", convertedImage);
        }
        else {
            imshow("Main Window", frame);
        }

        char character = waitKey(1000 / framesPerSecond);

        switch(character) {
            case 's':
                // saving image
                if (found) {
                    cv::Mat temp;
                    frame.copyTo(temp);
                    savedImages.push_back(temp);
                    
                    sprintf(buffer, "%s.%03d.png", label, frameid++);
                    cv::imwrite(buffer, convertedImage, pars);
                    printf("Image written: %s\n", buffer);

                }
                break;
            case 13:
                /* camera calibration */
                printf("Calibrating ...\n");
                // pass value of known 3D points (pointList) 
                // and corresponding pixel coordinates of detected corners (cornerList)
                if(savedImages.size() > 5) {
                    cameraCalibration(savedImages, patternsize, calibrationSquareDimension, cameraMatrix, distanceCoeffs);
                    saveCameraCalibration("Calibration Matrix", cameraMatrix, distanceCoeffs);
                }
                else {
                    printf("Not enough images saved for calibration");
                }
            
                break;
            case 'q':
                // exit program
                quit=1;
                break;
        }

    }

    // terminate the video capture
	printf("Terminating\n");
    delete capdev;


    //loadCameraCalibration("Calibration Matrix", cameraMatrix, distanceCoeffs);

    // find camera location
    //startWebcamMonitoring(cameraMatrix, distanceCoeffs, 0.099f);

    return 0;
}