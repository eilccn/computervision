#ifndef FUNCTIONS2_H
#define FUNCTIONS2_H

int threshold(cv::Mat &src, cv::Mat &dst);

int morphological(cv::Mat &src, cv::Mat &dst);

int conn_comp(cv::Mat &src, cv::Mat &dst);

int moments(cv::Mat &src, cv::Mat &dst);

int features(cv::Mat &src, cv::Mat &dst, char *argv);

#endif