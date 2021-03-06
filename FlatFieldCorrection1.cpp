/*
First Outline of field flattness correction algorith for LOCI's CAMM
Date: 06/7/2018
Author: Mark Siegel
Original Entropy and Background Subtraction from: ????????
*/

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//function to find entropy
float findEntropy(Mat image) {

	//channels and rows and columns
	int nChannels = image.channels();
	int nRows = image.rows;
	int nCols = image.cols;

	long entropy = 0, fx, fy;
	for (int y = 1; y < image.rows - 1; y++)
	{
		for (int x = 1; x < image.cols - 1; x++)
		{
			for (int c = 0; c < nChannels; c++)
			{
				fx = (2 * (long)image.at<Vec3b>(y, x)[c] - (long)image.at<Vec3b>(y, x - 1)[c] - (long)image.at<Vec3b>(y, x + 1)[c]);
				fy = (2 * (long)image.at<Vec3b>(y, x)[c] - (long)image.at<Vec3b>(y - 1, x)[c] - (long)image.at<Vec3b>(y + 1, x)[c]);
				entropy = entropy + fx*fx + fy*fy;
			}
		}
	}
	float avg_entropy = (float)entropy / (nRows*nCols);
	//for testing
	//cout << avg_entropy << endl;
	return avg_entropy;
}

//function to subtract background
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

//function to detect tissue
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


	//Current Inputs: Starting X, Y, and Z. Final X and Y. Background Image.
	//Images in question being taken within the code, not as imputs.

//*****most likely, the code described bellow will just be transcribed into the Wiscan system,
//*****not actually putting the literal code below in as a function
int main(int argc, char** argv) {
	int focusTracker = 0;
	//coordinates might have to be something other than an int, and will most likely come from inputs
	int X = starting coordinates;
	int Y = starting coordinates;
	int Z = starting coordinates;
	float entropy = 0;
	float prevEntropy = 0;
	int xmax = xmax;
	int ymax = ymax;
	Mat background;//background to be subtracted from everything

	while (Y >= ymax) {

		Mat image = Aquire Image at(X, Y, Z);

		//implemented outside function
		Mat newImage = subtractBackground(image);

		//implementing outside funciton to detect whether or not there is tissue present/
		if (tissueDetector(newImage)==false) {
			use newImage;
		}

		else { //there is tissue present in the image
			   
			//implemented outside function
			entropy = findEntropy(newImage);

			while (focusTracker != 2) {

				if (entropy < prevEntropy) {//passed by focus
					focusTracker++;
				}

				if (focusTracker == 0) {//not yet past focus, try next
					++Z;
					prevEntropy = entropy;
					Aquire new image at(X, Y, Z);

					//implemented at end of loop so the first entropy won't be taken twice
					entropy = findEntropy(subtractBackground(background, image));
				}

				else if (focusTracker == 1) {//passed over the focus once, so reversing directions
					--Z;
					prevEntropy = entropy;
					Aquire new image at(X, Y, Z);

					//implemented at end of loop so the first entropy won't be taken twice
					entropy = findEntropy(subtractBackground(background, image));
				}

				//loop's last iteration
				else {//focusTracker = 2, meaning the algorithm just passed over the focus again
					use/save previous image taken at(X, Y, Z + 1);
					//*****depending on how images are saved, could potentially just use previously taken image
					//(but not the image that was JUST taken) rather than aquiring a new one
				
					if (X = xmax) {
						X = 0;
						++Y;
						Z = wherever it started;
						//*****could potentially avoid by snaking around the 
						//images rather than going through them left to right
					}
				
					else {
						X++;
						/* z is left wherever it was, since it is likely that the next focus point is nearby

						(focusTracker incrementing twice prevents the scan from missing the focus entirely,
						regardless of strting position of initial scan direction) */
					}
				}//else focusTracker = 2
			}//inner while
		}//else tissue present
	}//outer while
	return 0;
}//main
