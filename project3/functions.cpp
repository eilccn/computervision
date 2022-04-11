/*
Eileen Chang

CS 5330 Computer Vision
Spring 2022

Functions.cpp that contains functions for 2d object recognition (in order):
* threshold 
* morphological
* cc (connected components)
* features
* training
* classify
* knn

More in-depth descriptions for each function are provided along with each function below
*/
#include <cstdio>
#include <cmath>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <numeric>
#include <algorithm>
#include "functions.h"
#include "csv_util.h"
#include "features.csv"

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




/** REGION FEATURES
 * computes set of features for regions given a region map and a region ID
 * computes major and minor axes of least central moment and the oriented bounding box
 * all features are translation, scale, and rotation invariant
 * displays the major and minor axes and the oriented bounding box in real time on the video output
 * creates and outputs a featureset vector for each object 
**/

// helper function for computing unrotated points
Point helper(int xp, int yp, double orientation, int x_cent, int y_cent, int rows) {
    double pi = 3.14159265359; // convert degrees to radians

    int x_unrotated = ( xp * cos(orientation) ) - ( yp * sin(orientation) ) + x_cent;
    int y_unrotated = ( xp * sin(orientation) ) + ( yp * cos(orientation) ) + y_cent;
    y_unrotated = (rows - 1) - y_unrotated;
    
    return Point(x_unrotated, y_unrotated);
}

// helper vector print function
void print(std::vector <double> const &a) {
   std::cout << "The vector elements are: " << endl;

   for(int i=0; i < a.size(); i++)
   std::cout << a.at(i) << endl;
}

/** features function
 * parameters: source image, destination image, vector for feature set values to be pushed into
 **/
