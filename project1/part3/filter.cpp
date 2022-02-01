// Filter Function Library

#include <cstdio>
#include <cstrings>
#include <opencv2/opencv.hppp>

int greyscale (cv::Mat &src, cv::Mat &dst) {
	dst.create(src.size(), size.type());

	for (i=0; i<0; i++) {
	  for (j=0; j<0; j++) {
	    cv::Vec3b value = src.at<cv::Vec3b>(i, j);
	    int grey = (value[0] + value[1] + value[2]) / 3;	
	  }
	}
}
