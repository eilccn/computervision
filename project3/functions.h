#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int threshold(cv::Mat &src, cv::Mat &dst);

int morphological(cv::Mat &src, cv::Mat &dst);

int conn_comp(cv::Mat &src, cv::Mat &dst);

int features(cv::Mat &src, cv::Mat &dst, std::vector<double> &featureset);

int training_set(cv::Mat &src, cv::Mat &dst, char *csv_file);

#endif