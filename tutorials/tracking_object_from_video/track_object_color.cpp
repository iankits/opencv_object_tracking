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
 *   
 *
 * @Usage:
 *  For compiling and creating executable file and running the executable
 *   $ ./make_prj
 *   $ ./trackObjectByColor sample_773-10.mp4 
 *  For cleaning files created during cmake and make command.
 *   $ . cmake_clean
 *=====================================================================*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;

IplImage* imgTracking;
int lastX = -1;
int lastY = -1;

//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV){       
  IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	//cvInRangeS(imgHSV, cvScalar(10, 100, 100), cvScalar(50, 150,120), imgThresh);// WORKING LITTLE
  cvInRangeS(imgHSV, cvScalar(20, 100, 120), cvScalar(30, 170,140), imgThresh); 
  //cvInRangeS(imgHSV, CV_RGB(180, 0, 0), CV_RGB(255, 0,0), imgThresh);
  return imgThresh;
}

void trackObject(IplImage* imgThresh){
  // Calculate the moments of 'imgThresh'
  CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
  cvMoments(imgThresh, moments, 1);
  double moment10 = cvGetSpatialMoment(moments, 1, 0);
  double moment01 = cvGetSpatialMoment(moments, 0, 1);
  double area = cvGetCentralMoment(moments, 0, 0);
  
  // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
  if(area>1000){
    // calculate the position of the ball
    int posX = moment10/area;
    int posY = moment01/area;        
    
    if(lastX>=0 && lastY>=0 && posX>=0 && posY>=0)
      {
	// Draw a yellow line from the previous point to the current point
	cvLine(imgTracking, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0,0,255), 4);
      }
    
    lastX = posX;
    lastY = posY;
  }
  
  free(moments); 
}

int main( int argc, char** argv ){
  
  if( argc < 2 ){
    printf( "\n Error! No video data!!! \n" );
    return -1;
  }
  
  //CvCapture* capture = 0;       
  //capture = cvCaptureFromCAM(0); // OR
   
  CvCapture* capture = cvCreateFileCapture( argv[1] );
  
  if(!capture){
    printf("Capture failure\n");
     return -1;
  }
  
  IplImage* frame=0;
  frame = cvQueryFrame(capture);           
  if(!frame) return -1;
  
  //create a blank image and assigned to 'imgTracking' which has the same size of original video
  imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
  cvZero(imgTracking); //covert the image, 'imgTracking' to black
  
  cvNamedWindow("Video");     
  cvNamedWindow("ObjectTracking");
  
  while(1) {

    frame = cvQueryFrame(capture);           
    if(!frame) break;
    frame=cvCloneImage(frame); 
    
    cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel
    
    IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
    cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
    IplImage* imgThresh = GetThresholdedImage(imgHSV);
    
    cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
    
    //track the possition of the object
    trackObject(imgThresh);
    
    // Add the tracking image and the frame
    cvAdd(frame, imgTracking, frame);
    
    cvShowImage("ObjectTracking", imgThresh);           
    cvShowImage("Video", frame);
    
    //Clean up used images
    cvReleaseImage(&imgHSV);
    cvReleaseImage(&imgThresh);            
    cvReleaseImage(&frame);
    
    //Wait 10mS
    int c = cvWaitKey(10);
    //If 'ESC' is pressed, break the loop
    if((char)c==27 ) break;      
  }
  
  cvDestroyAllWindows() ;
  cvReleaseImage(&imgTracking);
  cvReleaseCapture(&capture);   
      
}

