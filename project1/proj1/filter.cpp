#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "filter.h"

using namespace cv;
using namespace std;

// Functions

cv::Mat alt_greyscale( cv::Mat &src, cv::Mat &dst) {
	dst.create(src.size(), CV_8UC1);	

        for (int i = 0; i < src.rows; i++) {
          for (int j = 1; j < src.cols; j++) {
            dst.at<cv::Vec3b>(i, j) = ((src.at<cv::Vec3b>(i, j)[0] + src.at<cv::Vec3b>(i, j)[1] + src.at<cv::Vec3b>(i, j)[2]) / 3);
          }
	}    
	return dst;
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


/*
int blur(cv::Mat &src, cv::Mat &dst) 
{
		
}

int sobelX3x3( cv::Mat &src, cv::Mat &dst ) {

}

int sobelY3x3( cv::Mat &src, cv::Mat &dst ) {

}

int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ) {

}

int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels ) {

}

int cartoon( cv::Mat &src, cv::Mat &dst, int levels, int magThreshold ) {

}

int invert( cv::Mat &src, cv::Mat &dst) {

}

*/