int features(cv::Mat &src, cv::Mat &dst, std::vector<double> &featureset) {

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
    // commented out loop for calculating features for multiple objects simultaneously 
    //for (int label=1; label < nRegions; label++) { 

    // select the first major object region
    cv::Mat obj = (labelImage == 1);              

    /** MOMENTS **/
    Moments m = cv::moments(labelImage, true); 
    // push central moments to featureset
    featureset.push_back((double) m.mu11);
    featureset.push_back((double) m.mu02);
    featureset.push_back((double) m.mu20);

    // centroid (x, y)
    Point mc(m.m10/m.m00, m.m01/m.m00); 
    int x_cent = m.m10/m.m00;
    int y_cent = m.m01/m.m00; 

    // orientation of least central moment
    double pi = 3.14159265359; // convert degrees to radians
    double orientation =  ( 0.5*atan2(2*m.mu11, (m.mu20 - m.mu02)) );

    // second points to display axes of least central moment
    // major axis second point
    int x2 = x_cent + 200 * cos(orientation);
    int y2 = y_cent + 200 * sin(orientation);
    cv::Point p2(x2, y2);

    // minor axis second point
    int x3 = x_cent + 200 * (-1)*sin(orientation);
    int y3 = y_cent + 200 * cos(orientation);
    cv::Point p3(x3, y3);

    // draw axes 
    // major axis line
    cv::line(dst, mc, p2, {255,0,0}, 3); 
    // minor axis line
    cv::line(dst, mc, p3, {255,0,0}, 3); 
    // draw centroid
    cv::circle(dst, mc, 3, {255, 255, 255}, 3);
    

    /** ORIENTED BOUNDING BOX **/
    // initialize variables
    vector<vector<Point> > contours;
    findContours( binary, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );

    // push % filled of oriented bounding box to featureset 
    featureset.push_back((double) contourArea(contours[1], false));
    
    // push aspect ratio of oriented bounding box to featureset 
    Rect rect = boundingRect(contours[1]);
    long double aspect_ratio = (double) rect.width / (double) rect.height;
    featureset.push_back(aspect_ratio);

    for( size_t i = 0; i < contours.size(); i++ )
    {
        minRect[i] = minAreaRect( contours[i] );
    }

    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        // compute contour
        drawContours( dst, contours, (int)i, color );

        // rotated rectangle
        Point2f rect_points[4];
        minRect[i].points( rect_points );
        for ( int j = 0; j < 4; j++ )
        {
            line( dst, rect_points[j], rect_points[(j+1)%4], color );
        }
    }

    /* print featureset
     * featureset: {mu22, mu02, mu20, % filled of oriented bounding box, aspect ratio of oriented bounding box}
     */
    //print(featureset);

    /** MIN/MAX OF (X', Y')
     * compute x', y' and their min/max values
     * draw lines connecting min/max values to create an oriented bounding box
     * alternative method (to contours above) for drawing oriented bounding box
     **/
    // initialize variables for xprime and yprime
    int xp, yp;
    // compute min & max (xprime, yprime) points
    int minxp=0;
    int maxxp=1000;
    int minyp=0;
    int maxyp=1000;
    
    for (int i=0; i<obj.rows; i++) {
        for(int j=0; j<obj.cols; j++) {
            int x = j;
            int y = (obj.rows - 1) - i;

            int xp = (x - x_cent)*cos(orientation) + (y - y_cent)*sin(orientation);
            int yp = ( (x - x_cent)*(-1.0 * sin(orientation)) ) +  ( (y - y_cent)*cos(orientation) );

            if (xp < minxp) {
                minxp = xp;
            }
            if (xp > maxxp) {
                maxxp = xp;
            }
            if (yp < minyp) {
                minyp = yp;
            }
            if (yp > maxyp) {
                maxyp = yp;
            }

        }
    }
    //cout << "minxp, maxxp, ..." << endl;
    //cout << minxp << endl << maxxp << endl << minyp << endl << maxyp << endl;
    
    // compute unrotated points
    Point min_xpyp = helper(minxp, minyp, orientation, x_cent, y_cent, obj.rows);
    Point max_xpyp = helper(maxxp, minyp, orientation, x_cent, y_cent, obj.rows);
    Point minmax_xpyp = helper(minxp, maxyp, orientation, x_cent, y_cent, obj.rows);
    Point maxmin_xpyp = helper(maxxp, maxyp, orientation, x_cent, y_cent, obj.rows);
    
    //cout << "POINTS" << endl << endl;
    //cout << min_xpyp << endl << max_xpyp << endl << minmax_xpyp << endl << maxmin_xpyp << endl;
    
    /*
    // draw bounding box lines
    cv::line(dst, min_xpyp, minmax_xpyp, {255,0,0}, 3); 
    cv::line(dst, minmax_xpyp, maxmin_xpyp, {0,255,0}, 3); 
    cv::line(dst, maxmin_xpyp, max_xpyp, {0,0,255}, 3); 
    cv::line(dst, max_xpyp, min_xpyp, {255,255,255}, 3); 
    */
    
    return 0;
}




/** EXTRACT FEATURES TO DATABASE
 * parameters: source image, destination image, path of csv file passed in from command line
 * collect feature vectors from objects, attach labels, and store them in csv DB
 * prompt the user for a name/label ...
 * then store the feature vector for the current object and its label into csv DB
 * works for both real-time and still images
**/
int training(cv::Mat &src, cv::Mat &dst, char *csv_file) {

     // initialize variables
     char outputfile[256];
     char name[256];
     strcpy(outputfile, csv_file);
     std::vector<double> featureset;

     // compute features
     features(src, dst, featureset);

     // obtain user input for object label/name
     cout << "Enter a label for the object: ";
     cin >> name;

     // write feature set to csv database
     append_image_data_csv(outputfile, name, featureset, 0);

     return 0;
}




/** CLASSIFY UNKNOWN OBJECTS
 * compute features of unknown object
 * compare unknown features to known features in csv database
 * use scaled Euclidean distance metric 
 * label the unknown object as its closest match
 * output the label on the video display
 */
