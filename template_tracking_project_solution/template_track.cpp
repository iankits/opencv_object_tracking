/*========================================================================
 * This program tracks any object with minimal user interaction provided
 * the template (picture of the object).
 *
 * Author: Ankit Singh (ankitsingh.05@gmail.com)
 *
 * User Interaction Required:
 *  Users has to change between methods using trackbar. The best methods
 *  for tracking are
 *
 *     method=CV_TM_CCOEFF or method=CV_TM_CCOEFF_NORMED
 *  
 *  And advised to switch between these two methods only
 *
 * @Input:
 *   Video file & template of turtle (can be found in "test_templates/")
 *
 * @Output:
 *   Prints Video Properties in console like Frame height, width and 
 *   number of frames etc.
 *   And produces text file called "objectTrackingInfo.txt" which contains
 *   tracking infos of the object like framenumber, x, y and timestamp
 *
 * @Usage:
 * For compiling and creating executable file and running the executable
 *   $ ./make_prj
 *   $ ./templateTrack <video> <templatePic> 
 * e.g. 
 *   $ ./templateTrack sample_773-10.mp4 test_templates/ssp7.png 
 * For cleaning files created during cmake and make command.
 *   $ . cmake_clean
 *=======================================================================*/

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;

Mat frame, templ, result;
char* image_window = "Object Tracking Window";
char* result_win = "Result Window";

int match_method;
int max_Trackbar = 5;
int posFrame = 0, videoLengthHour, videoLengthMin, videoLengthSec;
int prev_pos_x, prev_pos_y; 
// threshold to check whether the object is moving
int move_threshold = 20;
double fps; //frames per second

std::ofstream frames_stream("objectTrackingInfo.txt");

/// Function Headers
void MatchingMethod( int, void* );
void drawObject(int x, int y, Mat &frame);
string intToString(int number);
void print_object_tracking_info(int x, int y);
bool check_object_movement(int x, int y);

int main( int argc, char** argv ){
  
  VideoCapture videoSource; 
  
  if( argc < 3 ){
    printf( "\n Error! No video data and template!!! \n" );
    printf( "\n\t USAGE: \t  ./templateTrack <VIDEO> <TEMPLATE> \n\n" );
    return -1;
  }
  
  if(!videoSource.open(argv[1])){
    exit(1);         // Exit if fail
  }
  
  frames_stream << "frameNumber" << "\tx" << "\t\ty" << "\t\tTimestamp\n";

  videoSource.set(CV_CAP_PROP_CONVERT_RGB, 1);
 
  bool status;
  double frame_width, frame_height, totFrames; 
  
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

    /// Create windows
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_win, CV_WINDOW_AUTOSIZE ); 

    /// Create Trackbar
    char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    // char* trackbar_label = "Method: \n 0: TM COEFF \n 1: TM COEFF NORMED";
    createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

    MatchingMethod( 0, 0 );
    
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
  frames_stream << "\n";
  frames_stream.close();
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
  /// Source image to display
  Mat img_display;
  int adjust_center = 60;
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

  // Draw crosshair on the tracking object
  drawObject(matchLoc.x+adjust_center,matchLoc.y+adjust_center,img_display);

  // rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  
  imshow( image_window, img_display );
  imshow( result_win, result );
  
  print_object_tracking_info(matchLoc.x+adjust_center, matchLoc.y+adjust_center);

  return;
}

// Write all tracking info of objects to a file
void print_object_tracking_info(int x, int y){
  bool move_stat = false;
  videoLengthHour = (int) posFrame / (fps * 3600);
  videoLengthMin = (int) posFrame / (fps * 60);
  videoLengthSec = (int) ((int)(posFrame / fps) % 60);
  // check for object movement
  move_stat = check_object_movement(x, y);
  
  if(move_stat)
    frames_stream << "\n" << "The Object Moves from Frame: " << posFrame <<" at time: "<< videoLengthHour << ":"<< videoLengthMin << ":" << videoLengthSec;
  
  frames_stream << "\n" << posFrame <<"\t\t"<< x <<"\t\t" << y <<"\t\t" << videoLengthHour << ":"<< videoLengthMin << ":" << videoLengthSec;
  // Saving previous position which can be used to check movement of the object
  prev_pos_x = x;
  prev_pos_y = y;
}

// Check whether object moved from its current place or not
bool check_object_movement(int x, int y){
  // Decision making whether object is moving or not.
  int tempx, tempy;
  bool obj_move = false;
  tempx = x - prev_pos_x;
  tempy = y - prev_pos_y;
    
    if(tempx > 0 && tempx > move_threshold){
      obj_move = true;
    } else if (tempx < 0) {
      tempx = tempx + (-1); // making it positive
      if(tempx > move_threshold)
	obj_move = true;
    } else if (tempy > 0 && tempy > move_threshold) {
      obj_move = true;
    } else if  (tempy < 0) {
      tempy = tempy + (-1);
      if(tempx > move_threshold)
	obj_move = true;
    } else
      obj_move = false;
    
  return obj_move; 
}

// Convert an integer to string
string intToString(int number){
	std::stringstream ss;
	ss << number;
	return ss.str();
}

// draws a crosshair on the tracked object
void drawObject(int x, int y,Mat &frame){

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!
	circle(frame,Point(x,y),20,Scalar(0,255,0),2);
	line(frame,Point(x,y-5),Point(x,y-25),Scalar(0,255,0),2);
	line(frame,Point(x,y+5),Point(x,y+25),Scalar(0,255,0),2);
	line(frame,Point(x-5,y),Point(x-25,y),Scalar(0,255,0),2);
	line(frame,Point(x+5,y),Point(x+25,y),Scalar(0,255,0),2);

	putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);
}
