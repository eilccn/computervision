/*
  Eileen Chang
  
  Functions for calculating distance metrics
*/
#include "csv_util.h"
#include "directory.h"
#include "features.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <numeric>
#include <climits>
#include "outputfile.csv"

using namespace std;
using namespace cv;

// sum squared difference
int ssd(std::vector<float> &target_data, std::vector<char *> dir_filenames, std::vector<std::vector<float>> &dir_fvec, char *num_matches) {
    
    // initialize variables
    float target_sum;
    float dir_sum;
    float diff;
    float ssd = 0;
    std::vector<float> dir_sum_values;
    std::vector<float> diff_values;
    std::vector<char *> names;

    // initialize struct for image filename & ssd value pairs
    struct ImageStruct {
        std::string img_name;
        float ssd_value;
    };

    std::vector<ImageStruct> img_ssd; // vector for pairs
    ImageStruct pair; 

    // compute sum of target image features
    target_sum = std::accumulate(target_data.begin(), target_data.end(), 0);

    // ssd computation
    for(int i=0; i<dir_filenames.size(); i++) { 
        // compute sum of each dir image's features
    
        dir_sum = std::accumulate(dir_fvec[i].begin(), dir_fvec[i].end(), 0);
        dir_sum_values.push_back(dir_sum);
            
        // difference between dir image features and target image features
        diff = dir_sum - target_sum;
        diff_values.push_back(diff);

        // sum square difference 
        ssd = diff * diff;

        // push ssd & image filename as pairs to a vector
        pair.ssd_value = ssd;
        pair.img_name = dir_filenames[i];
        img_ssd.push_back(pair);
    }
    
    // sort ssd values from min to max
    sort(img_ssd.begin(), img_ssd.end(), [](const ImageStruct& a, const ImageStruct& b) {
        return a.ssd_value < b.ssd_value;
    });
    
    // return N matches
    int N = atoi(num_matches); // convert argv[6] (N number of matches) from char to integer
    img_ssd.resize(N);

    std::cout << "*********************************" << std::endl;
    std::cout << "The top " <<  N << " matches are:" << std::endl;  
    std::cout << "*********************************" << std::endl;
    for( auto& n : img_ssd) {
        std::cout << n.img_name << ": " << std::fixed << n.ssd_value << std::endl;
    }

    return 0;
}

/*
// histogram intersection
int histx(std::vector<float> &target_data, std::vector<char *> dir_filenames, std::vector<std::vector<float>> &dir_fvec, char *num_matches) {
    // initialize variables
    float target_sum;
    float dir_sum;
    float diff;
    int i, j, c;
    std::vector<float> dir_sum_values;
    std::vector<float> diff_values;
    std::vector<char *> names;

    struct ImageStruct {
        char *img_name;
        float value;
    };

    std::vector<ImageStruct> img_value;
    ImageStruct pair;

    for(i=0; i<target_data.size(); i++) {
        for(j=0; j<)
    }

    // 1 - intersection(target, dir)

    // normalize histogram and store features to feature vector
    for(i=0; i<hist2d.rows; i++) {
        for(j=0; j<hist2d.cols; j++) {
            fvec.push_back(( (float)(hist2d.at<unsigned int>(i,j)) ) / (img.rows * img.cols) ) ;
        }
    
    return 0;
}
*/

