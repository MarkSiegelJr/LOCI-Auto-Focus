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

int main(int argc, char** argv)
{
	//image reading
	Mat image;
	image = imread(argv[1], 1);

	double imageSize = image.cols*image.rows;
	cout << "Image size: " << imageSize << endl;

	vector<Mat> splitImage;
	split(image, splitImage);

	//method 1:
	Mat blue, green, red;
	inRange(splitImage[2], Scalar(180), Scalar(299), blue);
	inRange(splitImage[1], Scalar(60), Scalar(179), green);
	inRange(splitImage[0], Scalar(0), Scalar(59), red);

	cout << "((double)countNonZero(blue)) " << ((double)countNonZero(blue)) << endl;
	cout << "((double)countNonZero(green)) " << ((double)countNonZero(green)) << endl;
	cout << "((double)countNonZero(red)) " << ((double)countNonZero(red)) << endl;

	double bluePercent = 100.0*((double)countNonZero(blue))/imageSize;
	double greenPercent = 100.0*((double)countNonZero(green))/imageSize;
	double redPercent = 100.0*((double)countNonZero(red))/imageSize;

	//method 2:
	/*
	cout << "((double)countNonZero(splitImage[2])) " << ((double)countNonZero(splitImage[0])) << endl;
	cout << "((double)countNonZero(splitImage[1])) " << ((double)countNonZero(splitImage[1])) << endl;
	cout << "((double)countNonZero(splitImage[0])) " << ((double)countNonZero(splitImage[2])) << endl;

	double bluePercent = 100.0*((double)countNonZero(splitImage[2]))/imageSize;
	double greenPercent = 100.0*((double)countNonZero(splitImage[1]))/imageSize;
	double redPercent = 100.0*((double)countNonZero(splitImage[0]))/imageSize;
	*/


	cout << "Image is " << bluePercent << "% blue." << endl;
	cout << "Image is " << greenPercent << "% green." << endl;
	cout << "Image is " << redPercent << "% red." << endl;

    return 0;
}

