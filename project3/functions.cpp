#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "functions.h"
#include "filter.h"

using namespace cv;
using namespace std;

/** THRESHOLDING ALGORITHM
 * separates object from background
**/
int threshold(cv::Mat &src, cv::Mat &dst) {

    // pre-process image by blurring as suggested in part 1
	cv::Mat blur_img;
	cv::blur(src, blur_img, Size(3, 3), Point(-1, -1));

    // create dst image with size same as src rgb image but with a single channel only
    dst.create(src.size(), CV_8UC1);

    // compute the thresholded image
    int i, j;
    int threshold = 127;

    for(i=0; i<src.rows; i++) {
        for(j=0; j<src.cols; j++) {
            if( ( (src.at<Vec3b>(i,j)[0] + src.at<Vec3b>(i,j)[1] + src.at<Vec3b>(i,j)[2]) / 3 ) > threshold)
				dst.at<uchar>(i,j)=0;	//Make pixel black
			else
				dst.at<uchar>(i,j)=255;	//Make pixel white
        }
    }

    return 0;
}

/** MORPHOLOGICAL FILTERING
 * cleans up the thresholded binary image
 * uses built-in opencv growing and shrinking fxns
**/
int morphological(cv::Mat &src, cv::Mat &dst) {

    // threshold the image
	cv::Mat threshold_img;
    threshold(src, threshold_img);

	// create structuring element
	int morph_size = 2;

	cv::Mat element = getStructuringElement(
        MORPH_RECT,
        Size(2 * morph_size + 1, 2 * morph_size + 1),
        Point(morph_size, morph_size));

	// apply closing morphological filtering
	cv::morphologyEx(threshold_img, dst, MORPH_CLOSE, element, Point(-1, -1), 2);

	// apply open morphological filtering
	cv::morphologyEx(threshold_img, dst, MORPH_OPEN, element, Point(-1, -1), 2);
    
    return 0;
}

/** CONNECTED COMPONENTS
 * 
**/
int conn_comp(cv::Mat &src, cv::Mat &dst) {

    // initialize cv::Mat variables for built-in conn comp fxn
    cv::Mat labels, stats, centroids;

    // create binary image
	cv::Mat binary;
	morphological(src, binary);

    // connected components computation
    cv::Mat labelImage(src.size(), CV_32S);
    int nLabels = cv::connectedComponentsWithStats(binary, labelImage, stats, centroids);
    //cout << "Number of connected components = " << nLabels << endl << endl;

    /**
    // print stats and centroids
    cout << "Show statistics and centroids:" << endl;
    cout << "stats:" << endl << "(left,top,width,height,area)" << endl << stats << endl << endl;
    cout << "centroids: " << endl << "(x, y)" << endl << centroids << endl << endl; 
    cout << "labels: " << endl << labels << std::endl;
	cout << "stats.size()=" << endl << stats.size() << std::endl;
    **/

	// assign different colors to each connected component region
	std::vector<Vec3b> colors(nLabels);
	colors[0] = Vec3b(0, 0, 0);//background
			
	for (int label = 1; label < nLabels; label++) {
		colors[label] = Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
	}
			
	dst.create(src.size(), CV_8UC3);
			
	for (int r = 0; r < dst.rows; ++r) {
				for (int c = 0; c < dst.cols; ++c) {
			int label = labelImage.at<int>(r, c);
			Vec3b &pixel = dst.at<Vec3b>(r, c);

			pixel = colors[label];
		}
	}
	
	// create rectangular bounding box around each region		
	for(int i=0; i<stats.rows; i++) {
		int x = stats.at<int>(Point(0, i));
		int y = stats.at<int>(Point(1, i));
		int w = stats.at<int>(Point(2, i));
		int h = stats.at<int>(Point(3, i));
				
		Scalar color(255,0,0);
		Rect rect(x,y,w,h);
		cv::rectangle(src, rect, color);
	}

    return 0;
}
