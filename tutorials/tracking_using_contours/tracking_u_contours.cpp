#include<cv.h>
#include<highgui.h>
//Input-Output
#include<stdio.h>
#include <iostream>
#include <Carbon/Carbon.h>
//Definitions
//#define h 240
//#define w 320
#define h 480
#define w 640
//NameSpaces
using namespace std;

//Global variables
int fcount=0;//Counts the number of frames

//Main Function
int main( int argc, char** argv){
  //Function prototypes
  void findxy(IplImage*,int*,int*); //Function to find X and Y
  double detectangle(IplImage*,IplImage*,double,IplImage*,IplImage*);//Angle measure
  double findvelocity(IplImage*);//Velocity measure
  void changepicture(IplImage*,IplImage*,IplImage*,IplImage*,double);
  CvCapture* capture;

  if(argc < 2){
    printf( "\n SYNTAX ERROR! \n" );
     printf( "\n Options- 0: Camera & 1: Manual Input \n" );
     printf( "\n\t USAGE: \t  ./track_contour <OPTIONS> \n\n\t\t\t  or\n" );
     printf( "\n\t\t\t  ./track_contour <OPTIONS> <VIDEO> \n\n" );
     return -1;
  }
    
  if(atoi(argv[1]) == 1){
    if( argc < 3 ){
      printf( "\n Error! No video data!!! \n" );
      printf( "\n\t USAGE: \t  ./track_contour 1 <VIDEO> \n\n" );
      return -1;
    }
    capture = cvCreateFileCapture( argv[2] );
  } else if(atoi(argv[1]) == 0) {
    //Structure to get feed from CAM
    capture=cvCreateCameraCapture(0);
  } else {
    printf( "\n UNKNOWN OPTION! ERROR.... EXITING... \n" );
    printf( "\n Options- 0: Camera & 1: Manual Input \n" );
    printf( "\n\t USAGE: \t  ./track_contour <OPTIONS> <VIDEO> \n\n" );
    return -1;
  }

 if(!capture){
      printf("Capture failure\n");
      return -1;
    } 
  // Get the main display and capture it. 
  //CGDirectDisplayID gCGDisplayID = CGMainDisplayID();
 
  //Windows
  cvNamedWindow("Live",CV_WINDOW_AUTOSIZE);
  cvNamedWindow("Threshy",CV_WINDOW_AUTOSIZE);
  cvNamedWindow("cnt",CV_WINDOW_AUTOSIZE);
  
  //Image Variables
  IplImage *frame=cvCreateImage(cvSize(w,h),8,3); //Original Image
  IplImage *hsvframe=cvCreateImage(cvSize(w,h),8,3);//Image in HSV color space
  IplImage *threshy=cvCreateImage(cvSize(w,h),8,1); //Threshold image of yellow color
  
  //Variables for trackbars
  int h1=23;int s1=41;int v1=133;
  int h2=40;int s2=150;int v2=255;
 
 //Creating the trackbars
 cvCreateTrackbar("H1","cnt",&h1,255,0);
 cvCreateTrackbar("H2","cnt",&h2,255,0);
 cvCreateTrackbar("S1","cnt",&s1,255,0);
 cvCreateTrackbar("S2","cnt",&s2,255,0);
 cvCreateTrackbar("V1","cnt",&v1,255,0);
 cvCreateTrackbar("V2","cnt",&v2,255,0);
 
 //Infinate Loop
 while(1)
   {
     
     //Getting the current frame
     IplImage *fram=cvQueryFrame(capture);
     //If failed to get break the loop
     if(!fram)
       break;
     
     //1.PREPROCESSING OF FRAME
     //Resizing the capture
     cvResize(fram,frame,CV_INTER_LINEAR );
     //Flipping the frame
     cvFlip(frame,frame,1);
     //Changing the color space
     cvCvtColor(frame,hsvframe,CV_BGR2HSV);
     //Thresholding the frame for yellow
     cvInRangeS(hsvframe,cvScalar(h1,s1,v1),cvScalar(h2,s2,v2),threshy);
     //Filtering the frame
     cvSmooth(threshy,threshy,CV_MEDIAN,7,7);
     //Getting the screen information
     int screenx = 480;
     int screeny = 640;
     //int screenx = CGDisplayPixelsWide(gCGDisplayID);
     // int screeny = CGDisplayPixelsHigh(gCGDisplayID);
     //int screenx = GetSystemMetrics(SM_CXSCREEN);
     //int screeny = GetSystemMetrics(SM_CYSCREEN);
     //Calculating the moments
     CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
     cvMoments(threshy, moments, 1);
     // The actual moment values
     double moment10 = cvGetSpatialMoment(moments, 1, 0);
     double moment01 = cvGetSpatialMoment(moments, 0, 1);
     double area = cvGetCentralMoment(moments, 0, 0);
     //Position Variables
     int x1;
     int y1;
     //Calculating the current position
     x1 = moment10/area;
     y1 = moment01/area;
     //Fitting to the screen
     int x=(int)(x1*screenx/w);
     int y=(int)(y1*screeny/h);
     if(x>>0 and y>>0 )
       {
	 cvLine(frame, cvPoint(x1,y1), cvPoint(x1,y1), cvScalar(0,25,255),5);
	 cout<<"X:"<<x<<"\tY:"<<y<<endl;
       }
     
     //Showing the images
     cvShowImage("Live",frame);
     cvShowImage("Threshy",threshy);
     //Escape Sequence
     char c=cvWaitKey(33);
     if(c==27)
       break;
     
   }
 //Cleanup
 cvReleaseCapture(&capture);
 cvDestroyAllWindows();
 
}
