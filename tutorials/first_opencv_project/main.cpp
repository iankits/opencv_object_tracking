#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main( int argc, char** argv )
{
  Mat image;
  
  if( argc < 2 ){
    printf( "Error! No image data \n" );
    return -1;
  }
  
  image = imread( argv[1], 1 );
  
  namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
  imshow( "Display Image", image );
  
  waitKey(0);
  
  return 0;
}
/*
#include <iostream>
#include </opt/local/include/opencv2/	core/core.hpp>
#include </opt/local/include/opencv2/highgui/highgui.hpp>

int main() {
	char var;
    cv::Mat img;
    img = cv::imread("IMG_0612.jpg");
    cv::namedWindow("Image");
    cv::imshow("Image", img);
    std::cin >> var;
    return 1;
}

//
// main.m
// OpenCVTest
//
// Created by __MyName__ on 16/09/11.
// Copyright 2011 __MyCompanyName__. All rights reserved.
//
//
#import <Foundation/Foundation.h>
#import "cv.h"
#import "highgui.h"

int main (int argc, const char * argv[])
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
    IplImage* img = cvCreateImage( cvSize( 640, 480 ), IPL_DEPTH_8U, 3 );
    
    cvCircle( img, cvPoint( 320, 240 ), 100, cvScalar( 255, 0, 0 , 255 ), 5, 8, 0 );
    
    cvNamedWindow( "OpenCV Window", CV_WINDOW_NORMAL );
    cvShowImage( "OpenCV Window", img );
    
    cvWaitKey(0);
    
    cvDestroyWindow( "OpenCV Window" );
    cvReleaseImage( &img );
    
    [pool drain];
    return 0;
}*/