int classify(cv::Mat &src, cv::Mat &dst, std::vector<double> &unknown_featureset, std::vector<char *> obj_labels, std::vector<std::vector<double>> &db_featureset) {

    // initialize struct for object name and its corresponding distance metric value
    struct ObjectStruct {
        double value;
        string object;
    };
    std::vector<ObjectStruct> obj_distance; //vector for pairs
    ObjectStruct pair;

    // compute scaled Euclidean distance
    for (int i=0; i<obj_labels.size(); i++) {
        double distance = 0;
        for (int j=0; j<unknown_featureset.size(); j++) {
            distance += ( unknown_featureset[j] - db_featureset[i][j] ) * ( unknown_featureset[j] - db_featureset[i][j] ) ;
            
        }
        distance = (double) sqrt(distance); 

        // push distance metrics and object names as pairs to vector
        pair.value = distance;
        pair.object = obj_labels[i];
        obj_distance.push_back(pair);
    }
    
    // sort distance metrics from min to max
    sort(obj_distance.begin(), obj_distance.end(), [](const ObjectStruct& a, const ObjectStruct& b) {
        return a.value < b.value;
    });

    // display match as text on video output
    cv::putText(dst, obj_distance[0].object, 
                cv::Point(10, dst.rows / 2),
                cv::FONT_HERSHEY_DUPLEX,
                1.0,
                CV_RGB(118, 185, 0), //font color
                2);
    
    /*
    // print list of all objects ordered from best match to least match
    std::cout << "*********************************" << std::endl;
    std::cout << "All objects ordered from best match to least match:" << std::endl;  
    std::cout << "*********************************" << std::endl;
    for( auto& n : obj_distance) {
        std::cout << n.object << ": " << std::fixed << n.value << std::endl;
    }
    */

    return 0;
}




/** KNN CLASSIFIER
 * compute features of unknown object
 * compare unknown features to known features in csv database
 * use scaled Euclidean distance metric to sort from closest match to furthest match
 * find K neighbors and count the frequency of an object class 
 * label the unknown object as its closest match
 */
int KNN(cv::Mat &src, cv::Mat &dst, std::vector<double> &unknown_featureset, std::vector<char *> obj_labels, std::vector<std::vector<double>> &db_featureset, int k_value) {

    // initialize struct for object name and its corresponding distance metric value
    struct ObjectStruct {
        double value;
        string object;
    };
    std::vector<ObjectStruct> obj_distance; //vector for pairs
    ObjectStruct pair;

    // compute scaled Euclidean distance
    for (int i=0; i<obj_labels.size(); i++) {
        double distance = 0;
        for (int j=0; j<unknown_featureset.size(); j++) {
            distance += ( unknown_featureset[j] - db_featureset[i][j] ) * ( unknown_featureset[j] - db_featureset[i][j] ) ;
            
        }
        distance = (double) sqrt(distance); 

        // push distance metrics and object names as pairs to vector
        pair.value = distance;
        pair.object = obj_labels[i];
        obj_distance.push_back(pair);
    }
    
    // sort distance metrics from min to max
    sort(obj_distance.begin(), obj_distance.end(), [](const ObjectStruct& a, const ObjectStruct& b) {
        return a.value < b.value;
    });

    /** K nearest neighbor matching **/
    // initialize vector for counts of K nearest neighbors of each object
    std::vector<std::pair<int, std::string> > results;
    std::vector<int> arr(k_value);
    int visited = -1;

    // count how many of K neighbors are the same object
    for (int i=0; i<k_value; i++) {
        int count = 1;
        for (int j=i+1; j<k_value; j++) {
            if (obj_distance[i].object == obj_distance[j].object) {
                count++;

                // to avoid counting same element again
                arr[j] = visited;
            }
        }

        if (arr[i] != visited) {
            arr[i] = count;
        }

        results.push_back({arr[i], {obj_distance[i].object}});
        
    }

    // sort the counts from largest to smallest
    sort(results.rbegin(), results.rend());
    
    // print the best match object along with its count
    std::cout << "*********************************" << std::endl;
    std::cout << "The best match:" << std::endl;  
    std::cout << "*********************************" << std::endl;
    std::cout << results[0].second << endl;
    std::cout << results[0].first << " out of K=" << k_value << " neighbors classfied as " << results[0].second << std::endl;

    // print list of all objects ordered from best match to least match
    /*
    std::cout << "*********************************" << std::endl;
    std::cout << "All objects ordered from best match to least match:" << std::endl;  
    std::cout << "*********************************" << std::endl;
    for( auto& n : results) {
        std::cout << n.second << ": " << std::fixed << n.first << std::endl;
    }
    */
   
    /** note: removed text display functionality and the best match is revealed in terminal
    // display match as text on video output
    cv::putText(dst, results[0].second, 
                cv::Point(10, dst.rows / 2),
                cv::FONT_HERSHEY_DUPLEX,
                1.0,
                CV_RGB(118, 185, 0), //font color
                2);
    **/





    /*** ALTERNATIVE CODE for use in the same approach as the final implemented code except it can only classify with 10 hardcoded preset items
     * note: this function will only work with the preset objects below
     * the final above implementation uses a different functionality that can work with any object
    ***/

    /*
    // initialize object groups
    int pencil = 0;
    int key = 0;
    int star = 0;
    int paint = 0;
    int tree = 0;
    int steps = 0;
    int flower = 0;
    int rect = 0;
    int pin = 0;
    int camera = 0;
    // count how many of K neighbors are the same object
    for (int i=0; i<k_value; i++) {
        if (obj_distance[i].object == "pencil") {
            pencil++;
        }
        if (obj_distance[i].object == "key") {
            key++;
        }
        if (obj_distance[i].object == "star") {
            star++;
        }
        if (obj_distance[i].object == "paint") {
            paint++;
        }
        if (obj_distance[i].object == "tree") {
            tree++;
        }
        if (obj_distance[i].object == "steps") {
            steps++;
        }
        if (obj_distance[i].object == "flower") {
            flower++;
        }
        if (obj_distance[i].object == "rect") {
            rect++;
        }
        if (obj_distance[i].object == "pin") {
            pin++;
        }
        if (obj_distance[i].object == "camera") {
            camera++;
        }
    }
  
    // push total counts to vector
    results.push_back({pencil, {"pencil"}});
    results.push_back({key, {"key"}});
    results.push_back({star, {"star"}});
    results.push_back({paint, {"paint"}});
    results.push_back({tree, {"tree"}});
    results.push_back({steps, {"steps"}});
    results.push_back({flower, {"flower"}});
    results.push_back({rect, {"rect"}});
    results.push_back({pin, {"pin"}});
    results.push_back({camera, {"camera"}});
    */
        

    return 0;
}




