/*======================================================================
 * A small program for counting total number of frames in a video file
 * and displaying some the properties of the video.
 *
 * Author: Ankit Singh (ankitsingh.05@gmail.com)
 *
 * @Input:
 *   Video file
 *
 * @Output:
 *   Video Properties like Frame height, width and number of frames etc.
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
  int posFrame = 0, videoLengthHour, videoLengthMin, videoLengthSec;
  bool status;
  double frame_width, frame_height, totFrames, fps; 
  
  frame_width = _videoSource.get(CV_CAP_PROP_FRAME_WIDTH);
  frame_height = _videoSource.get(CV_CAP_PROP_FRAME_HEIGHT);
  fps = _videoSource.get(CV_CAP_PROP_FPS);
  totFrames = _videoSource.get(CV_CAP_PROP_FRAME_COUNT);
  // TODO: Write function to calculate time
  videoLengthHour = (int) totFrames / (fps * 3600);
  videoLengthMin = (int) totFrames / (fps * 60);
  videoLengthSec = (int) ((int)(totFrames / fps) % 60);
  

  std::cout << "\n Video Properties:\n" << "\tFrame Width: " << frame_width << "\n\tFrame Height: " << frame_height;
  std::cout << "\n\tfps: " << fps << "\n\tTotal Frames: " << totFrames << "\n\tTotal Length: "<< videoLengthHour << ":"<< videoLengthMin << ":" << videoLengthSec;
  frames_stream << "\n";
  while(1) {
    // checking for end of the video
    status  = _videoSource.read(frame);      
    //checking the existence of image to capture
    if( !status ){
      std::cout << "\n\tVideo ENDS at Frame Number: " << posFrame << "\n";
      break;
    }
    
    // Grab the Frames
    posFrame=_videoSource.get(CV_CAP_PROP_POS_FRAMES);
    imshow("Frame Capture", frame);
    
    frames_stream << posFrame << " "; // writing frame number to the file
    int c = cvWaitKey(15);    
    //If 'ESC' is pressed, break the loop
    if((char)c==27 ){ 
      std::cout << "\n\n\tVideo EXITED at Frame Number: " << posFrame;
      videoLengthHour = (int) posFrame / (fps * 3600);
      videoLengthMin = (int) posFrame / (fps * 60);
      videoLengthSec = (int) ((int)(posFrame / fps) % 60);
      std::cout << "\n\tVideo EXITED at Time: " << videoLengthHour << ":"<< videoLengthMin << ":" << videoLengthSec << "\n\n";
      frames_stream << "\n";
      break;
    }  
  } 
  
  frames_stream << "\n";
  frames_stream.close();
}
