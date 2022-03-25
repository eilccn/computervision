#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int threshold(cv::Mat &src, cv::Mat &dst);

int morphological(cv::Mat &src, cv::Mat &dst);

int conn_comp(cv::Mat &src, cv::Mat &dst);

int features(cv::Mat &src, cv::Mat &dst, std::vector<double> &featureset);

int training(cv::Mat &src, cv::Mat &dst, char *csv_file);

int classify(std::vector<double> &unknown_featureset,  std::vector<char *> obj_labels, std::vector<std::vector<double>> &db_featureset);

#endif