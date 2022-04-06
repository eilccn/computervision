/*
Eileen Chang
CS5330 
Spring 2022

Main file for AR functionalities (Tasks 4-7)
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
    cv::Size patternsize( 9, 6 );   

    // camera matrix
    double col = double(convertedImage.cols / 2);
    double rows = double(convertedImage.rows / 2);
    double data[3][3] = { {1, 0, col}, {0, 1, rows}, {0, 0, 1} };
    cv::Mat cameraMatrix = cv::Mat(3,3, CV_64FC1, &data);

    //cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat distanceCoeffs = cv::Mat::zeros(8, 1, CV_64FC1);

    // extract paths of individual images 
    std::vector<Mat> savedImages; 
    // image points found by findChessboardCorners
    std::vector<cv::Point2f> corners; 
    // vector of vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f> > cornerList, rejectedCandidates;
    // 3D world points, constructed
    std::vector<cv::Point3f> points; 
    // vector of vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Point3f> > pointList; 
    // solvePnP rvecs and tvecs
    cv::Mat rotation_values, translation_values;

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

    bool flag = false;

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
            cornerList.clear();
            pointList.clear();
            points.resize(0);

            /* refine pixel coordinates of 2D points for more accurracy */
            cv::Mat gray;
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            cv::TermCriteria criteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 30, 0.001);
            cv::cornerSubPix(gray, corners, cv::Size(11,11), cv::Size(-1,-1), criteria);

            /* draw to chessboard */
            frame.copyTo(convertedImage);
            drawChessboardCorners(convertedImage, patternsize, corners, found);
            imshow("Main Window", convertedImage);

            // push corners
            cornerList.push_back(corners);

            // push points
            /* define world coordinates for 3D points */
            for(int i=0; i<patternsize.width; i++) {
                for(int j=0; j<patternsize.height; j++) {
                    points.push_back(cv::Point3d(j * calibrationSquareDimension, i * calibrationSquareDimension, 0.0f));
                }
            }
            
            
        }
        else {
            imshow("Main Window", frame);
        }

        // wait for keypress
        char character = waitKey(1000 / framesPerSecond);

        // switch case
        switch(character) {
            case 's':
                // save image to computer
                if (found) {
                    // saving images for calibration
                    cv::Mat temp;
                    frame.copyTo(temp);
                    savedImages.push_back(temp);

                    // save image to computer
                    sprintf(buffer, "%s.%03d.png", label, frameid++);
                    cv::imwrite(buffer, convertedImage, pars);
                    printf("Image written: %s\n", buffer);

                }
                break;
            case 'q':
                // exit program
                quit=1;
                break;
            case 13:
                // turn flag on for solvePnP
                flag = true;
                break;
            case 'f':
                // turn flag off for solvePnP
                flag = false;
                break;
            case 'c':
                /* camera calibration */
                printf("Calibrating ...\n");
                
                if(savedImages.size() > 5) { // require more than 5 images to be saved
                    // camera calibration function
                    cameraCalibration(savedImages, patternsize, calibrationSquareDimension, cameraMatrix, distanceCoeffs);
                    // save data from calibration to file
                    storeCameraConfig(cameraMatrix, distanceCoeffs);
                }
                else {
                    printf("Not enough images saved for calibration");
                }
                
                break;
            case 'd':
                if (found) {
                    // draw axes 
                    printf("Drawing (x,y,z) axes ...\n");
                    // read calibration data
                    readCameraConfig(cameraMatrix, distanceCoeffs);
                    cv::Mat draw_axes;
                    frame.copyTo(draw_axes);

                    // draw corners
                    drawChessboardCorners(draw_axes, patternsize, corners, found);

                    // draw frame axes
                    cv::drawFrameAxes(draw_axes, cameraMatrix, distanceCoeffs, rotation_values, translation_values, 10.0f, 3);
                    imshow("Axes", draw_axes);
                }
            
                break;
            /* 
            case 't':
                // place 3D virtual object

                break;
            case 'r':
                // detect robust features
                
                break;
            */
                
        }


        if (flag) {
            /* calculate board's pose (rotation and translation) */
            printf("Calculating board's pose (rotation and translation) ...\n");
            // read calibration data
            readCameraConfig(cameraMatrix, distanceCoeffs);


            //cout << "POINTS" << endl;
            std::vector<Point3d> objp;
            for(auto &i : points) {
                objp.push_back(i);
                //cout << i << endl;
            }

            //cout << "CORNERS" << endl;
            std::vector<Point2d> imagep;
            for(auto &j : corners) {
                imagep.push_back(j);
                //cout << j << endl;
            }

            // calculate pose
            solvePnP(objp, imagep, cameraMatrix, distanceCoeffs, rotation_values, translation_values);


            // print rotation and translation values
            cout << "R = " << endl << " "  << rotation_values << endl << endl;
            cout << "T = " << endl << " "  << translation_values << endl << endl;
            
        }

    
    }

    // terminate the video capture
	printf("Terminating\n");
    delete capdev;
    return 0;
}
    // find aruco markers
    //startWebcamMonitoring(cameraMatrix, distanceCoeffs, 0.099f);