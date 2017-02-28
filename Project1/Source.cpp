#define _CRT_SECURE_NO_DEPRECATE

#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#include<iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat src, src_gray, frame, src_2;
Mat dst, detected_edges, dstt;
VideoCapture video("C:/Users/∂¿©s¿M/Desktop/a.avi");
VideoCapture camera(0);

int edgeTresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Demo";
//canny ==edge detection
void CannyThreshold(int, void*) {
	blur(src_gray, detected_edges, Size(3, 3));
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);
	
	
}



int main(int argc, char ** argv) {
	//check whether camera exist or not
	if (!camera.isOpened()) {
		cout << "Cannot open the video" << endl;
		return -1;
	}
	// check can video open
	if (!video.isOpened()) {
		cout << "Cannot open the video" << endl;
		return -1;
	}


	
	for (;;) {
		//get camera frame
		camera >> src_2;
		//resize to match window
		resize(src_2, src, Size(0.4*src_2.cols, 0.4*src_2.rows), 2, 2, CV_INTER_LANCZOS4);
		dst.create(src.size(), src.type());

		cvtColor(src, src_gray, CV_BGR2GRAY);

		namedWindow(window_name, CV_WINDOW_AUTOSIZE);
		video.read(frame);
		//blend
		if (frame.rows > 0) {
			char c = cvWaitKey(16);
			createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
			CannyThreshold(0, 0);
			addWeighted(src, (float)lowThreshold / (float)max_lowThreshold, dst, 1 - (float)lowThreshold / (float)max_lowThreshold, 0, dst);
			Mat imgROI = frame(Rect(30, 100, dst.cols, dst.rows));
			addWeighted(imgROI, 0.5, dst, 0.5, 0, imgROI);
			imshow(window_name, frame);
			if (c == 27)break;//press button ese to quit

		}
		else break;
		waitKey(30);
	}
	return 0;
}