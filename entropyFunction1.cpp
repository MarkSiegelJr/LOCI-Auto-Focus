/*
entropyFunction1.cpp
Date:06/12/2018
Author: Mark Siegel
Original entropy from: ????????
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
///*
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
//*/

//cutoffTest## Results:
/*
00 48.1716
01 30.6868
02 29.2786
03 22.7195
04 18.4103
05 28.85
06 22.7571
07 16.5804
08 16.475
09 15.6547
10 18.114
11 23.5276
12 18.8169
13 17.2822
*/

//testing with subtract background
/*
int main(int argc, char** argv)
{
	//image reading
	Mat image;
	image = imread(argv[2], 1);
	Mat background;
	background = imread(argv[1], 1);

	//cout << "Entropy of Background: " << findEntropy(background) << endl;
	cout << "Entropy of Image:      " << findEntropy(image) << endl;
	cout << "Entropy of Result:     " << findEntropy(subtractBackground(background,image)) << endl;
	return 0;
}
*/

//Results of findEntropy(subtrackBackground)
/*
legend:
background image: entropy
original image:   entropy
resulting image:  entropy

background00: 6.28376
background00: 6.28376
result:       0

background00: 6.28376
image00:      20.4747
result:       25.2315

background00: 6.28376
image04:      64.6141
result:       68.9084

background00: 6.28376
image07:      82.5003
result:       86.7486

background00: 6.28376
image14:      31.643
result:       37.1223

background00: 6.28376
image20:      8.94446
result:       14.8064

cutoffTest00: 48.1716
cutoffTest00: 48.1716
result:       0

cutoffTest00: 48.1716
cutoffTest01: 30.6868
result:       47.2187

cutoffTest01: 30.6868
cutoffTest00: 48.1716
result:       54.9901

cutoffTest00: 48.1716
cutoffTest07: 16.5804
result:       30.6706

cutoffTest00: 48.1716
cutoffTest09: 15.6547
result:       28.8237

cutoffTest01: 30.6868
cutoffTest07: 16.5804
result:       28.7018

cutoffTest01: 30.6868
cutoffTest09: 15.6547
result:       26.2868
*/
//Original images with & without background subtraction
/*
legend:
original image:   entropy
resulting image:  entropy

background image: background00.tif: 6.28376

image00: 20.4747
result:  25.2315

image01: 29.3561
result:  34.0584

image02: 40.4896
result:  45.1828

image03: 52.7272
result:  57.3462

image04: 64.6141
result:  68.9084

image05: 72.6338
result:  76.7338

image06: 79.6326
result:  83.798

image07: 82.5003
result:  86.7486

image08: 81.2747
result:  85.6169

image09: 76.3242
result:  80.7395

image10: 67.2039
result:  71.6985

image11: 57.8407
result:  62.5267

image12: 48.0982
result:  53.1118

image13: 39.7218
result:  45.0434

image14: 31.643
result:  37.1223

image15: 24.8767
result:  30.537

image16: 19.406
result:  25.2182

image17: 15.3199
result:  21.1988

image18: 12.4066
result:  18.3536

image19: 10.3512
result:  16.2406

image20: 8.94446
result:  14.8064
*/
//Still in same order as before. image07 is still the focus

//measuring runtime of function
/*
int main(int argc, char** argv) {
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
	int average = 0;
	int n = 100;
	for (int i = 0; i < n; i++) {
		//executing function and finding its runtime
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		float entropy = findEntropy(image);
		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(t2 - t1).count();
		average += duration;
		

		//cout << "Entropy of Image: " << entropy << endl;
		cout << i << "Runtime (microseconds): " << duration << endl;
	}
	cout << endl << "Average Runtime (microseconds): " << average / (float)n << endl;

	return 0;
}
*/


//testing findEntropy in a while loop to find focus in set of images
/*NOT YET CODED CORRECTLY
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
