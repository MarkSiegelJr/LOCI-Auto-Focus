
/*
entropyFunction1.cpp
Date:06/12/2018
Author: Mark Siegel
Origional entropy from: ????????
*/

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//takes an image as input and returns the image's average entropy
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

//Simple test of findEntropy
int main(int argc, char** argv)
{
	//image reading
	Mat image;
	image = imread(argv[1], 1);

	//image and argument checks
	if (!image.data)
	{
		printf("No image data \n");
		return -1;
	}
	if (argc != 2)
	{
		printf("usage: DisplayImage.out <Image_Path>\n");
		return -1;
	}

	cout<<"Entropy of Image: "<<findEntropy(image)<<endl;
	return 0;
}



/*NOT YET CODED CORRECTLY
//testing findEntropy in a while loop to find focus in set of images
int main(int argc, char** argv)
{
	//tracker to notify loop if focus found
	int tracker = 0;

	//entropy variables
	float entropy = 0;
	float prevEntropy = 0;

	//for varying input strings of main, whose command line argument is ../data/image0   the program will ad the #.tif as appropriate.
	int z = 0;
	char imageName[200];
	imageName = strcat_s(argv[1], "0.tif");

	while (tracker!=2) {
		//image reading
		Mat image;
		image = imread(argv[1], 1);

		//image and argument checks
		if (!image.data) {
			printf("No image data \n");
			return -1;
		}
		if (argc != 2) {
			printf("usage: DisplayImage.out <Image_Path>\n");
			return -1;
		}

		entropy = findEntropy(??????????);

		if (entropy < prevEntropy) {//passed by focus
			tracker++;
		}
		
		if (tracker == 0) {//not yet past focus, try next
			++z;
			prevEntropy = entropy;
			Aquire new image at(x, y, z);
		}

		else if (focusTracker == 1) {//passed over the focus once
			--z;
			prevEntropy = entropy;
			Aquire new image at(x, y, z);
		}
	}
	return 0;
}

*/
