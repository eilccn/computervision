/*
  Eileen Chang
  
  Code to read image files in a directory, compute their features, and write their features to a csv file
*/
#include "csv_util.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

/*
  Given a directory on the command line, scans through the directory for image files.
  Reads the full path name for each file.  This can be used as an argument to fopen or to cv::imread.
 */
int process_directory(int argc, char *argv[]) {
  char dirname[256];
  char buffer[256];
  FILE *fp;
  DIR *dirp;
  struct dirent *dp;
  int i;

  // check for sufficient arguments
  if( argc < 2) {
    printf("usage: %s <directory path>\n", argv[0]);
    exit(-1);
  }

  // get the directory path
  strcpy(dirname, argv[1]);
  printf("Processing directory %s\n", dirname );

  // open the directory
  dirp = opendir( dirname );
  if( dirp == NULL) {
    printf("Cannot open directory %s\n", dirname);
    exit(-1);
  }

  // loop over all the files in the image file listing
  while( (dp = readdir(dirp)) != NULL ) {

    // check if the file is an image
    if( strstr(dp->d_name, ".jpg") ||
	strstr(dp->d_name, ".png") ||
	strstr(dp->d_name, ".ppm") ||
	strstr(dp->d_name, ".tif") ) {

      printf("processing image file: %s\n", dp->d_name);

      // build the overall filename
      strcpy(buffer, dirname);
      strcat(buffer, "/");
      strcat(buffer, dp->d_name);

      img = cv::imread(buffer, cv::IMREAD_COLOR);
      if( img.data == NULL ) {
        printf("Unable to read file %s, skipping\n", buffer );
        continue;
      }

    }
  }


  return(0);
}