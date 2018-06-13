// subtrackBackground1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <cstring>

using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
	/*
	This function takes in a backgroundImage and a rawImage. Finds the mean value of the backgroundImage.
	Finds the field corrected image as = rawImage/backgroundImage*mean_value.
	The corrected image is saved as corrected_<raw_image_name>
	*/

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

	//displaying images
	namedWindow("Background Image", WINDOW_AUTOSIZE);
	imshow("Background Image", backgroundImage);
	//waitKey(0);
	namedWindow("Raw Image", WINDOW_AUTOSIZE);
	imshow("Raw Image", rawImage);
	waitKey(0);

	//error check- both images have same dimensions
	if (backgroundImage.channels() != rawImage.channels() || backgroundImage.rows != rawImage.rows || backgroundImage.cols != rawImage.cols) {
		printf("raw and background images have different dimensions");
		return -1;
	}

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
	printf("mean value=%li\n", meanBackgroundValue);
	for (int c = 0; c < nChannels; c++) {
		for (int y = 0; y < nRows; y++) {
			for (int x = 0; x < nCols; x++) {
				rawImage.at<Vec3b>(y, x)[c] = (int)((float)rawImage.at<Vec3b>(y, x)[c] / (float)backgroundImage.at<Vec3b>(y, x)[c] * (float)meanBackgroundValue);
			}
		}
	}
	namedWindow("Raw Image", WINDOW_AUTOSIZE);
	imshow("Raw Image", rawImage);
	waitKey(0);

	//Need to save the image too 
	//char correctedImagename[200];
	//strcpy_s(correctedImagename, "corrected_");
	//strcat_s(correctedImagename, argv[2]);
	//imwrite(correctedImagename, rawImage);
	imwrite("C:/Users/Mark/Documents/Visual Studio 2015/Projects/subtrackBackground1/data/corrected_image07.tif", rawImage);
	return 0;
}
