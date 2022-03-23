#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "functions2.h"

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
 * 
**/
/* moments rotation */
int moments(cv::Mat &src, cv::Mat &dst) {
    // initialize cv::Mat variables for built-in conn comp fxn
    vector<vector<int>> contours;
    vector<int> hierarchy;
    Mat mTest,mThresh,mConnected;

    Mat m;
    cv::Mat labels, stats, centroids;

    // create binary image
    cv::Mat binary;
    morphological(src, binary);

    // connected components computation
    cv::Mat labelImage(src.size(), CV_8UC1);
    int nRegions = cv::connectedComponentsWithStats(binary, labelImage, stats, centroids);
    //cout << "Number of connected components = " << nLabels << endl << endl;
        

    findContours(labelImage,contours,hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    vector<int> ctrSelec;
    for (int i = 0; i < contours.size(); i++)
    {
        if (contours[i].size()>= 500 )
        {
            ctrSelec.push_back(i);
        }
    }

    Mat mc = Mat::zeros(m.size(),CV_8UC3);

    vector< vector<Point2f> >z;
    vector< vector<Point2f> >Z;
    z.resize(ctrSelec.size());
    Z.resize(ctrSelec.size());


    for (int i = 0; i < ctrSelec.size();i++)
    {
        ximgproc::contourSampling(contours[ctrSelec[i]], z[i], 1024);
        dft(z[i],Z[i],DFT_SCALE); //Fourier transform
    }

    int indRef = 1;
    vector<float> alpha,phi,s;
    alpha.resize(ctrSelec.size());
    phi.resize(ctrSelec.size());
    s.resize(ctrSelec.size());
    ximgproc::ContourFitting fit;
    double dist;
    Mat t;
    fit.setFDSize(50);
    fit.setCtrSize(1024);
    vector<vector<Point>> ctrRotated;

    for (int i = 0; i < ctrSelec.size();i++)
    {
        fit.estimateTransformation(z[indRef], z[i], t, &dist, false);
        cout<<“Contour “<<indRef<<” with “<<i<< " origin “<<1-t.at(0,0)<<” and rotated of”<<t.at(0, 1)* 180 / M_PI<<” and scale "<<t.at(0, 2)<<endl;

        for (int i = 0; i < ctrSelec.size(); i++)
        {
            if (i!=indRef)
                drawContours(mc,contours,ctrSelec[i],Scalar(255,0,0));
            else
                drawContours(mc,contours,ctrSelec[i],Scalar(255,255,255));
            putText(mc,format("%d",i),Point(Z[i][0].x,Z[i][0].y),FONT_HERSHEY_SIMPLEX,1,Scalar(255,0,0));
        }

        ximgproc::transformFD(z[1], t, dst, false);
        cout<<"bla bla"<<endl;
        ctrRotated.push_back(dst);
        drawContours(mc,ctrRotated,0,Scalar(0,0,255));

        for (int i = 0; i < ctrSelec.size(); i++)
        {
            drawContours(mc,ctrRotated,i,Scalar(0,0,255));
        }
    }

    return 0;
}

int moments_save() {
    int minx_ur, maxx_ur, miny_ur, maxy_ur;

    // min_xpyp
    for (int i=0; i<obj.rows; i++) {
        for (int j=0; j<obj.cols; j++) {
            minx_ur = ( minxp * cos(orientation) ) - ( minyp * sin(orientation) ) + x_cent;
            miny_ur = ( minxp * sin(orientation) ) + ( minyp * cos(orientation) ) + y_cent;

            int x = j;
            int y = (obj.rows - 1) - i;
        }
    }

    // max_xpyp
    for (int i=0; i<obj.rows; i++) {
        for (int j=0; j<obj.cols; j++) {
            maxx_ur = ( maxxp * cos(orientation) ) - ( maxyp * sin(orientation) ) + x_cent;
            maxy_ur = ( maxxp * sin(orientation) ) + ( maxyp * cos(orientation) ) + y_cent;

            int x = j;
            int y = (obj.rows - 1) - i;
        }
    }

    // minmax_xpyp
    for (int i=0; i<obj.rows; i++) {
        for (int j=0; j<obj.cols; j++) {
            minx_ur = ( minxp * cos(orientation) ) - ( maxyp * sin(orientation) ) + x_cent;
            maxy_ur = ( minxp * sin(orientation) ) + ( maxyp * cos(orientation) ) + y_cent;

            int x = j;
            int y = (obj.rows - 1) - i;
        }
    }

    // maxmin_xpyp
    for (int i=0; i<obj.rows; i++) {
        for (int j=0; j<obj.cols; j++) {
            maxx_ur = ( maxxp * cos(orientation) ) - ( minyp * sin(orientation) ) + x_cent;
            miny_ur = ( maxxp * sin(orientation) ) + ( minyp * cos(orientation) ) + y_cent;

            int x = j;
            int y = (obj.rows - 1) - i;
        }
    }
        
    Point min_xpyp(minx_ur, miny_ur);
    Point max_xpyp(maxx_ur, maxy_ur);
    Point minmax_xpyp(minx_ur, maxy_ur);
    Point maxmin_xpyp(maxx_ur, miny_ur);

    cout << "POINTS" << endl << endl;
    cout << min_xpyp << endl << max_xpyp << endl << minmax_xpyp << endl << maxmin_xpyp << endl;

    return 0;
}


