#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int alt_greyscale( cv::Mat &src, cv::Mat &dst) {
            dst.create(src.size(), src.type());
            for (int i = 0; i < src.rows; i++)
                        for (int j = 0; j < src.cols; j++)
                                    dst.at<uchar>(i, j) = ((src.at<cv::Vec3b>(i, j)[0] + src.at<cv::Vec3b>(i, j)[1] + src.at<cv::Vec3b>(i, j)[2]) / 3);
            return 0;
}
