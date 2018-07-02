/*
tissueDetectionFunction1.cpp
Date:06/25/2018
Author: Mark Siegel

colorDetection1.cpp implemented as a function, more easily transfered to other programs.
*/

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

bool tissueDetector(Mat bgrImage) {
	//converting bgr image to hsv image
	Mat hsvImage;
	cvtColor(bgrImage, hsvImage, COLOR_BGR2HSV);

	//calculaing image size
	double imageSize = hsvImage.cols*hsvImage.rows;

	//converting the image into a form where tissue appears white
	//and everything else appears black.
	Mat tissue;
	inRange(hsvImage, Scalar(130, 40, 50), Scalar(179, 255, 255), tissue);

	//Calculating percentage of image is white (is tissue)
	double tissuePercent = 100.0*((double)countNonZero(tissue)) / imageSize;

	//Background images will yeild less than 1 percent.
	if (tissuePercent < 1) {
		return false;
	}
	else {
		return true;
	}
}


int main(int argc, char** argv)
{
	
	Mat image = imread(argv[1], 1);

	
	cout << "Tissue present: ";
	if (tissueDetector(image) == true) {
		cout << "Yes" << endl;
	}

	else {
		cout << "No" << endl;
	}

    return 0;
}

