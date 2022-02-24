# Project 1: Real-time image filtering system

https://wiki.khoury.northeastern.edu/display/~eilccn/CS5330+Project+1+Wiki

### Instructions for running executables

1. Place all files from the "vid_display" directory (filter.cpp, filter.h, vidDisplay.cpp) into a directory on your own OS (i.e. assume it's called "proj1") <br />
2. You may download the CMake compiler then add your own CMakeLists.txt file to your directory for compiling OR you may opt to use a different compiling method.
3. Create a directory within your "proj1" directory called "build." <br />
4. If using CMake, while inside the "build" directory enter "cmake .." in the command line followed by "make." <br />
5. You should now be able to run the code by entering "./project1" in the command line.

### Description

This project is a C/C++ and OpenCV implementation of various real-time image filtering functions. The program consists of the following files: "vidDisplay.cpp," "filter.cpp," "filter.h," as well as a "CMakeLists.txt" file that aids in code compilation using CMake. The "vidDisplay.cpp" file implements the main function, which opens a live video channel, creates a window, and displays a new frame through a loop. If the user types 'q,' the program will exit out of the loop that keeps the video capture open and the entire program will terminate. Additional keypress conditionals are implemented within the video capture loop, and they include 's' for saving images as .png files, and other keypresses for calling each filter function in the "filter.cpp" executable file. Aside from the first greyscale filter implementation that used the built-in OpenCV cvtColor() function, the rest of the filter functions manipulated individual pixels stored in the cv::Mat matrix format. In doing so, the filter functions had to assign the proper size and data types to each cv::Mat object that would then specify the correct color channel depth and the number of color channels. Each pixel is manipulated via various operations, such as convolution via separable x and y kernels, calculating the Euclidian distance for magnitude, and color quantization. 

### Filtered Image Sample Outputs

#### Original Image

![Original](image_samples/lamp0.png)

#### cvtColor() Greyscale

![Greyscale](image_samples/lamp1.png)

#### Average Greyscale
![Avg Greyscale](image_samples/lamp2.png)

#### Gaussian Blur
![Blur](image_samples/lamp3.png)

#### Sobel X
![Sobel X](image_samples/lamp4.png)

#### Sobel Y
![Sobel Y](image_samples/lamp5.png)

#### Gradient Magnitude
![Gradient](image_samples/lamp6.png)

#### Blur and Quantize
![Blur and Quantize](image_samples/lamp7.png)

#### Cartoonized
![Cartoon](image_samples/lamp8.png)

Cartoon video clip: https://drive.google.com/file/d/1VH6HbUszX0352z7_PlvPN_wfrOIJg-UF/view?usp=sharing

#### Custom
![Custom](image_samples/lamp9.png)

