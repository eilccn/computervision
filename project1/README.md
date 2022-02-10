# Project 1: Real-time image filtering system

• Links/URLs to videos: https://drive.google.com/file/d/1VH6HbUszX0352z7_PlvPN_wfrOIJg-UF/view?usp=sharing 

• Project Wiki URL: https://wiki.khoury.northeastern.edu/x/GxQdBg

• Operating system and IDE used to run and compile code: Mac OS, terminal, cmake, and tmux

• Instructions for running executables: <br /> 
1. Place all files (filter.cpp, filter.h, vidDisplay.cpp, CMakeLists.txt)  into one directory (i.e. assume it's called "project1") <br />
2. Create a directory within project1 called "build". <br />
3. Inside the build directory type "cmake .." in command line followed by "make". <br />
4. You can now run the code by typing "./project1" in the command line.

### Description

This project is a C/C++ and OpenCV implementation of various real-time image filtering functions. The program consists of the following files: "vidDisplay.cpp," "filter.cpp," "filter.h," as well as a "CMakeLists.txt" file that aids in code compilation using CMake. The "vidDisplay.cpp" file implements our main function, which opens a live video channel, creates a window, and displays a new frame through a loop. If the user types 'q,' the program will exit out of the loop that keeps the video capture open and the entire program will terminate. Additional keypress conditionals are implemented within the video capture loop, and they include 's' for saving images as .png files, and other keypresses for calling each filter function in the "filter.cpp" executable file. Aside from the first greyscale filter implementation that used the built-in OpenCV cvtColor() function, the rest of the filter functions manipulated individual pixels stored in the cv::Mat matrix format. In doing so, the filter functions had to assign the proper size and data types to each cv::Mat object that would then specify the correct color channel depth and the number of color channels. We altered the pixels through operations, such as convolution via separable x and y kernels, calculating the Euclidian distance for magnitude, and color quantization. 

### Filtered Image Outputs

#### cvtColor() Greyscale

![Greyscale](/Users/eileenchang/Downloads/cs5330/proj1 final images/lamp.000.png)

#### Average Greyscale

#### Gaussian Blur

#### Sobel X

#### Sobel Y

#### Gradient Magnitude

#### Blur and Quantize

#### Cartoonized

#### Custom
