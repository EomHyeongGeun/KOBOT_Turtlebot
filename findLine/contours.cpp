
#ifdef _DEBUG 

#pragma comment (lib, "opencv_world320d.lib") 

#else 

#pragma comment (lib, "opencv_world320.lib")

#endif


#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <unistd.h>
#include "linefinder.h"
#include "edgedetector.h"
 
#define PI 3.1415926
 
int main()
{
    // Read input image
    //cv::Mat image= cv::imread("road.jpg",0);
	
	cv::Mat image;

	cv::VideoCapture cap1(0);
    //if (!image.data)
    //    return 0; 
 	
	if (!cap1.isOpened()) {

		std::cerr << "ERROR! Unable to open camera1\n";

		return -1;

	}
	
	for(;;){
		
		cap1.read(image);
		//sleep(10);
	
    // Display the image
    cv::namedWindow("Original Image");
	
 //   	cv::imshow("Original Image",image);
 	
    // Compute Sobel
    EdgeDetector ed;
	if(!image.empty())
	{
	    ed.computeSobel(image);
	} 
    // Apply Canny algorithm
    //cv::Mat contours;
    //cv::Canny(image,contours,125,350);
 
    // Create LineFinder instance
    //LineFinder ld;
 
    // Set probabilistic Hough parameters
    //ld.setLineLengthAndGap(100,20);
    //ld.setMinVote(60);
 
    // Detect lines
    //std::vector<cv::Vec4i> li= ld.findLines(contours);
 
    // eliminate inconsistent lines
   // ld.removeLinesOfInconsistentOrientations(ed.getOrientation(),0.4,0.1);
 


    //image= cv::imread("road.jpg");
 
    //ld.drawDetectedLines(image);
    cv::namedWindow("Detected Lines (2)");

	if(!image.empty())
	{
    	cv::imshow("Detected Lines (2)",image);
 	}
    cv::waitKey();

	}
    return 0;
}
