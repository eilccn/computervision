#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "functions.h"

using namespace cv;
using namespace std;

RNG rng(12345); // used for moments_2 fxn

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
 * compute connected components 
 * display regions by color
 * limits recognition to largest N regions
**/
int conn_comp(cv::Mat &src, cv::Mat &dst) {

    // initialize cv::Mat variables for built-in conn comp fxn
    cv::Mat labels, stats, centroids;

    // create binary image
	cv::Mat binary;
	morphological(src, binary);

    // connected components computation
    cv::Mat labelImage(src.size(), CV_32S);
    int nRegions = cv::connectedComponentsWithStats(binary, labelImage, stats, centroids);
    //cout << "Number of connected components = " << nLabels << endl << endl;

	// assign different colors to each connected component region
	std::vector<Vec3b> colors(nRegions);
	colors[0] = Vec3b(0, 0, 0);//background
			
	for (int label = 1; label < nRegions; label++) {
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

    return 0;
}

/** COMPUTE REGION FEATURES & MOMENTS
 * computes set of 2 features for regions given a region map and a region ID
 * computes axis of least central moment and the oriented bounding box
 * all features are translation, scale, and rotation invariant
 * display at least one feature in real time on the video output
**/

// helper function for computing unrotated points
Point2d helper(int xp, int yp, double orientation, int x_cent, int y_cent, int rows) {

    int x_unrotated = ( xp * cos(orientation) ) - ( yp * sin(orientation) ) + x_cent;
    int y_unrotated = ( xp * sin(orientation) ) + ( yp * cos(orientation) ) + y_cent;
    y_unrotated = (rows - 1) - y_unrotated;
    
    return Point2d(x_unrotated, y_unrotated);
}

int moments(cv::Mat &src, cv::Mat &dst) {

    // create binary image
	cv::Mat binary;
	morphological(src, binary);

    // create destination image
    dst = Mat::zeros( binary.size(), CV_8UC3 );

    // connected components 
    cv::Mat labels, stats, centroids; // initialize cv::Mat variables for built-in conn comp fxn
    cv::Mat labelImage(src.size(), CV_32S);
    int nRegions = cv::connectedComponentsWithStats(binary, labelImage, stats, centroids);
    //cout << "Number of connected components = " << nRegions << endl << endl;

    /*
    // print stats and centroids
    cout << "Show statistics and centroids:" << endl;
    cout << "stats:" << endl << "(left,top,width,height,area)" << endl << stats << endl << endl;
    cout << "centroids: " << endl << "(x, y)" << endl << centroids << endl << endl; 
    cout << "labels: " << endl << labels << std::endl;
	cout << "stats.size()=" << endl << stats.size() << std::endl;
    */

    // compute axes of least central moments for each region
    for (int label=0; label < nRegions; label++) {

        // select the image part relative to label
        cv::Mat obj = (labelImage == label);              

        // get moments as world coordinates    
        Moments m = cv::moments(obj, true); 

        // centroid (x, y)
        Point2d mc(m.m10/m.m00, m.m01/m.m00); 
        int x_cent = m.m10/m.m00;
        int y_cent = m.m01/m.m00; 

        // orientation of least central moment
        double pi = 3.14159265359; // convert degrees to radians
        double orientation =  ( 0.5*atan(2*m.mu11 / (m.mu20 - m.mu02)) ) * (pi / 180); 

        // second points to display axes of least central moment
        int x2 = x_cent + 200 * cos(orientation);
        int y2 = y_cent - 200 * sin(orientation);
        cv::Point p2(x2, y2);

        int x3 = x_cent + 200 * -1 * sin(orientation);
        int y3 = y_cent + 200 * cos(orientation);
        cv::Point p3(x3, y3);

        // draw axes
        cv::line(dst, mc, p2, {255,0,0}, 3); 
        cv::line(dst, mc, p3, {255,0,0}, 3); 
        // draw centroid
        cv::circle(dst, mc, 3, {255, 255, 255}, 3);

        // compute min & max (xprime, yprime) points
        int minxp=INT_MIN;
        int maxxp=INT_MAX;
        int minyp=INT_MIN;
        int maxyp=INT_MAX;
        
        for (int i=0; i<src.rows; i++) {
            for(int j=0; j<src.cols; j++) {
                int x = j;
                int y = (src.rows - 1) - i;

                float xpf = (x - x_cent)*cos(orientation) + (y - y_cent)*sin(orientation);
                float ypf = ( (x - x_cent)*(-1.0 * sin(orientation)) ) +  ( (y - y_cent)*cos(orientation) );

                int xp = (int) xpf;
                int yp = (int) ypf;

                if (xp < minxp) {
                    minxp = xp;
                }
                if (xp > maxxp) {
                    maxxp = xp;
                }
                if (yp < minyp) {
                    minyp = yp;
                }
                if (yp > maxxp) {
                    maxxp = yp;
                }

            }
        }

        // computing unrotated points
        Point2d min_xpyp = helper(minxp, minyp, orientation, x_cent, y_cent, src.rows);
        Point2d max_xpyp = helper(maxxp, maxyp, orientation, x_cent, y_cent, src.rows);
        Point2d minmax_xpyp = helper(minxp, maxyp, orientation, x_cent, y_cent, src.rows);
        Point2d maxmin_xpyp = helper(maxxp, minyp, orientation, x_cent, y_cent, src.rows);
        
        /*
        cv::line(dst, min_xpyp, max_xpyp, {255,0,0}, 3); 
        cv::line(dst, max_xpyp, minmax_xpyp, {0,255,0}, 3); 
        cv::line(dst, minmax_xpyp, maxmin_xpyp, {0,0,255}, 3); 
        cv::line(dst, maxmin_xpyp, min_xpyp, {255,255,255}, 3); 
        */
        
    }

   // oriented bounding box
    vector<vector<Point> > contours;
    findContours( binary, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );

    for( size_t i = 0; i < contours.size(); i++ )
    {
        minRect[i] = minAreaRect( contours[i] );
        if( contours[i].size() > 5 )
        {
            minEllipse[i] = fitEllipse( contours[i] );
        }
    }

    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        // contour
        drawContours( dst, contours, (int)i, color );

        // ellipse
        //ellipse( dst, minEllipse[i], color, 2 );

        // rotated rectangle
        Point2f rect_points[4];
        minRect[i].points( rect_points );
        for ( int j = 0; j < 4; j++ )
        {
            line( dst, rect_points[j], rect_points[(j+1)%4], color );
        }
    }

    return 0;
}


/** EXTRACT FEATURES TO DATABASE
 * collect feature vectors from objects, attach labels, and store them in csv DB
 * prompt the user for a name/label ...
 * then store the feature vector for the current object and its label into csv DB
 * works for both real-time and still images
**/
int features(cv::Mat &src, cv::Mat &dst, char *argv) {
    return 0;
}




