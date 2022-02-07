#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
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


// Filter Functions
int greyscale( cv::Mat &src, cv::Mat &dst) {
	dst.create(src.size(), src.type());	

	// loop over all rows	
        for (int i=0; i<src.rows; i++) {
	  // loop over all cols
          for (int j=0; j<src.cols; j++) {
	    // apply filter and write result to destination
            dst.at<cv::Vec3b>(i, j)[0] = ((src.at<cv::Vec3b>(i, j)[0] + src.at<cv::Vec3b>(i, j)[1] + 
	      src.at<cv::Vec3b>(i, j)[2]) / 3);
	  
	    dst.at<cv::Vec3b>(i, j)[1] = (src.at<cv::Vec3b>(i, j)[0] + src.at<cv::Vec3b>(i, j)[1] +
              src.at<cv::Vec3b>(i, j)[2]) / 3;

	    dst.at<cv::Vec3b>(i, j)[2] = (src.at<cv::Vec3b>(i, j)[0] + src.at<cv::Vec3b>(i, j)[1] +
              src.at<cv::Vec3b>(i, j)[2]) / 3;
          }
	}
	
    	
	return 0;
}


// BLUR FILTER
int blur5x5(cv::Mat &src, cv::Mat &dst) 
{
	dst.create(src.size(), src.type());

	// create temp destination	
	cv:Mat temp_dst;
	temp_dst.create(src.size(), src.type());
	
	int c, i, j;

	// applying filter [1 2 4 2 1] horizontally
	// loop over all rows except top and bottom
	for(i=1; i<src.rows-1; i++) {
	  // loop over all cols except first and last
	  for(j=1; j<src.cols-1; j++) {
	    // apply the filter and write the result to temp destination image
	    for (c=0;c<3;c++) { // loop over number of channels
	      temp_dst.at<cv::Vec3b>(i, j)[c] = (src.at<cv::Vec3b>(i-1,j-1)[c] + 
	        src.at<cv::Vec3b>(i-1, j)[c]*2 + src.at<cv::Vec3b>(i-1, j+1)[c]*4 + 
	        src.at<cv::Vec3b>(i-1, j+2)[c]*2 + src.at<cv::Vec3b>(i-1, j+3)[c]) / 10;
	    }
	  }
	}
	
	// applying filter [1 2 4 2 1] vertically
        // loop over all rows except top and bottom
        for(i=1; i<src.rows-1; i++) {
          // loop over all cols except first and last
          for(j=1; j<src.cols-1; j++) {
	    // apply the filter and write the result to a destination image
            for (c=0;c<3;c++) {
              dst.at<cv::Vec3b>(i,j)[c] = (temp_dst.at<cv::Vec3b>(i-1, j-1)[c] + 
	        temp_dst.at<cv::Vec3b>(i, j-1)[c]*2 + temp_dst.at<cv::Vec3b>(i+1, j-1)[c]*4 + 
		temp_dst.at<cv::Vec3b>(i+2, j-1)[c]*2 + temp_dst.at<cv::Vec3b>(i+3, j-1)[c]) / 10;
	    }
	  }
	}
	
	return 0;
}



// SOBEL X FILTER
int sobelX3x3( cv::Mat &src, cv::Mat &dst) {
	// create destination output
        dst.create(src.size(), CV_16SC3);

	// create temp destination
        cv:Mat temp_dst;
        temp_dst.create(src.size(), CV_16SC3);

        int c;
        int i, j;

        // applying filter [1 2 1] 
        // loop over all rows except top and bottom
        for(i=1; i<src.rows-1; i++) {
          // loop over all cols except first and last
          for(j=1; j<src.cols-1; j++) {
            // apply the filter and write the result to temp destination image
            for (c=0;c<3;c++) { // loop over number of channels
              temp_dst.at<cv::Vec3s>(i, j)[c] = (src.at<cv::Vec3b>(i-1,j-1)[c] + 
	        src.at<cv::Vec3b>(i, j-1)[c]*2 + src.at<cv::Vec3b>(i+1, j-1)[c]) / 4;

            }
          }
        }

        // applying filter [-1 0 1] 
        // loop over all rows except top and bottom
        for(i=1; i<src.rows-1; i++) {
          // loop over all cols except first and last
          for(j=1; j<src.cols-1; j++) {
            // apply the filter and write the result to a destination image
            for (c=0;c<3;c++) {
              dst.at<cv::Vec3s>(i,j)[c] = temp_dst.at<cv::Vec3s>(i-1, j-1)[c]*(-1) + 
		temp_dst.at<cv::Vec3s>(i-1, j)[c]*0 + temp_dst.at<cv::Vec3s>(i-1, j+1)[c];
            }
          }
        }
	// convert destinatinon to CV_8UC3
	dst.convertTo(dst, CV_8UC3);
        return 0;
}


