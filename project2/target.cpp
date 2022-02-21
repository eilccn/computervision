/*
  Eileen Chang
  
  Main function 
  Command line program that takes in target image path, image directory path, csv file path, feature type, matching method, and # imgages N to return
  Computes target image features then calls functions that compute directory image features and functions that return the top N matching images 
*/

#include "directory.h"
#include "features.h"
#include "csv_util.h"
#include "distance.h"
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

    /*  check for sufficient arguments */
    if (argc < 4) {
        std::cout << "Please enter: \n[argv0]: ./project2 \n"
            "[argv1]: target image path \n"
            "[argv2]: directory path \n"
            "[argv3]: csv file \n"
            "[argv4]: feature type \n"
            "[argv5]: matching method \n"
            "[argv6]: # images N to return" << std::endl;
        exit(-1);
    }

    /* read target image */
    cv::Mat img = cv::imread(argv[1]);

    if (img.empty()) {
        std::cout << "Not a valid image file" << std::endl;
        return -1;
    }

    /* compute target image features based on feature type cl argv[4] argument */
    // initialize vector for target image feature set
    std::vector<float> target_fvec; 

    if (strcmp(argv[4], "b") == 0) {
        // compute 9x9 center feature vector
        process_baseline(img, target_fvec);
    }

    /* write feature set data to csv file if it exists OR read feature set data if csv file already exists */
    // initialize csv file variable
    std::fstream fileStream;
    fileStream.open(argv[3]);
    char outputfile[256]; 
    strcpy(outputfile, argv[3]);

    // initialize vectors for the read function to push data into
    std::vector<char *> dir_filenames; // vector for image file names
    std::vector<std::vector<float>> dir_fvec; // vector for feature set data

    if( fileStream.peek() == std::ifstream::traits_type::eof() ) { // if csv is empty, process directory
        // read directory, compute image features, write features to csv file, read from the csv file, push csv data into initialized vectors
        process_directory(argv[2], argv[3], argv[4]); 
        read_image_data_csv( outputfile, dir_filenames, dir_fvec, 0 );
    }
    else {
        // if csv is not empty, simply read the existing csv file and push csv data into initialized vectors
        read_image_data_csv( outputfile, dir_filenames, dir_fvec, 0 );
    }

    /* compare target image to directory images using distance functions and return N number of the top matches */
    /*int N = atoi(argv[6]); // convert argv[6] (N number of matches) from char to integer*/

    
    if(strcmp(argv[5], "ssd") == 0) {
        ssd(target_fvec, dir_fvec);
    }
    
 

    return 0;
}