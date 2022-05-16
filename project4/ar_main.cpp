/*
Eileen Chang
CS5330 
Spring 2022

Main file for Calibration (Tasks 1-4) and AR functionalities (Tasks 4-7)
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

enum Filter {
	CORNERS, AXES, OBJECT, ROBUST, ARUCO
};

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
    int framesPerSecond = 20;
    cv::Mat frame;
    cv::Mat convertedImage;
    convertedImage = frame;
    cv::namedWindow("Main Window", cv::WINDOW_AUTOSIZE);

    // chessboard parameters
    const float calibrationSquareDimension = 0.01778f; // meters
    const float arucoSquareDimension = 0.1016f; // meters
    cv::Size patternsize( 9, 6 );   

    // camera matrix
    double col = double(convertedImage.cols / 2);
    double rows = double(convertedImage.rows / 2);
    double data[3][3] = { {1, 0, col}, {0, 1, rows}, {0, 0, 1} };
    cv::Mat cameraMatrix = cv::Mat(3,3, CV_64FC1, &data);
    // distortion coefficients
    cv::Mat distCoeffs = cv::Mat::zeros(8, 1, CV_64FC1);
    // extract paths of individual images 
    std::vector<Mat> savedImages; 
    // image points found by findChessboardCorners
    std::vector<cv::Point2f> corners; 
    // vector of vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f> > cornerList, rejectedCandidates;
    // 3D world points, constructed
    std::vector<cv::Point3f> points; 
    // vector of vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Point3f> > pointList(1); 
    // solvePnP rvecs and tvecs
    cv::Mat rotation_values = cv::Mat::zeros(3, 1, CV_64FC1);
    cv::Mat translation_values = cv::Mat::zeros(3, 1, CV_64FC1);
    // create undistorted corners or image points
    std::vector<cv::Point2f> imagePoints;
    // initialize vector for projectPoints 3D cube image points output
    std::vector<Point2f> cubeProjectedPoints; 
    // initialize vector for projectPoints 3D axes image points output
    std::vector<Point2f> axesProjectedPoints; 
    // bool for solvePnP 
    bool flag = false;


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

    // create instance of filterState to keep track of state machine states	
	Filter filterState = Filter::CORNERS;

    /* loop for various functions */
    for(;!quit;) {

        *capdev >> frame; 

        if(frame.empty()) {
            printf("frame is empty\n");
            break;
        }
        
        /* detect and extract chessboard corners */
        bool found = false;
        cv::Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        found = findChessboardCorners(gray, patternsize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);


        /**
         * @brief if-else ladder for:
         * CORNERS: finding, drawing, and pushing corners + pushing 3D world points
         * AXES: drawing a 3D axes
         * OBJECT: drawing a 3D object
         * ROBUST: harris corner detection
         * ARUCO: detect aruco markers
         */

        // CORNERS
        if (filterState == CORNERS) {
            if(found) {
                cornerList.clear();
                pointList.clear();
                points.clear();

                /* refine pixel coordinates of 2D points for more accurracy */
                cv::TermCriteria criteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 30, 0.001);
                cv::cornerSubPix(gray, corners, cv::Size(11,11), cv::Size(-1,-1), criteria);

                /* draw to chessboard */
                frame.copyTo(convertedImage);
                drawChessboardCorners(convertedImage, patternsize, corners, found);
                //imshow("Main Window", convertedImage);

                // push corners
                cornerList.push_back(corners);

                // push points
                /* define world coordinates for 3D points */
                for(int i=0; i<patternsize.width; i++) {
                    for(int j=0; j<patternsize.height; j++) {
                        points.push_back(cv::Point3f(j * calibrationSquareDimension, i * calibrationSquareDimension, 0.0f));
                    }
                }
                
                
            }
            else {
                convertedImage = frame;
            }
        // AXES
        }
        else if (filterState == AXES) {
            if (found) {
                axesProjectedPoints.clear();

                // draw axes 
                printf("Drawing (x,y,z) axes ...\n");
                frame.copyTo(convertedImage);

                // read calibration data
                readCameraConfig(cameraMatrix, distCoeffs);

                // draw corners
                drawChessboardCorners(convertedImage, patternsize, corners, found);

                // create undistorted corners or image points
                undistortPoints(corners, imagePoints, cameraMatrix, distCoeffs);

                //cout << "POINTS" << endl;
                std::vector<Point3d> objp;
                for(auto &i : points) {
                    objp.push_back(i);
                    //cout << i << endl;
                }

                //cout << "CORNERS" << endl;
                std::vector<Point2d> imagep;
                for(auto &j : imagePoints) {
                    imagep.push_back(j);
                    //cout << j << endl;
                }
                
                
                // draw 3D axes using projectPoints
                std::vector<Point3f> axis;
                axis.push_back(cv::Point3f(0.02, 0, 0));
                axis.push_back(cv::Point3f(0, 0.02, 0));
                axis.push_back(cv::Point3f(0, 0, -0.02));
                

                // solvePnP rvecs and tvecs
                cv::Mat rvec; 
                cv::Mat tvec; 

                // solvePnP for finding rotation and translation values
                solvePnP(objp, imagep, cameraMatrix, distCoeffs, rvec, tvec);
                
                // projectPoints to find 2D world image points
                projectPoints(axis, rvec, tvec, cameraMatrix, distCoeffs, axesProjectedPoints);

                cout << "image points" << endl;
                for (auto &n : axesProjectedPoints) {
                    cout << n << endl;
                }

                // draw lines from origin corner to projected image points
                cv::line(convertedImage, corners[0], axesProjectedPoints[0], {255,0,0}, 5);
                cv::line(convertedImage, corners[0], axesProjectedPoints[1], {0,255,0}, 5);
                cv::line(convertedImage, corners[0], axesProjectedPoints[2], {0,0,255}, 5);
                
                cout << "corner 8" << endl;
                cout << corners[8] << endl;
                cout << axesProjectedPoints[0] << endl;
                

                // draw 3D axes using cv::drawFrameAxes
                //cv::drawFrameAxes(convertedImage, cameraMatrix, distCoeffs, rvec, tvec, 0.2f, 3);

                
                // highlight corners 
                cv::circle(convertedImage, corners[0], 30, {0, 255, 0}, FILLED, 5);
                cv::circle(convertedImage, corners[8], 30, {0, 255, 0}, FILLED, 5);
                cv::circle(convertedImage, corners[45], 30, {0, 255, 0}, FILLED, 5);
                cv::circle(convertedImage, corners[53], 30, {0, 255, 0}, FILLED, 5);
                
            }
            else {
                filterState = CORNERS;
            }
        // 3D OBJECT
        }
        else if (filterState == OBJECT) {
            if (found) {
                // draw a 3D cube
                printf("Drawing cube ...\n");
                frame.copyTo(convertedImage);

                // read calibration data
                readCameraConfig(cameraMatrix, distCoeffs);

                // draw corners
                drawChessboardCorners(convertedImage, patternsize, corners, found);

                // axis
                std::vector<Point3f> cube;
                cube.push_back(Point3f(0,0,0));
                cube.push_back(Point3f(0,0.02,0));
                cube.push_back(Point3f(0.02,0.02,0));
                cube.push_back(Point3f(0.02,0,0));
                cube.push_back(Point3f(-0.02,0,0));
                cube.push_back(Point3f(0,0.02,-0.02));
                cube.push_back(Point3f(0.02,0.02,-0.02));
                cube.push_back(Point3f(0.02,0,-0.02));
                

                // create undistorted corners or image points
                undistortPoints(corners, imagePoints, cameraMatrix, distCoeffs);

                //cout << "POINTS" << endl;
                std::vector<Point3d> objp;
                for(auto &i : points) {
                    objp.push_back(i);
                    //cout << i << endl;
                }

                //cout << "CORNERS" << endl;
                std::vector<Point2d> imagep;
                for(auto &j : imagePoints) {
                    imagep.push_back(j);
                    //cout << j << endl;
                }
                // solvePnP rvecs and tvecs
                cv::Mat rvec; 
                cv::Mat tvec; 

                // solvePnP for finding rotation and translation values
                solvePnP(objp, imagep, cameraMatrix, distCoeffs, rvec, tvec);

                projectPoints(cube, rvec, tvec, cameraMatrix, distCoeffs, cubeProjectedPoints);

                cv::drawContours(convertedImage, cubeProjectedPoints, -1, {0,255,0}, 3);
            
            }
            else {
                filterState = CORNERS;
            }
        // ROBUST
        }
        // harris corner detection
        else if (filterState == ROBUST) {
            int thresh = 200;
            int max_thresh = 255;
            int blockSize = 2;
            int apertureSize = 3;
            double k = 0.04;

            //cv::Mat temp = Mat::zeros( frame.size(), CV_32FC1 );
            Mat temp = Mat::zeros( frame.size(), CV_32FC1 );
            cornerHarris( gray, temp, blockSize, apertureSize, k );
            
            Mat dst_norm;
            normalize( temp, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
            //convertScaleAbs( dst_norm, convertedImage );

            for( int i = 0; i < dst_norm.rows ; i++ )
            {
                for( int j = 0; j < dst_norm.cols; j++ )
                {
                    if( (int) dst_norm.at<float>(i,j) > thresh )
                    {
                        circle( convertedImage, Point(j,i), 5, {255, 105, 235}, 10, 8, 0 );
                    }
                }
            }
        }
        // ARUCO
        else if (filterState == ARUCO) {
            // read saved calibration data 
            readCameraConfig(cameraMatrix, distCoeffs);

            // detect aruco markers
            startWebcamMonitoring(convertedImage, cameraMatrix, distCoeffs, calibrationSquareDimension);
        }
                

        // display video in its designated state (via keypress)
        imshow("Main Window", convertedImage);

        // wait for keypress
        int key = waitKey(1);
        //char key = waitKey(1000 / framesPerSecond);

        // switch case
        if (key == 's') {
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
            else {
                filterState = CORNERS;
            }
        }
        else if (key == 'q') {
            // exit program
            quit=1;
        }
        else if (key == ' ') {
            // turn flag on for solvePnP (see code proceeding this if-else ladder)
            flag = true;
        }
        else if (key == 'f') {
            // turn flag off for solvePnP (see code proceeding this if-else ladder)
            flag = false;
        }
        else if (key == 'c') {
            // camera calibration
            printf("Calibrating ...\n");
            
            if(savedImages.size() > 5) { // require more than 5 images to be saved
                // camera calibration function
                cameraCalibration(savedImages, patternsize, calibrationSquareDimension, cameraMatrix, distCoeffs);

                // save data from calibration to file
                storeCameraConfig(cameraMatrix, distCoeffs);
            }
            else {
                printf("Not enough images saved for calibration");
                return -1;
            }
            
        }
        else if (key == 'x') {
            // draw corners
            filterState = CORNERS; 
        }
        else if (key == 'd') {
            // draw axes
            filterState = AXES;
        }
        else if (key == 'o') {
            // place 3D virtual object
            filterState = OBJECT;
        }
        else if (key == 'r') {
            // detect robust features
            filterState = ROBUST;
        }
        else if (key == 'a') {
            // detect aruco markers
            filterState = ARUCO;
        }
                

        /* calculate board's pose (rotation and translation) */
        if (flag) {
            printf("Calculating board's pose (rotation and translation) ...\n");
            // read calibration data
            readCameraConfig(cameraMatrix, distCoeffs);

            // create undistorted corners or image points
            undistortPoints(corners, imagePoints, cameraMatrix, distCoeffs);


            //cout << "POINTS" << endl;
            std::vector<Point3d> objp;
            for(auto &i : points) {
                objp.push_back(i);
                //cout << i << endl;
            }

            //cout << "CORNERS" << endl;
            std::vector<Point2d> imagep;
            for(auto &j : imagePoints) {
                imagep.push_back(j);
                //cout << j << endl;
            }

            cout << "point size" << endl;
            cout << objp.size() << endl;

            // calculate pose
            solvePnP(objp, cornerList[0], cameraMatrix, distCoeffs, rotation_values, translation_values, true, SOLVEPNP_ITERATIVE);


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
    //startWebcamMonitoring(cameraMatrix, distCoeffs, 0.099f);