// SOBEL Y FILTER
int sobelY3x3( cv::Mat &src, cv::Mat &dst ) {
	// create destination output
        dst.create(src.size(), CV_16SC3);

	// create temp destination 
        cv:Mat temp_dst;
        temp_dst.create(src.size(), CV_16SC3);

        int c;
        int i, j;

        // applying filter [-1 0 1]
        // loop over all rows except top and bottom
        for(i=1; i<src.rows-1; i++) {
          // loop over all cols except first and last
          for(j=1; j<src.cols-1; j++) {
            // apply the filter and write the result to temp destination image
            for (c=0;c<3;c++) { // loop over number of channels
              temp_dst.at<cv::Vec3s>(i, j)[c] = (src.at<cv::Vec3b>(i-1,j-1)[c]*(-1) +
                src.at<cv::Vec3b>(i, j-1)[c]*0 + src.at<cv::Vec3b>(i+1, j-1)[c]);
            }
          }
        }

        // applying filter [1 2 1]
        // loop over all rows except top and bottom
        for(i=1; i<src.rows-1; i++) {
          // loop over all cols except first and last
          for(j=1; j<src.cols-1; j++) {
            // apply the filter and write the result to a destination image
            for (c=0;c<3;c++) {
              dst.at<cv::Vec3s>(i,j)[c] = (temp_dst.at<cv::Vec3s>(i-1, j-1)[c] +
                temp_dst.at<cv::Vec3s>(i-1, j)[c]*2 + temp_dst.at<cv::Vec3s>(i-1, j+1)[c]) / 4;
            }
          }
        }
	 // convert destination to CV_8UC3
         dst.convertTo(dst, CV_8UC3);
         return 0;
}


// GRADIENT
int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ) {
	dst.create(sx.size(), sx.type());

	int i, j, c;
	
	// loop over all rows except top and bottom
        for(i=1; i<sx.rows-1; i++) {
          // loop over all cols except first and last
          for(j=1; j<sx.cols-1; j++) {
            // apply the filter and write the result to a destination image
            for (c=0;c<3;c++) {
	      dst.at<cv::Vec3s>(i,j)[c] = cv::sqrt( cv::pow(sx.at<cv::Vec3s>(i,j)[c], 2.0) + 
		cv::pow(sy.at<cv::Vec3s>(i,j)[c], 2.0) ); 
            }
          }
        }
	// convert destination to CV_8UC3
	dst.convertTo(dst, CV_8UC3);
	return 0;	
}


// BLUR QUANTIZE FILTER
int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels ) {
	dst.create(src.size(), src.type());
	
	// apply blur filter	
	blur5x5(src, dst);
	
	// initialize variables
	int i, j, c;	
	int b = 255 / levels;

	//loop over all rows
        for(i=0; i<dst.rows; i++) {
          // loop over all colss
          for(j=0; j<dst.cols; j++) {
            // apply the filter and write the result to a destination image
	    for(c=0; c<3; c++) {
              dst.at<cv::Vec3b>(i, j)[c] = dst.at<cv::Vec3b>(i, j)[c] / b * b + b / 2; 
	    }
	  }
        }
	return 0;
}


// CARTOON FILTER
int cartoon( cv::Mat &src, cv::Mat &dst, int levels, int magThreshold ) {
        dst.create(src.size(), src.type());

        int i, j, c;

	// apply gradient magnitude filter
	cv::Mat mag;
	mag.create(src.size(), src.type());

        cv::Mat sx;
        sx.create(src.size(), src.type());
        sobelX3x3(src, sx);
        sx = src;

        cv::Mat sy;
        sy.create(src.size(), src.type());
        sobelY3x3(src, sy);
        sy = src;

	magnitude(sx, sy, mag);
	mag = src;

	// apply blur and quantize filter
	blurQuantize(mag, dst, levels);

        // loop over all rows except top and bottom
        for(i=1; i<dst.rows-1; i++) {
          // loop over all cols except first and last
          for(j=1; j<dst.cols-1; j++) {
            // apply the filter and write the result to a destination image
            for (c=0;c<3;c++) {
	      if (mag.at<cv::Vec3b>(i, j)[c] < magThreshold) {
	        dst.at<cv::Vec3b>(i,j)[c] = 0;
	      }
            }
          }
        }

	return 0;
}


// INVERT FILTER (CHOICE)
int invert(cv::Mat &src, cv::Mat &dst) {
        dst.create(src.size(), src.type());

        int i, j, c;
	
	// loop over all rows
        for(i=1; i<dst.rows-1; i++) {
          // loop over all cols
          for(j=1; j<dst.cols-1; j++) {
            // apply the filter and write the result to a destination image
            for (c=0;c<3;c++) {
              dst.at<cv::Vec3b>(i,j)[c] = ( dst.at<cv::Vec3b>(i,j)[0] + dst.at<cv::Vec3b>(i,j)[1] +
		dst.at<cv::Vec3b>(i,j)[2] ) / -3;
            }
          }
        }

        return 0;
}

