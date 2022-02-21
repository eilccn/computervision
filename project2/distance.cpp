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
int ssd(std::vector<float> &target_data, std::vector<std::vector<float>> &directory_data) {
    
    // initialize variables
    double target_sum;
    float dir_sum;
    float diff;
    float ssd = 0;
    std::vector<float> ssd_values;
    std::vector<float> dir_sum_values;
    std::vector<float> diff_values;

    // compute sum of target image features
    target_sum = std::accumulate(target_data.begin(), target_data.end(), 0);

    // ssd computation
    for( auto& n : directory_data ) {
        // compute sum of each dir image's features
        dir_sum = std::accumulate(n.begin(), n.end(), 0);
        dir_sum_values.push_back(dir_sum);
            
        // difference between dir image features and target image features
        diff = dir_sum - target_sum;
        diff_values.push_back(diff);

        // sum square difference 
        ssd = diff * diff;

        ssd_values.push_back(ssd);
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
    sort(ssd_values.begin(), ssd_values.end());
    
    // return N matches

    /*ssd_values.resize(N);*/

    for( auto& n : ssd_values) {
        std::cout << std::fixed << n << std::endl;
    }

    return 0;
}


