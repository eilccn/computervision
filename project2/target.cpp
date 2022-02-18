/*
  Eileen Chang
  
  Code to read target image, compute its features, then compare its features to directory image features written to a csv file
*/

#include "directory.h"
#include "features.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include <opencv2/core/types.hpp>
#include "outputfile.csv"

using namespace std;
using namespace cv;

int main( int argc, char *argv[] ) {

    // check for sufficient arguments
    if (argc < 2) {
        std::cout << "Expecting an image file to be passed to program" << std::endl;
        exit(-1);
    }
    else if(argc < 3) {
        printf("Expecting directory path: %s <directory path>\n", argv[0]);
        exit(-1);
    }
    
    // read target image
    cv::Mat img = cv::imread(argv[1]);

    if (img.empty()) {
        std::cout << "Not a valid image file" << std::endl;
        return -1;
    }

    // compute target image features 
    // compute baseline features of 9x9 center square
    std::vector<float> fvec;
    process_baseline(img, fvec); 

    // check if directory feature set csv file already exists
    // if exists, don't execute process_directory fxn
    /*
    std::ifstream file(outputfile);

    if( !outputfile ) {
        printf("file is not open");
        return -1;
    }
    else if( !is_empty(outputfile) ) {
        append_image_data_csv(outputfile, dp->d_name, fvec, 0);
    }
    */
    

    // read directory, compute image features, write features to csv file
    process_directory(argv[2], argv[3]); 

    /*
    // normalize target image feature vector data
    std::vector<float> normalized_target;
    for (int n : fvec) {
        float normalized;
        normalized = fvec / 243;
        normalized_target.push_back(normalized);
    }
    */

    // compare target image to directory images using sum of squared difference
    

    // sort list of matches

    // return top N matches

    return 0;
}