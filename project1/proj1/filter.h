#ifndef FILTER_H
#define FILTER_H

// function declaration

int alt_greyscale(cv::Mat &src, cv::Mat &dst);

int blur5x5(cv::Mat &src, cv::Mat &dst);

int sobelX3x3(cv::Mat &src, cv::Mat &dst);

int sobelY3x3(cv::Mat &src, cv::Mat &dst);

int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst);

#endif

