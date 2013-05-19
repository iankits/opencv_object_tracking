/*======================================================================
 * Program to understand Historgram and Tracking object using it.
 *
 * Author: Ankit Singh (ankitsingh.05@gmail.com)
 *
 * @Input:
 *   Video file
 *
 * @Output:
 *
 * @Usage:
 * For compiling and creating executable file and running the executable
 *   $ ./make_prj
 *   $ ./histogramTut sample_773-10.mp4 
 * For cleaning files created during cmake and make command.
 *   $ . cmake_clean
 *=====================================================================*/

#include <cv.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char** argv ){
  
  if( argc < 2 ){
    printf( "\n Error! No video data!!! \n" );
    return -1;
  }
  /*  
  //CvCapture* capture = 0;       
  //capture = cvCaptureFromCAM(0); // OR
  
  CvCapture* capture = cvCreateFileCapture( argv[1] );
  
  if(!capture){
    printf("Capture failure\n");
    return -1;
    }*/
  
  IplImage* src;
  if( argc == 2 && (src=cvLoadImage(argv[1], 1))!= 0) {
    // Compute the HSV image and decompose it into separate planes.
    //
    IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
    cvCvtColor( src, hsv, CV_BGR2HSV );
    IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
    IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
    IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
    IplImage* planes[] = { h_plane, s_plane };
    cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
    // Build the histogram and compute its contents.
    //
    int h_bins = 30, s_bins = 32;
    CvHistogram* hist;
    {
      int hist_size[] = { h_bins, s_bins };
      float h_ranges[] = { 0, 180 }; // hue is [0,180]
      float s_ranges[] = { 0, 255 };
      float* ranges[] = { h_ranges, s_ranges };
      hist = cvCreateHist(
			  2,
			  hist_size,
			  CV_HIST_ARRAY,
			  ranges,
			  1
			  );
    }
    
    cvCalcHist( planes, hist, 0, 0 ); //Compute histogram
    cvNormalizeHist( hist, 1.0 ); //Normalize it
    // Create an image to use to visualize our histogram.
    //
    int scale = 10;
    IplImage* hist_img = cvCreateImage(
				       cvSize( h_bins * scale, s_bins * scale ),
				       8,
				       3
				       );
    cvZero( hist_img );
    // populate our visualization with little gray squares.
    //
    float max_value = 0;
    cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
    for( int h = 0; h < h_bins; h++ ) {
      for( int s = 0; s < s_bins; s++ ) {
	
	float bin_val = cvQueryHistValue_2D( hist, h, s );
	int intensity = cvRound( bin_val * 255 / max_value );
	cvRectangle(
		    hist_img,
		    cvPoint( h*scale, s*scale ),
		    cvPoint( (h+1)*scale - 1, (s+1)*scale - 1),
CV_RGB(intensity,intensity,intensity),
		    CV_FILLED
		    );
      }
    }
    cvNamedWindow( "Source", 1 );
    cvShowImage( "Source", src );
    cvNamedWindow( "H-S Histogram", 1 );
    cvShowImage( "H-S Histogram", hist_img );
    cvWaitKey(0);
  }
}

/*
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
*/
