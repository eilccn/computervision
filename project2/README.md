# Project 2: Content-based image retrieval
### Project Description ###
The URL for wiki report for this project: 
https://wiki.khoury.northeastern.edu/x/mYR5Bg

Content-based image retrieval, when applied to computer vision, searches through image databases by image content as opposed to associated text. In this project, we are analyzing and manipulating images at the pixel level in order to match images via pattern recognition. The images were analyzed based on color, texture, and spatial layout. While implementing the program, we had to consider color spaces, histograms, spatial features, and texture features.

The overall program is a command line program that takes in the following arguments:

[argv0]: ./project2 
[argv1]: target image path
[argv2]: directory path
[argv3]: csv file path
[argv4]: feature type (b, h, m, t)
[argv5]: matching method (ssd, histx, multix, tcx)
[argv6]: # images N to return

The overall flow of the program is that it computes the features of a target image, computes the features of each directory image, computes the distance between the target image and each directory image according to a distance metric, sorts the directory images according to their distance from the target image, and returns the top N matches. When using distance metrics, the smaller the distance between two images, the more similar they will be. The matching methods I implemented include sum square difference, histogram intersection, and histogram intersection with weighted averaging to combine the distances between the different histograms. The feature types include a 9x9 center crop of the image, an rg histogram, a multi-histogram (the first is the rg histogram of the center 9x9 crop and the second is the rg histogram of the whole image), a texture/color histogram (the texture histogram is grayscale gradient magnitude and the color is an rg histogram), and a custom built feature type that only collects the green values from an rg histogram.  

Instructions for running  executables:
1. Place all .cpp and .h files along with a CMakeLists.txt file into a directory (i.e. called "project2")
2. Open "project2" in Visual Studio Code
3. Build "project2" using the "CMake Tools" extension
4. Run the program by first entering "./project2 " followed by the required command line arguments (please read the main file "target.cpp" for what arguments need to be passed)
5. Note: Every time you use a new feature type, new data needs to be written to a csv file. There is no implementation for clearing the csv file so the outputfile needs to be manually deleted and recreated.

