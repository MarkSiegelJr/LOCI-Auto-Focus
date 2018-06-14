/*
Second Outline of field flattness correction algorith for LOCI's CAMM,
with the idea that images are processes outside my algorithm and all this
algorithm exports is instructions as to where the existing code should move
the microscope next.

Date: 06/14/2018
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


//Current Inputs: Starting X, Y, and Z. Final X and Y. Aquired Image.
//Previous Entropy value, which will have to be saved outsied this algorithm

int main(int argc, char** argv) {
	int focusTracker = 0;
	float entropy = 0;
	//below coordinates might have to be something other than an int, and will come from inputs
	Mat image;
	int X = starting coordinates;
	int Y = starting coordinates;
	int Z = starting coordinates;
	float prevEntropy = 0;
	int xmax = xmax;
	int ymax = ymax;

	while (Y >= ymax) {

		//implemented outside functions
		entropy = findEntropy(subtractBackground(image));

		//establishing an entropy cutoff point where below said point, you have skippable background images,
		//since background has much lower entropy than images with something in them
		if (entropy < 10) {//*****cutoff point subject to change
			use subtractBackground(image);
		}

		else { //there is tissue present in the image

			while (focusTracker != 2) {

				if (entropy < prevEntropy) {//passed by focus
					focusTracker++;
				}

				if (focusTracker == 0) {//not yet past focus, try next
					prevEntropy = entropy;
					insruction: Increase Z;//These will output instructions as to how microscope should move
									   //Currently left as psudocode until further information is obtained
				}

				else if (focusTracker == 1) {//passed over the focus once, so reversing directions
					prevEntropy = entropy;
					instruction: Decrease Z;
				}

				else {//focusTracker = 2, meaning the algorithm just passed over the focus again
					use / save previous image taken at(X, Y, Z + 1);
					//*****depending on how images are saved, could potentially just use previously taken image
					//(but not the image that was JUST taken) rather than aquiring a new one

					if (X = xmax) {
						instruction: Reset X, Increase Y, Reset Z;//*****could potentially avoid by snaking around the 
															  //images rather than going through them left to right
					}

					else {
						instruction: Increase X;
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
