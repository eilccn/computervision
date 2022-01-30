#include <cstdio>
#include <opencv2/opencv.hpp>

int main()
{
	cv::VideoCapture video(0);

	if(!video.isOpened()) return -1;

	cv::Mat frame;

	while(video.read(frame))
	{
		cv::imshow("Video feed", frame);

		if (cv::waitKey(25) >= 0)
		{
			break;
		}
	}
}
