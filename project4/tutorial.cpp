/*
Eileen Chang
CS5330 
Spring 2022

Tutorial Notes
*/

int tutorial() {
    /*
    cv::Size patternsize( 8, 6 ); // what size chessboard to use
    std::vector<cv::Point2f> corners; //image points found by findChessboardCorners
    std::vector<cv::Vec3f> points; //3D world points, constructed
    std::vector<std::vector<cv::Point2f> > cornerlist;
    std::vector<std::vector<cv::Vec3f> > pointList; // >> is a special symbol in C++ so need space
    */
   
    // looping

    // grab a frame
    // run findChessboardCorners 
    // if it finds something 
    //      use cornerSubPix to get more accurate locations
    //      drawChessboardCorners
    //      saved this point set to std::vector lastcorners
    //      saved this frame to another cv::mat

    // check for a kepress

    // case 's': grab the lastcorners and write out the corresponding image
    //      add lastcorners to the cornerlist
    //          cornerList.push_back(std::vector<cv::Point2f>( lastcorners ) );
    //      generate a new std::vector of 3D ponts
    //          for patternsize.height
    //              for patternsize.width
    //                  generate the 3D point associated with this corner
    //                  3D point for (i,j) is (x,y,z) = (j,-1,0)
    //      add 3D points to the pointList
    //          pointList.push_back(std::vector<cv::Vec3f>( points ) );

    // case 'c': call the calibration function passing in cornerList and pointList

    // case 'w': writes the intrinsic parameters to a file
    //      choose a file format that is easy to read

    // ar
    //      read calibration data
    //      read chessboard with solvepnp
    //          output extrinsic parameters - create 3D points and map to pixel in camera, draw lines btwn those points to get 3D shapes
    


    return 0;
}