/*
  Eileen Chang
  
  Functions for calculating feature sets
*/

#include "filters.h"
#include "features.h"
#include "csv_util.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// compute 9x9 center image baseline feature set
int process_baseline(cv::Mat &img, std::vector<float> &fvec) {
    
    // obtain center 9x9 square of the image
    cv::Mat center;
    const int cropSize = 9;
    const int offsetW = (img.cols - cropSize) / 2;
    const int offsetH = (img.rows - cropSize) / 2;
    const Rect cropped(offsetW, offsetH, cropSize, cropSize);
    center = img(cropped).clone();


    // store features to feature vector
    for(int i=0; i<center.rows; i++) {
        for(int j=0; j<center.cols; j++) {
            for(int c=0; c<3; c++) {
                
                // push values into feature vector      
                float value = center.at<Vec3b>(i,j)[c];
                fvec.push_back(value);

            }
        }
    }

    /*
    // Print out the vector
    std::cout << "v = { ";

    for (auto n : fvec) {
        std::cout << n << ", ";
    }
    std::cout << "}; \n";
    */
    
    return 0;
}

// compute rg histogram feature set
int histogram(cv::Mat &img, std::vector<float> &fvec) {
    const int Hsize = 32;
    int dim[2] = {Hsize, Hsize};
    cv::Mat hist2d;
    hist2d = cv::Mat::zeros(2, dim, CV_32S);
    int i, j, rx, ry;
    int num_bins = 16;
    float r, g, b;

    for(i=0; i<img.rows; i++) {
        for(j=0; j<img.cols; j++) {
            r = img.at<cv::Vec3b>(i, j)[0];
            g = img.at<cv::Vec3b>(i, j)[1];
            b = img.at<cv::Vec3b>(i, j)[2];
            rx = num_bins * r / (r + b + g + 1);
            ry = num_bins * g / (r + b + g + 1);
            
            hist2d.at<int>(rx, ry)++;
        }

    }

    for(i=0; i<hist2d.rows; i++) {
        for(j=0; j<hist2d.cols; j++) {
            
            fvec.push_back(hist2d.at<int>(i,j));
        }
    }

    return 0;
}

    /*
    // Print out the vector
    std::cout << "v = { ";

    for (auto n : fvec) {
        std::cout << std::fixed << n << ", ";
    }
    std::cout << "}; \n";

    return 0;
    
}
*/





/*
// compute texture and color feature set 
int(cv::Mat &img, std::vector<float> &fvec) {
    int i, j, c;

    cv::Mat sx;
	sx.create(img.size(), CV_16SC3);
	sobelX3x3(img, sx);

	cv::Mat sy;
	sy.create(img.size(), CV_16SC3);
	sobelY3x3(img, sy);

	cv::Mat mag_img;
    cv::Mat converted_img;
	mag_img.create(img.size(), img.type());
	magnitude(sx, sy, mag_img);
	convertScaleAbs(mag_img, converted_img);

    // convert gradient magnitude image to greyscale
    cv::Mat gray_img;
    img.copyTo(gray_img);
    cv::cvtColor(converted_img, gray_img, cv::COLOR_BGR2GRAY);

    for(i=0; i<gray_img.rows; i++) {
        for(j=0; j<gray_img.cols; j++) {
            magnitude = (int)gray_img.at<uchar>(i,j)[0];
            bin = (int)magnitude / (256 / num_bins));
            hist1d bin += 1;
        }
    }

    return 0;
}
*/