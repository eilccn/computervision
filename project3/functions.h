#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int threshold(cv::Mat &src, cv::Mat &dst);

int morphological(cv::Mat &src, cv::Mat &dst);

int conn_comp(cv::Mat &src, cv::Mat &dst);

int moments(cv::Mat &src, cv::Mat &dst, std::vector<double> &featureset);

int features(cv::Mat &img, std::vector<double> &featureset, char *obj_name, char *csv_file);

#endif