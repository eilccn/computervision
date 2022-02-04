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

int alt_greyscale( cv::Mat &src, cv::Mat &dst) {
	dst.create(src.size(), src.type());	
	
        for (int i=0; i<src.rows; i++) {
          for (int j=0; j<src.cols; j++) {
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
	    dst.at<cv::Vec3b>(i,j)[c] = finalresult[c];	    
	    }
	  }
	}
	
	return 0;
}



// SOBEL X FILTER
int sobel3x3( cv::Mat &src, cv::Mat &dst) {

        dst.create(src.size(), CV_16SC3);

        cv:Mat temp_dst;
        temp_dst.create(src.size(), CV_16SC3);

        cv::Vec3i first_result = {0, 0, 0}; // initialize to zeros
        cv::Vec3i second_result = {0, 0, 0};
        cv::Vec3s finalresult;
        cv::Vec3s first_final;

        int c;
        int i, j;

        // applying filter [-1 0 1] 
        // loop over all rows except top and bottom
        for(i=1; i<src.rows-1; i++) {
          // loop over all rows except first and last
          for(j=1; j<src.cols-1; j++) {
            // apply the filter and write the result to temp destination image
            for (c=0;c<3;c++) { // loop over number of channels
              temp_dst.at<cv::Vec3s>(i, j)[c] = (src.at<cv::Vec3b>(i-1,j-1)[c] + 
	        src.at<cv::Vec3b>(i, j-1)[c]*2 + src.at<cv::Vec3b>(i+1, j-1)[c]) / 4;

            }
           }
          }

        // applying filter [1 2 1] 
        // loop over all rows except top and bottom
        for(i=1; i<src.rows-1; i++) {
          // loop over all rows except first and last
          for(j=1; j<src.cols-1; j++) {
            // apply the filter and write the result to a destination image
            for (c=0;c<3;c++) {
              dst.at<cv::Vec3s>(i,j)[c] = temp_dst.at<cv::Vec3s>(i-1, j-1)[c]*(-1) + 
		temp_dst.at<cv::Vec3s>(i-1, j)[c]*0 + temp_dst.at<cv::Vec3s>(i-1, j+1)[c];
            }
          }
        }
	dst.convertTo(dst, CV_8UC3);
        return 0;
}

/*
int sobel( cv::Mat &src, cv::Mat &dst ) {
	// output needs to be type 16S

	int c, y, x;
        int i, j;
	
	// allocate space for dst
        dst.create(src.size(), CV_16SC3);

	// create intermediate cv::Mat
        cv:Mat temp_dst;
        temp_dst.create(src.size(), CV_16SC3);

	// initialize results cv::Vec3b
	cv::Vec3b first_result;
	cv::Vec3b second_result;
	cv::Vec3b first_final;
        cv::Vec3b finalresult;

	// create filter [-1 0 1]
	cv::Mat<float> filter(1,3);
	filter << -1, 0, 1;		

	// set result to all zeros to start
        cv::Vec3i result = {0, 0, 0};
        int halfrow = filter.rows/2;
	int halfcol = filter.cols/2;
	float maxpos = 0.0;
	float maxneg = 0.0;

	// applying filter [-1 0 1] horizontally
	for(y=0; y<filter.rows; y++) {
	  for(x=0; x<filter.cols; x++) {
	    for(c=0;c<3;c++) {
	      //product of overlapping filter/src values
	      first_result[c] += filter.at<float>(y,x) * src.at<cv::Vec3b>(i - halfrow + y,
		j - halfcol + x)[c];
	    }

	    // calculate the normalization coefficient
	    if (filter.at<float>(y,x) >= 0.0) {
	      	maxpos += filter.at<float>(y,x);
	    }
	    else {
		maxneg -= filter.at<float>(y,x);
	    }
	  }
	}

	maxpos = maxpos > maxneg ? maxpos : maxneg;
	for(c=0; c<3; c++) {
	  first_result[c] += 255*maxneg; // set max negative value to 0
	  first_result[c] /= maxpos+maxneg; // sets the max range back to 255
	  first_final[c] = (signed short)first_result[c]; // explicit cast to final value
	  temp_dst.at<cv::Vec3b>(i, j)[c] = first_final[c];
	}	

	// applying filter [1 2 1] vertically
	for(y=0; y<filter.rows; y++) {
          for(x=0; x<filter.cols; x++) {
            for(c=0;c<3;c++) {
              //product of overlapping filter/first_result values
              second_result[c] += filter.at<float>(y,x) * first_result.at<cv::Vec3b>(i - halfrow + y,
                j - halfcol + x)[c];
            }

            // calculate the normalization coefficient
            if (filter.at<float>(y,x) >= 0.0) {
                maxpos += filter.at<float>(y,x);
            }
            else {
                maxneg -= filter.at<float>(y,x);
            }
          }
        }

        maxpos = maxpos > maxneg ? maxpos : maxneg;
        for(c=0; c<3; c++) {
          second_result[c] += 255*maxneg; // set max negative value to 0
          second_result[c] /= maxpos+maxneg; // sets the max range back to 255
          finalresult[c] = (signed short)second_result[c]; // explicit cast to final value
          dst.at<cv::Vec3b>(i, j)[c] = finalresult[c];
	}

       return 0; 
}

// SOBEL Y FILTER
cv::Mat sobelY3x3( cv::Mat &src, cv::Mat &dst ) {
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

// GRADIENT FILTER
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ) {

}

// BLUR QUANTIZE FILTER
int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels ) {

}

// CARTOON FILTER
int cartoon( cv::Mat &src, cv::Mat &dst, int levels, int magThreshold ) {

}

// INVERT FILTER (CHOICE)
int invert( cv::Mat &src, cv::Mat &dst) {

}

*/
