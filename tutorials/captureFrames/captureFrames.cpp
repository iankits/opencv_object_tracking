/*======================================================================
 * A small program for counting total number of frames in a video file
 * 
 * Author: Ankit Singh (ankitsingh.05@gmail.com)
 *
 * @Input:
 *   Video file
 *
 * @Output:
 *   Video Properties like Frame height, width and number of frames.
 *   And a Text File called "numberOfFrame.txt"
 *
 * @Usage:
 * For compiling and creating executable file and running the executable
 *   $ ./make_prj
 *   $ ./capture_frames sample_773-10.mp4 
 * For cleaning files created during cmake and make command.
 *   $ . cmake_clean
 *=====================================================================*/

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;

int main( int argc, char** argv ){
  
  VideoCapture _videoSource;
  std::ofstream frames_stream("numberOfFrames.txt");
  
  if( argc < 2 ){
    printf( "\n Error! No video data!!! \n" );
    return -1;
  }
  
  if(!_videoSource.open(argv[1])){
    exit(1);         // Exit if fail
  }
  
  _videoSource.set(CV_CAP_PROP_CONVERT_RGB, 1);
  
  Mat frame;
  int posFrame = 0;
  bool status;
  double frame_width, frame_height;
  
  frame_width = _videoSource.get(CV_CAP_PROP_FRAME_WIDTH);
  frame_height = _videoSource.get(CV_CAP_PROP_FRAME_HEIGHT);

  std::cout << "\n Video Properties:\n" << "\tFrame Width: " << frame_width << "\n\tFrame Height: " << frame_height;
  
  frames_stream << "\n";
  while(1) {
    // checking for end of the video
    status  = _videoSource.read(frame);      
    //checking the existence of image to capture
    if( !status ){
      std::cout << "\n\t Video ENDS at Frame Number: " << posFrame << "\n";
      break;
    }
    
    // Grab the Frames
    //_videoSource >> frame;
    posFrame=_videoSource.get(CV_CAP_PROP_POS_FRAMES);
    imshow("Frame Capture", frame);
    
    frames_stream << posFrame << " "; // writing frame number to the file
    int c = cvWaitKey(15);    
    //If 'ESC' is pressed, break the loop
    if((char)c==27 ){ 
      std::cout << "\n \tVideo EXITED at Frame Number: " << posFrame << "\n";
      break;
    }  
  } 
  
  frames_stream << "\n";
  frames_stream.close();
}
