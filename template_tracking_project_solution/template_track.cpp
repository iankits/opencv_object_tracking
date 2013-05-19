#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;

Mat frame, templ, result;
char* image_window = "Frame Capture";
char* result_win = "Result Window";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );

int main( int argc, char** argv ){
  
  VideoCapture videoSource;
  //std::ofstream frames_stream("numberOfFrames.txt");
  
  if( argc < 2 ){
    printf( "\n Error! No video data!!! \n" );
    return -1;
  }
  
  if(!videoSource.open(argv[1])){
    exit(1);         // Exit if fail
  }
  
  videoSource.set(CV_CAP_PROP_CONVERT_RGB, 1);
  
  int posFrame = 0, videoLengthHour, videoLengthMin, videoLengthSec;
  bool status;
  double frame_width, frame_height, totFrames, fps; 
  
  frame_width = videoSource.get(CV_CAP_PROP_FRAME_WIDTH);
  frame_height = videoSource.get(CV_CAP_PROP_FRAME_HEIGHT);
  fps = videoSource.get(CV_CAP_PROP_FPS);
  totFrames = videoSource.get(CV_CAP_PROP_FRAME_COUNT);
  // TODO: Write function to calculate time
  videoLengthHour = (int) totFrames / (fps * 3600);
  videoLengthMin = (int) totFrames / (fps * 60);
  videoLengthSec = (int) ((int)(totFrames / fps) % 60);
  

  std::cout << "\n Video Properties:\n" << "\tFrame Width: " << frame_width << "\n\tFrame Height: " << frame_height;
  std::cout << "\n\tfps: " << fps << "\n\tTotal Frames: " << totFrames << "\n\tTotal Length: "<< videoLengthHour << ":"<< videoLengthMin << ":" << videoLengthSec;
  //frames_stream << "\n";
  while(1) {
    // checking for end of the video
    status  = videoSource.read(frame);   
    templ = imread( argv[2], 1 );
    //checking the existence of image to capture
    if( !status ){
      std::cout << "\n\tVideo ENDS at Frame Number: " << posFrame << "\n";
      break;
    }
    
    // Grab the Frames
    posFrame=videoSource.get(CV_CAP_PROP_POS_FRAMES);
    // imshow(image_window, frame);

    /// Create windows
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_win, CV_WINDOW_AUTOSIZE ); 

    /// Create Trackbar
    char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    // char* trackbar_label = "Method: \n 0: TM COEFF \n 1: TM COEFF NORMED";
    createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

    MatchingMethod( 0, 0 );
    
    //frames_stream << posFrame << " "; // writing frame number to the file
    int c = cvWaitKey(5);    
    //If 'ESC' is pressed, break the loop
    if((char)c==27 ){ 
      std::cout << "\n\n\tVideo EXITED at Frame Number: " << posFrame;
      videoLengthHour = (int) posFrame / (fps * 3600);
      videoLengthMin = (int) posFrame / (fps * 60);
      videoLengthSec = (int) ((int)(posFrame / fps) % 60);
      std::cout << "\n\tVideo EXITED at Time: " << videoLengthHour << ":"<< videoLengthMin << ":" << videoLengthSec << "\n\n";
      //frames_stream << "\n";
      break;
    }  
  } 
  // frames_stream << "\n";
  //frames_stream.close();
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
  /// Source image to display
  Mat img_display;
  frame.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  frame.cols - templ.cols + 1;
  int result_rows = frame.rows - templ.rows + 1;

  result.create( result_cols, result_rows, CV_32FC1 );

  /// Do the Matching and Normalize
  matchTemplate( frame, templ, result, match_method );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }
  
  //matchLoc = maxLoc;
  
  /// Show me what you got
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  
  imshow( image_window, img_display );
  imshow( result_win, result );

  return;
}
