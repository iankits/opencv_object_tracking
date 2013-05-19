/*
* Program to detect Turtle using Haar Training
* 
* USAGE: ./turtledetect <video>
*/

#include<stdio.h>
#include<cv.h>
#include<highgui.h>

CvHaarClassifierCascade *cascade;
CvMemStorage *Membuffer;

void detectturtle(IplImage *frame) {
  int i;
  CvSeq *object=cvHaarDetectObjects(frame,cascade,Membuffer,1.1,3,0,cvSize(25,25));
  for(i=0;i<(object?object->total:0);i++)
    {
      CvRect *r=(CvRect *)cvGetSeqElem(object,i);
      cvRectangle(frame,cvPoint(r->x,r->y),cvPoint(r->x+r->width,r->y+r->height),CV_RGB(255,0,0),1,8,0);
      //Draws a rectangle with two points given with color of red and thickness as 8 and line type - 1
    }
  
  cvShowImage("Video",frame);  
}


int main(int argc,char *argv[]){
  IplImage *frame;
  // CvCapture *capture;
  int key;
  
  char filename[]="haarcascade-inter.xml";
  
  if( argc < 2 ){
    printf( "\n Error! No video data!!! \n" );
    return -1;
  }
  
  cascade=(CvHaarClassifierCascade *)cvLoad(filename,0,0,0);
  
  Membuffer=cvCreateMemStorage(0);//Allocating the default memory for image (64K at Present)
    
  //CvCapture* capture = 0;       
  //capture = cvCaptureFromCAM(0); // OR
  
  CvCapture* capture = cvCreateFileCapture( argv[1] );
  
  if(!capture){
    printf("Capture failure\n");
    return -1;
    }
  
  assert(cascade && Membuffer && capture);//To make sure that everything needed is always available
  cvNamedWindow("Video",1);
  
  while(key!='q')
    
    {frame=cvQueryFrame(capture);
      if(!frame)
	break;
      
      //cvFlip(frame,frame,0);
      frame->origin=0;

      detectturtle(frame);
      key=cvWaitKey(5);//Wait for 10 milli seconds, the user may press 'q'
    }
  cvReleaseCapture(&capture);
  cvDestroyWindow("Video");
  cvReleaseHaarClassifierCascade(&cascade);
  cvReleaseMemStorage(&Membuffer);
}












