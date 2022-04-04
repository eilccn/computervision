/*
Eileen Chang
CS5330 
Spring 2022

Main file
*/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "csv_util.h"

using namespace cv;
using namespace std;

//const float calibrationSquareDimension = 0.02032; //meters
//const float arucoSquareDimension = 

// what size chessboard to use
cv::Size patternsize( 8, 6 ); 

int createKnownBoardPosition(cv::Size boardSize, std::vector<Point3f>& corners) {
    /* define world coordinates for 3D points */
    for(int i=0; i<patternsize.width; i++) {
        for(int j=0; j<patternsize.height; j++) {
            corners.push_back(cv::Point3f(j,i,0));
        }
    }
    return 0;
}

int getChessboardCorners(std::vector<Mat> images, vector<vector<Point2f> >& cornerList, bool showResults = false) {

    for(std::vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++) {
        std::vector<Point2f> corners;
        bool found = findChessboardCorners(*iter, patternsize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            cornerList.push_back(corners);
        }
        if (showResults) {
            drawChessboardCorners(*iter, patternsize, corners, found);
            imshow("Looking for Corners", *iter);
            waitKey(0);
        }
    }
    return 0;
}

int main( int argc, char *argv[] ) {
    
    int quit = 0;

    // check for sufficient arguments
    if (argc < 1) {
        printf("Please enter enough arguments");
        return(-1);
    }

    /* initialize variables */
    cv::Mat frame;
    cv::Mat drawToFrame;
    cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat distanceCoeffs;
    // extract paths of individual images 
    std::vector<Mat> savedImages; 
    // image points found by findChessboardCorners
    std::vector<cv::Point2f> corners; 
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
    cv::VideoCapture vid(1); // opens video capture with eppoccam on mobile device
    if(!vid.isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    int framesPerSecond = 20;
    cv::namedWindow("Main Window", cv::WINDOW_AUTOSIZE);

    /* loop for various functions */
    for(;!quit;) {

        if(!vid.read(frame)) {
            printf("frame is empty\n");
            break;
        }

        bool found = false;

        /* detect and extract chessboard corners */
        found = findChessboardCorners(frame, patternsize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
        
        if(found) {
            // refine pixel coordinates of 2D points for more accurracy
            cv::Mat gray;
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            cv::TermCriteria criteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 30, 0.001);
            cv::cornerSubPix(gray, corners, cv::Size(11,11), cv::Size(-1,-1), criteria);
            
            // draw to chessboard
            frame.copyTo(drawToFrame);
            drawChessboardCorners(drawToFrame, patternsize, corners, found);
            
            imshow("Main Window", drawToFrame);
        }
        else {
            imshow("Main Window", frame);
        }

        char character = waitKey(1000 / framesPerSecond);

    }




    /* camera calibration */
    // pass value of known 3D points (pointList) 
    // and corresponding pixel coordinates of detected corners (cornerList)
    /*
    cv::calibrateCamera(pointList, cornerList, cv::Size(gray.rows, gray.cols), cameraMatrix, distCeoffs, R, T);

    cout << "cameraMatrix: " << cameraMatrix << endl;
    cout << "distCoeffs: " << distCeoffs << endl;
    cout << "Rotation vector: " << R << endl;
    cout << "Translation Vector: " << T << endl;
    */

    return 0;
}