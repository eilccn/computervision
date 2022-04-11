/*
Eileen Chang
CS5330 
Spring 2022

Functions file
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

using namespace cv;
using namespace std;

// chessboard parameters
cv::Size patternsize( 6, 9 ); // size of chessboard
const float calibrationSquareDimension = 0.01778f; // chessboard square in meters
const float arucoSquareDimension = 0.1016f; // aruco square in meters

/**
 * @brief Create Aruco Markers 
 * 
 * @return int 0
 */
int createArucoMarkers() {
    cv::Mat outputMarker;

    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

    for(int i=0; i<50; i++) {
        aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
        ostringstream convert;
        string imageName = "4x4Marker_";
        convert << imageName << i << ".jpg";
        imwrite(convert.str(), outputMarker);
    }
    return 0;
}


/**
 * @brief Create a Known Board Position object
 * 
 * @param boardSize 
 * @param squareEdgeLength 
 * @param points 
 * @return int 0
 */
int createKnownBoardPosition(cv::Size boardSize, float squareEdgeLength, std::vector<Point3f> &points) {
    /* define world coordinates for 3D points */
    for(int i=0; i<boardSize.width; i++) {
        for(int j=0; j<boardSize.height; j++) {
            points.push_back(cv::Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
        }
    }
    //pointList.push_back(std::vector<cv::Point3f>( points ) );
    return 0;
}



/**
 * @brief Find and draw the chessboard corners for a vector of saved images
 * 
 * @param images 
 * @param cornerList 
 * @param showResults 
 * @return int 0
 */
int getChessboardCorners(std::vector<Mat> images, vector<vector<Point2f> > &cornerList, bool showResults = false) {

    for(std::vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++) {
        /* refine pixel coordinates of 2D points for more accurracy */
        std::vector<Point2f> corners;
        cv::Mat gray;
        cvtColor(*iter, gray, COLOR_BGR2GRAY);
        bool found = findChessboardCorners(*iter, patternsize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            /* refine pixel coordinates of 2D points for more accurracy */
            cv::TermCriteria criteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 30, 0.001);
            cv::cornerSubPix(gray, corners, cv::Size(11,11), cv::Size(-1,-1), criteria);

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


/**
 * @brief Calibrate the camera
 * 
 * @param calibrationImages 
 * @param boardSize 
 * @param squareEdgeLength 
 * @param cameraMatrix 
 * @param distanceCoeffs 
 * @return int 0
 */
int cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, cv::Mat& cameraMatrix, cv::Mat& distanceCoeffs) {
    std::vector<vector<Point2f> > cornerList;
    getChessboardCorners(calibrationImages, cornerList, false);

    std::vector<vector<Point3f> > pointList(1);
    //std::vector<vector<Point3f> > pointList;

    createKnownBoardPosition(boardSize, squareEdgeLength, pointList[0]);
    pointList.resize(cornerList.size(), pointList[0]);

    vector<Mat> rVectors, tVectors;
    distanceCoeffs = cv::Mat::zeros(8,1,CV_64FC1);

    double rms = calibrateCamera(pointList, cornerList, boardSize, cameraMatrix, distanceCoeffs, rVectors, tVectors, CALIB_FIX_ASPECT_RATIO /*| CALIB_FIX_K4 | CALIB_FIX_K5*/);
    cout << "cameraMatrix: " << cameraMatrix << endl;
    cout << "distCoeffs: " << distanceCoeffs << endl;
    cout << "Rotation vector : " <<  std::fixed << &rVectors << endl;
    cout << "Translation Vector : " << std::fixed << &tVectors << endl;
    cout << "Reprojection error: " << rms << endl;
    return 0;
}



/**
 * @brief Save calibration matrix and distance coefficients to an outstream file 
 * 
 * @param name of file
 * @param cameraMatrix 
 * @param distanceCoeffs 
 * @return true 
 * @return false 
 */
bool saveCameraCalibration(string name, cv::Mat cameraMatrix, cv::Mat distanceCoeffs) {
    ofstream outStream(name);
    if(outStream) {
        uint16_t rows = cameraMatrix.rows;
        uint16_t columns = cameraMatrix.cols;

        outStream << rows << endl;
        outStream << columns << endl;

        for(int r=0; r<rows; r++) {
            for (int c=0; c<columns; c++) { 
                double value = cameraMatrix.at<double>(r,c);
                outStream << value << endl;

            }
        }


        rows = distanceCoeffs.rows;
        columns = distanceCoeffs.cols;

        outStream << rows << endl;
        outStream << columns << endl;

        for(int r=0; r<rows; r++) {
            for (int c=0; c<columns; c++) { 
                double value = distanceCoeffs.at<double>(r,c);
                outStream << value << endl;

            }
        }

        outStream.close();
        return true;
    }
    return false;
}



/**
 * @brief Detect aruco markers
 * 
 * @param dst
 * @param cameraMatrix 
 * @param distanceCoeffs 
 * @param arucoSquareDimensions 
 * @return int 
 */
int startWebcamMonitoring(cv::Mat &dst, cv::Mat &cameraMatrix, cv::Mat &distanceCoeffs, float arucoSquareDimensions) {

    vector<int> markerIds;
    vector<vector<Point2f> > markerCorners, rejectedCandidates;
    aruco::DetectorParameters parameters;

    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

    vector<Vec3d> rotationVectors, translationVectors;

    aruco::detectMarkers(dst, markerDictionary, markerCorners, markerIds);
    aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoeffs, rotationVectors, translationVectors);

    for(int i=0; i<markerIds.size(); i++) {
        aruco::drawAxis(dst, cameraMatrix, distanceCoeffs, rotationVectors[i], translationVectors[i], 0.1f);
    }

    return 0;
}



/**
 * @brief Load saved camera calibration data for later use
 * 
 * @param name 
 * @param cameraMatrix 
 * @param distanceCoeffs 
 * @return true 
 * @return false 
 */
bool loadCameraCalibration(string name, cv::Mat &cameraMatrix, cv::Mat distanceCoeffs) {
    ifstream inStream(name);
    if(inStream) {
        uint16_t rows;
        uint16_t columns;

        inStream >> rows;
        inStream >> columns;

        cameraMatrix = cv::Mat(Size(columns, rows), CV_64F);

        for (int r=0; r<rows; r++) {
            for (int c=0; c<columns; c++) {
                double read = 0.0f;
                inStream >> read;
                cameraMatrix.at<double>(r,c) = read;
                cout << cameraMatrix.at<double>(r,c) << "\n";
            }
        }

        // distance coefficients
        inStream >> rows;
        inStream >> columns;

        distanceCoeffs = cv::Mat::zeros(rows, columns, CV_64F);

        for (int r=0; r<rows; r++) {
            for (int c=0; c<columns; c++) {
                double read = 0.0f;
                inStream >> read;
                distanceCoeffs.at<double>(r,c) = read;
                cout << distanceCoeffs.at<double>(r,c) << "\n";

            }
        }
        inStream.close();
        return true;

    }
    return false;
}

/**
 * @brief Save camera calibration matrix and distance coefficients to a yaml file 
 * 
 * @param cameraMatrix 
 * @param distCoeffs 
 * @return int 
 */
int storeCameraConfig(cv::Mat &cameraMatrix, cv::Mat &distCoeffs) {
    cv::FileStorage fs("CameraConfig.yaml", cv::FileStorage::WRITE);
    fs << "intriMat" << cameraMatrix;
    fs << "distCoeff" << distCoeffs;
    fs.release();
    return 0;
}



/**
 * @brief Read camera calibration matrix and distance coefficients from a yaml file
 * 
 * @param cameraMatrix 
 * @param distCoeffs 
 * @return int 
 */
int readCameraConfig(cv::Mat &cameraMatrix, cv::Mat &distCoeffs){
    cv::FileStorage fs("CameraConfig.yaml", cv::FileStorage::READ);
    fs ["intriMat"] >> cameraMatrix;
    fs ["distCoeff"] >> distCoeffs;
    std::cout << "CameraMatrix" << cameraMatrix << std::endl;
    std::cout << "distCoeff" << distCoeffs << std::endl;
    fs.release();
    return 0;
}



/**
 * @brief Function to draw a cube 
 * 
 * @param points vector of world points to write to
 * @param origin origin of the cube
 * @param scale scale of the cube
 * @param color color of the cube
 * @return int 
 */
int draw_axes(std::vector<cv::Vec3f> &points, cv::Point3f origin, float scale){
  points.push_back(cv::Point3f(0, 0, 0)); // origin 
  points.push_back(cv::Point3f(0, 0, 1)); // z axis
  points.push_back(cv::Point3f(1, 0, 0)); // x axis
  points.push_back(cv::Point3f(0, 1, 0)); // y axis
  return 0; 
}





