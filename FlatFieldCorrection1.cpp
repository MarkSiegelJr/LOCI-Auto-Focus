/*
First Outline of field flattness correction algorith for LOCI's CAMM
Date: 06/7/2018
Author: Mark Siegel
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

int main(int argc, char** argv) {
	int focusTracker = 0;
	//coordinates might have to be something other than an int
	int x = 0;
	int y = 0;
	int z = 0;
	float entropy = 0;
	float prevEntropy = 0;
	//the two bellow will most likely come from inputs
	int xmax = xmax;
	int ymax = ymax;

	while (y >= ymax) {

		Aquire Image at(x, y, z);

			if (no tissue present) {//*****need way of determining if there is tissue present or if it is just a background image.
				Skip focusing process;//*****may still have to subtract background. Maybe through that we can figure out if there is any tissue or not.
			}

			else if (tissue present) {

				while (focusTracker != 2) {

					//implemented outside functions
					entropy = findEntropy(subtractBackground(image));

					if (entropy < prevEntropy) {//passed by focus
						focusTracker++;
					}

					if (focusTracker == 0) {//not yet past focus, try next
						++z;
						prevEntropy = entropy;
						Aquire new image at(x, y, z);
					}

					else if (focusTracker == 1) {//passed over the focus once, so reversing directions
						--z;
						prevEntropy = entropy;
						Aquire new image at(x, y, z);
					}

					else {//focusTracker = 2, meaning the algorithm just passed over the focus again
						use/save previous image taken at(x, y, z + 1);
						//*****depending on how images are saved, could potentially just use previously taken image
						//(but not the image that was JUST taken) rather than aquiring a new one
					}

					if (x = xmax) {
						x = 0;
						++y;
						z = wherever it started;
					}
					//*****could potentially avoid by snaking around the images rather than going through them left to right
					else {
						x++;
						/* z is left wherever it was, since it is likely that the next focus point is nearby
						(focusTracker incrementing twice prevents the scan from missing the focus entirely,
						regardless of strting position of initial scan direction) */
					}
				}//inner while
			}//else if
	}//outer while
	return 0;
}//main end
