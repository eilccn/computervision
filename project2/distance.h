#ifndef DISTANCE_H
#define DISTANCE_H
#include <opencv2/opencv.hpp>

int ssd(std::vector<float> &target_data, std::vector<std::vector<float>> &directory_data, char *num_matches);

#endif