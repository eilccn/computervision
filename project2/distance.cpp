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
    double target_sum;
    float dir_sum;
    float diff;
    float ssd = 0;
    std::vector<float> ssd_values;
    std::vector<float> dir_sum_values;
    std::vector<float> diff_values;
    std::vector<char *> names;

    struct ImageStruct {
        char *img_name;
        float ssd_value;
    };

    std::vector<ImageStruct> img_ssd;
    ImageStruct pair;
    

    // compute sum of target image features
    target_sum = std::accumulate(target_data.begin(), target_data.end(), 0);

    // ssd computation
    for( auto& n : dir_fvec ) { 
        // compute sum of each dir image's features
        dir_sum = std::accumulate(n.begin(), n.end(), 0);
        dir_sum_values.push_back(dir_sum);
            
        // difference between dir image features and target image features
        diff = dir_sum - target_sum;
        diff_values.push_back(diff);

        // sum square difference 
        ssd = diff * diff;

        pair.ssd_value = ssd;

        
        for(auto& x : dir_filenames ) {
            pair.img_name = x;
        }

        img_ssd.push_back(pair);

        ssd_values.push_back(ssd);

    }
    

    std::cout << "SSD values for each image:" << std::endl;
    for( auto& x : img_ssd) {
        std::cout << x.img_name << ": " << std::fixed << x.ssd_value << std::endl;
    }
    
    /*
    // ssd test
    for( auto& n : directory_data ) {
        for(auto& i : n ) {
            ssd += ( target_data[i] - n[i] ) * ( target_data[i] - n[i] );
            ssd_values.push_back(ssd);
        }   
    }
    */

    // sort ssd values from min to max
    sort(img_ssd.begin(), img_ssd.end(), [](ImageStruct a, ImageStruct b) {
        return a.ssd_value < b.ssd_value;
    });
    
    // return N matches
    int N = atoi(num_matches); // convert argv[6] (N number of matches) from char to integer
    img_ssd.resize(N);

    std::cout << "**********************************" << std::endl;
    std::cout << "The top " <<  N << " matches are:" << std::endl;  
    for( auto& n : img_ssd) {
        std::cout << n.img_name << ": " << std::fixed << n.ssd_value << std::endl;
    }

    return 0;
}


