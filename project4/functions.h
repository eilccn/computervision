#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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

int createArucoMarkers();
int createKnownBoardPosition(cv::Size boardSize, float squareEdgeLength, std::vector<Point3f> &points);
int getChessboardCorners(std::vector<Mat> images, vector<vector<Point2f> > &cornerList, bool showResults = false);
int cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, cv::Mat &cameraMatrix, cv::Mat &distanceCoeffs);
bool saveCameraCalibration(string name, cv::Mat cameraMatrix, cv::Mat distanceCoeffs);
int startWebcamMonitoring(cv::Mat &dst, cv::Mat &cameraMatrix, cv::Mat &distanceCoeffs, float arucoSquareDimensions);
bool loadCameraCalibration(string name, cv::Mat &cameraMatrix, cv::Mat distanceCoeffs);
int storeCameraConfig(cv::Mat &cameraMatrix, cv::Mat &distCoeffs);
int readCameraConfig(cv::Mat &cameraMatrix, cv::Mat &distCoeffs);
int draw_axes(std::vector<cv::Vec3f> &points, cv::Point3f origin, float scale);


#endif