#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int threshold(cv::Mat &src, cv::Mat &dst);

int morphological(cv::Mat &src, cv::Mat &dst);

int conn_comp(cv::Mat &src, cv::Mat &dst);

int moments(cv::Mat &src, cv::Mat &dst);

int features(cv::Mat &img, std::vector<std::vector<float>> &fvec, char *obj_name);

#endif