/*
colorDetection1.cpp
Date:06/21/2018
Author: Mark Siegel
*/

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
//for runtime analysis
#include <chrono>

using namespace cv;
using namespace std;
//for runtime analysis
using namespace std::chrono;


//for testing
//function that takes a baackground image and subtracts it from another image, returning the result
Mat subtractBackground(Mat backgroundImage, Mat rawImage) {

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


int main(int argc, char** argv)
{
	//for methdods 1 and 2
	/*
	//image reading
	Mat image;
	image = imread(argv[1], 1);

	double imageSize = image.cols*image.rows;
	cout << "Image size: " << imageSize << endl;

	vector<Mat> splitImage;
	split(image, splitImage);
	*/

	//method 1:
	/*
	Mat blue, green, red;
	inRange(splitImage[0], Scalar(75,150,60), Scalar(130,255,255), blue);
	inRange(splitImage[1], Scalar(38,150,60), Scalar(75,255,255), green);
	inRange(splitImage[2], Scalar(160,150,60), Scalar(179,255,255), red);

	imshow("Blue", splitImage[0]);
	waitKey(0);
	imshow("Green", splitImage[1]);
	waitKey(0);
	imshow("Red", splitImage[2]);
	waitKey(0);

	cout << "((double)countNonZero(blue)) " << ((double)countNonZero(blue)) << endl;
	cout << "((double)countNonZero(green)) " << ((double)countNonZero(green)) << endl;
	cout << "((double)countNonZero(red)) " << ((double)countNonZero(red)) << endl;

	double bluePercent = 100.0*((double)countNonZero(blue))/imageSize;
	double greenPercent = 100.0*((double)countNonZero(green))/imageSize;
	double redPercent = 100.0*((double)countNonZero(red))/imageSize;

	cout << "Image is " << bluePercent << "% blue." << endl;
	cout << "Image is " << greenPercent << "% green." << endl;
	cout << "Image is " << redPercent << "% red." << endl;
	*/

	//method 2:
	/*
	cout << "((double)countNonZero(splitImage[2])) " << ((double)countNonZero(splitImage[0])) << endl;
	cout << "((double)countNonZero(splitImage[1])) " << ((double)countNonZero(splitImage[1])) << endl;
	cout << "((double)countNonZero(splitImage[0])) " << ((double)countNonZero(splitImage[2])) << endl;

	double bluePercent = 100.0*((double)countNonZero(splitImage[2]))/imageSize;
	double greenPercent = 100.0*((double)countNonZero(splitImage[1]))/imageSize;
	double redPercent = 100.0*((double)countNonZero(splitImage[0]))/imageSize;

	cout << "Image is " << bluePercent << "% blue." << endl;
	cout << "Image is " << greenPercent << "% green." << endl;
	cout << "Image is " << redPercent << "% red." << endl;
	*/

	//method 3: (This one works!)
	///*
	//reads the bgr image
	Mat bgr = imread(argv[2],1);

	imshow("Tissue", bgr);
	waitKey();

	//converting the bgr image to an hsv for more convinient color based analysis
	Mat hsv;
	cvtColor(bgr, hsv, COLOR_BGR2HSV);

	//imshow("Tissue", hsv);
	//waitKey();

	//calculating image size
	double imageSize = hsv.cols*hsv.rows;
	cout << "Image size: " << imageSize << endl;

	//creating an image where tissue appears as white and anything else appears black
	//(based on the color of each pixel)
	Mat tissue;
	inRange(hsv, Scalar(130, 40, 50), Scalar(179, 255, 255), tissue);
	//Scalar parameters describe: Color hue from 0 to 180,
	//saturation (color mixed with white) from 0 to 255,
	//and value (color mized with black) from 0 to 255 respectively.

	imshow("Tissue", tissue);
	waitKey();
	
	//calculating the percent of the whole image teh tissue portion is
	double tissuePercent = 100.0*((double)countNonZero(tissue)) / imageSize;
	cout << "Image is " << tissuePercent << "% tissue." << endl;

    return 0;
}
//*/

	//method 3 with background subtract:
	/*
	Mat3b image = imread(argv[2],1);
	Mat3b background = imread(argv[1], 1);

	imshow("Tissue", background);
	waitKey(0);
	imshow("Tissue", image);
	waitKey(0);

	Mat3b bgr = subtractBackground(background, image);

	imshow("Tissue", bgr);
	waitKey();

	Mat3b hsv;
	cvtColor(bgr, hsv, COLOR_BGR2HSV);

	//imshow("Tissue", hsv);
	//waitKey();

	double imageSize = hsv.cols*hsv.rows;
	cout << "Image size: " << imageSize << endl;

	Mat1b tissue;
	inRange(hsv, Scalar(130, 55, 50), Scalar(179, 255, 255), tissue);

	imshow("Tissue", tissue);
	waitKey();

	double tissuePercent = 100.0*((double)countNonZero(tissue)) / imageSize;
	cout << "Image is " << tissuePercent << "% tissue." << endl;

	return 0;
	}
	*/
