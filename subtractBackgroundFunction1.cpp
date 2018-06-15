/*
subtractBackgroundFunction.cpp :
Date:06/12/2018
Author: Mark Siegel
Origional background subtract from: ????????
*/


#include "stdafx.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <cstring>
//for runtime analysis
#include <chrono>

using namespace cv;
using namespace std;
//for runtime analysis
using namespace std::chrono;


//function that takes a baackground image and subtracts it from another image, returning the result
Mat subtractBackground(Mat backgroundImage,Mat rawImage) {

	//storing image dimensions
	int nChannels = rawImage.channels();
	int nRows = rawImage.rows;
	int nCols = rawImage.cols;


	//find mean value of backgroundImage
	long meanBackgroundValue = 0;
	for (int c = 0; c < nChannels; c++) {
		for (int y = 0; y < nRows; y++) {
			for (int x = 0; x < nCols; x++) {
				meanBackgroundValue = meanBackgroundValue + backgroundImage.at<Vec3b>(y, x)[c];
			}
		}
	}
	meanBackgroundValue /= nRows*nCols*nChannels;
	
	//subtracting that value from raw image
	for (int c = 0; c < nChannels; c++) {
		for (int y = 0; y < nRows; y++) {
			for (int x = 0; x < nCols; x++) {
				rawImage.at<Vec3b>(y, x)[c] = (int)((float)rawImage.at<Vec3b>(y, x)[c] / (float)backgroundImage.at<Vec3b>(y, x)[c] * (float)meanBackgroundValue);
			}
		}
	}
	return rawImage;
}

//simple check to see if subtractBackground works, displaying the background image, raw image, and resulting image.
/*
int main(int argc, char** argv)
{

	//checking arguments
	if (argc != 3) {
		printf("Correct Usage is DisplayImage.out <BackgroundImage_Path> <Raw_Image_Path>");
		return -1;
	}
	Mat backgroundImage = imread(argv[1], 1);
	Mat rawImage = imread(argv[2], 1);
	if (!rawImage.data || !backgroundImage.data) {
		printf("No image data in either backgroundImage or rawImage");
		return -1;
	}
	//error check- both images have same dimensions
	if (backgroundImage.channels() != rawImage.channels() || backgroundImage.rows != rawImage.rows || backgroundImage.cols != rawImage.cols) {
		printf("raw and background images have different dimensions");
		return -1;
	}

	//displaying images
	namedWindow("Background Image", WINDOW_AUTOSIZE);
	imshow("Background Image", backgroundImage);
	waitKey(0);
	namedWindow("Raw Image", WINDOW_AUTOSIZE);
	imshow("Raw Image", rawImage);
	waitKey(0);

	//result
	namedWindow("Raw Image", WINDOW_AUTOSIZE);
	imshow("Raw Image", subtractBackground(backgroundImage,rawImage));
	waitKey(0);

	return 0;
}
*/

//measuring runtime of function
int main(int argc, char** argv) {
	//checking arguments
	if (argc != 3) {
		printf("Correct Usage is DisplayImage.out <BackgroundImage_Path> <Raw_Image_Path>");
		return -1;
	}
	Mat backgroundImage = imread(argv[1], 1);
	Mat rawImage = imread(argv[2], 1);
	if (!rawImage.data || !backgroundImage.data) {
		printf("No image data in either backgroundImage or rawImage");
		return -1;
	}
	//error check- both images have same dimensions
	if (backgroundImage.channels() != rawImage.channels() || backgroundImage.rows != rawImage.rows || backgroundImage.cols != rawImage.cols) {
		printf("raw and background images have different dimensions");
		return -1;
	}


	int average = 0;
	int n = 100;
	for (int i = 0; i < n; i++) {
		//executing function and finding its runtime
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		subtractBackground(backgroundImage, rawImage);
		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(t2 - t1).count();
		average += duration;

		cout << i << "Runtime (microseconds): " << duration << endl;
	}
	cout << endl << "Average Runtime (microseconds): " << average / (float)n << endl;

	return 0;
}