/** KNN SUMS CLASSIFIER
 * compute features of unknown object
 * compare unknown features to known features in csv database
 * use scaled Euclidean distance metric to sort objects by closest match to furthest match
 * use KNN to find the distance sums of the first K elements of each object class
 * sort the distance sums from above from least to greatest
 * the object class with the smallest distance sum is the best match
 */
/*
int KNN_sums(cv::Mat &src, cv::Mat &dst, std::vector<double> &unknown_featureset, std::vector<char *> obj_labels, std::vector<std::vector<double>> &db_featureset, int k_value) {

    // initialize struct for object name and its corresponding distance metric value
    struct ObjectStruct {
        double value;
        string object;
    };
    std::vector<ObjectStruct> obj_distance; //vector for pairs
    ObjectStruct pair;

    // compute scaled Euclidean distance
    for (int i=0; i<obj_labels.size(); i++) {
        double distance = 0;
        for (int j=0; j<unknown_featureset.size(); j++) {
            distance += ( unknown_featureset[j] - db_featureset[i][j] ) * ( unknown_featureset[j] - db_featureset[i][j] ) ;
            
        }
        distance = (double) sqrt(distance); 

        // push distance metrics and object names as pairs to vector
        pair.value = distance;
        pair.object = obj_labels[i];
        obj_distance.push_back(pair);
    }
    
    // sort distance metrics from min to max
    sort(obj_distance.begin(), obj_distance.end(), [](const ObjectStruct& a, const ObjectStruct& b) {
        return a.value < b.value;
    });




    // K nearest neighbor matching
    // initialize vector for counts of K nearest neighbors of each object
    std::vector<std::pair<int, std::string> > results;
    std::vector<int> arr;
    int visited = 1;

    // count how many of K neighbors are the same object
    for (int i=0; i<obj_distance.size(); i++) {
        int sum = 0;
        for (int j=i+1; j<obj_distance.size(); j++) {
            if (obj_distance[i].object == obj_distance[j].object) {
                sum += obj_distance[j].value;

                // to avoid counting same element again
                arr[j] = visited;
            }
        }

        if (arr[i] != visited) {
            arr[i] = sum;
        }

        results.push_back({arr[i], {obj_distance[i].object}});
        
    }

    // sort the counts from largest to smallest
    sort(results.begin(), results.end());

    // print list of all objects ordered from best match to least match
    std::cout << "*********************************" << std::endl;
    std::cout << "All objects ordered from best match to least match:" << std::endl;  
    std::cout << "*********************************" << std::endl;
    for( auto& n : results) {
        std::cout << n.second << ": " << std::fixed << n.first << std::endl;
    }
    
    // print the best match object along with its count
    std::cout << "*********************************" << std::endl;
    std::cout << "The best match:" << std::endl;  
    std::cout << "*********************************" << std::endl;
    std::cout << results[0].second << results[0].first << std::endl;
    */



    /*
    // ALTERNATIVE CODE
    // initialize object groups
    std::vector<int> pencil_vec;
    std::vector<int> key_vec;
    std::vector<int> star_vec;
    std::vector<int> paint_vec;
    std::vector<int> tree_vec;
    std::vector<int> steps_vec;
    std::vector<int> flower_vec;
    std::vector<int> rect_vec;
    std::vector<int> pin_vec;
    std::vector<int> camera_vec;
    

    // find the closest K neighbors of each object and sum their distances
    for(auto &n : obj_distance) {
        if (n.object == "pencil") {
            pencil_vec.push_back(n.value);
        }
        if (n.object == "key") {
            key_vec.push_back(n.value);
        }
        if (n.object == "star") {
            star_vec.push_back(n.value);
        }
        if (n.object == "paint") {
            paint_vec.push_back(n.value);
        }
        if (n.object == "tree") {
            tree_vec.push_back(n.value);
        }
        if (n.object == "steps") {
            steps_vec.push_back(n.value);
        }
        if (n.object == "flower") {
            flower_vec.push_back(n.value);
        }
        if (n.object == "rect") {
            rect_vec.push_back(n.value);
        }
        if (n.object == "pin") {
            pin_vec.push_back(n.value);
        }
        if (n.object == "camera") {
            camera_vec.push_back(n.value);
        }
    }

    // sum first K values of each object vector 
    int pencil = std::accumulate(pencil_vec.begin(), pencil_vec.begin() + k_value, 0);
    int key = std::accumulate(key_vec.begin(), key_vec.begin() + k_value, 0);
    int star = std::accumulate(star_vec.begin(), star_vec.begin() + k_value, 0);
    int paint = std::accumulate(paint_vec.begin(), paint_vec.begin() + k_value, 0);
    int tree = std::accumulate(tree_vec.begin(), tree_vec.begin() + k_value, 0);
    int steps = std::accumulate(steps_vec.begin(), steps_vec.begin() + k_value, 0);
    int flower = std::accumulate(flower_vec.begin(), flower_vec.begin() + k_value, 0);
    int rect = std::accumulate(rect_vec.begin(), rect_vec.begin() + k_value, 0);
    int pin = std::accumulate(pin_vec.begin(), pin_vec.begin() + k_value, 0);
    int camera = std::accumulate(camera_vec.begin(), camera_vec.begin() + k_value, 0);

    // push total sums to vector
    results.push_back({pencil, {"pencil"}});
    results.push_back({key, {"key"}});
    results.push_back({star, {"star"}});
    results.push_back({paint, {"paint"}});
    results.push_back({tree, {"tree"}});
    results.push_back({steps, {"steps"}});
    results.push_back({flower, {"flower"}});
    results.push_back({rect, {"rect"}});
    results.push_back({pin, {"pin"}});
    results.push_back({camera, {"camera"}});
    
    
    return 0;
}
*/

