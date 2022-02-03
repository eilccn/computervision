#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "filter.h"

using namespace cv;
using namespace std;

// Template
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

// Functions

cv::Mat alt_greyscale( cv::Mat &src, cv::Mat &dst) {
	dst.create(src.size(), src.type());	

        for (int i=0; i<src.rows; i++) {
          for (int j=1; j<src.cols; j++) {
            dst.at<cv::Vec3b>(i, j) = ((src.at<cv::Vec3b>(i, j)[0] + src.at<cv::Vec3b>(i, j)[1] + src.at<cv::Vec3b>(i, j)[2]) / 3);
          }
	}    
	return dst;
}


cv::Mat blur5x5(cv::Mat &src, cv::Mat &dst) 
{
	dst.create(src.size(), src.type());
	
	cv:Mat temp_dst;
	temp_dst.create(src.size(), src.type());
	
	cv::Vec3i first_result = {0, 0, 0, 0, 0}; // initialize to zeros
	cv::Vec3i second_result = {0, 0, 0, 0, 0};
	cv::Vec3b finalresult;
	cv::Vec3b first_final;
	
	int c;
	int i, j;

	// applying filter [1 2 4 2 1] horizontally
	// loop over all rows except top and bottom
	for(i=1; i<src.rows-1; i++) {
	  // loop over all rows except first and last
	  for(j=1; j<src.cols-1; j++) {
	    // apply the filter and write the result to temp destination image
	    for (c=0;c<3;c++) { // loop over number of channels
	      first_result[c] = src.at<cv::Vec3b>(i-1,j-1)[c] + src.at<cv::Vec3b>(i-1, j)[c]*2 + 
	      src.at<cv::Vec3b>(i-1, j+1)[c]*4 + src.at<cv::Vec3b>(i-1, j+2)[c]*2 +
	      src.at<cv::Vec3b>(i-1, j+3)[c];
	    first_result[c] /= 10; // sum of the filter coefficients
	    first_final[c] = (unsigned char)first_result[c];
	    temp_dst.at<cv::Vec3b>(i, j)[c] = first_final[c];
	    }
	   }
	  }
	
	// applying filter [1 2 4 2 1] vertically
        // loop over all rows except top and bottom
        for(i=1; i<src.rows-1; i++) {
          // loop over all rows except first and last
          for(j=1; j<src.cols-1; j++) {
	    // apply the filter and write the result to a destination image
            for (c=0;c<3;c++) {
              second_result[c] = temp_dst.at<cv::Vec3b>(i-1, j-1)[c] + temp_dst.at<cv::Vec3b>(i, j-1)[c]*2 +
              temp_dst.at<cv::Vec3b>(i+1, j-1)[c]*4 + temp_dst.at<cv::Vec3b>(i+2, j-1)[c]*2 +
              temp_dst.at<cv::Vec3b>(i+3, j-1)[c];
            second_result[c] /= 10; // sum of filter coefficients
            finalresult[c] = (unsigned char)second_result[c];
	    dst.at<cv::Vec3b>(i,j)[c] = second_result[c];	    
	    }
	  }
	}
	
	return dst;
}
/*
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
