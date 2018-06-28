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

//Zstack results
//Z0 (Lighter tissue)
/*
standard tissue sample:
cropZ0stack00: 13.2066
cropZ0stack01: 19.4303
cropZ0stack02: 19.4847
cropZ0stack03: 19.7324
cropZ0stack04: 19.8148
cropZ0stack05: 20.0906
cropZ0stack06: 20.3944
cropZ0stack07: 20.7912
cropZ0stack08: 21.5671
cropZ0stack09: 22.4029
cropZ0stack10: 24.1757
cropZ0stack11: 27.1792
cropZ0stack12: 32.2415
cropZ0stack13: 40.908
cropZ0stack14: 56.0073
cropZ0stack15: 78.5822
cropZ0stack16: 103.506
cropZ0stack17: 125.394
cropZ0stack18: 143.047
cropZ0stack19: 158.213
cropZ0stack20: 166.896
cropZ0stack21: 150.275
cropZ0stack22: 133.126
cropZ0stack23: 97.9335
cropZ0stack24: 65.3778
cropZ0stack25: 45.3174
cropZ0stack26: 34.5974
cropZ0stack27: 28.6601
cropZ0stack28: 25.4428
cropZ0stack29: 23.4881
cropZ0stack30: 22.2707
cropZ0stack31: 21.5944
cropZ0stack32: 21.073
cropZ0stack33: 20.7998
cropZ0stack34: 20.5443
cropZ0stack35: 20.4002
cropZ0stack36: 20.2595
cropZ0stack37: 20.1343
cropZ0stack38: 20.0804
cropZ0stack39: 20.0778
cropZ0stack40: 20.0011
*/
//Z1 (Darker tissue)
/*
Darker tissue sample:
cropZ1stack00: 17.3691
cropZ1stack01: 17.4525
cropZ1stack02: 17.4916
cropZ1stack03: 17.6182
cropZ1stack04: 17.7585
cropZ1stack05: 17.9718
cropZ1stack06: 18.3302
cropZ1stack07: 18.7516
cropZ1stack08: 19.3605
cropZ1stack09: 20.3255
cropZ1stack10: 21.777
cropZ1stack11: 24.2901
cropZ1stack12: 28.8159
cropZ1stack13: 36.2201
cropZ1stack14: 47.8046
cropZ1stack15: 66.0581
cropZ1stack16: 85.329
cropZ1stack17: 102.7
cropZ1stack18: 127.978
cropZ1stack19: 154.031
cropZ1stack20: 160.179
cropZ1stack21: 138.366
cropZ1stack22: 117.997
cropZ1stack23: 89.5027
cropZ1stack24: 60.1281
cropZ1stack25: 42.6751
cropZ1stack26: 32.0871
cropZ1stack27: 26.8252
cropZ1stack28: 23.8067
cropZ1stack29: 21.9573
cropZ1stack30: 20.7465
cropZ1stack31: 19.9635
cropZ1stack32: 19.4005
cropZ1stack33: 19.0073
cropZ1stack34: 18.7169
cropZ1stack35: 18.5762
cropZ1stack36:??????
cropZ1stack37: 18.2453
cropZ1stack38: 18.1947
cropZ1stack39: 18.1871
cropZ1stack40:??????
*/
//Z2 (background)
/*
Background sample:
cropZ2stack00: 20.6857
cropZ2stack01: 20.682
cropZ2stack02: 20.6312
cropZ2stack03: 20.7138
cropZ2stack04: 20.7495
cropZ2stack05: 20.7188
cropZ2stack06: 20.6917
cropZ2stack07: 20.6897
cropZ2stack08: 20.7243
cropZ2stack09: 20.7425
cropZ2stack10: 20.7488
cropZ2stack11: 20.8662
cropZ2stack12: 20.7929
cropZ2stack13: 20.9723
cropZ2stack14: 21.3432
cropZ2stack15: 21.6234
cropZ2stack16: 21.9879
cropZ2stack17: 22.3981
cropZ2stack18: 21.8869
cropZ2stack19: 22.0772
cropZ2stack20: 21.8076
cropZ2stack21: 21.3162
cropZ2stack22: 21.1328
cropZ2stack23: 26.6361
cropZ2stack24: 20.9627
cropZ2stack25: 20.9906
cropZ2stack26: 21.1033
cropZ2stack27: 21.0324
cropZ2stack28: 21.0386
cropZ2stack29: 21.0486
cropZ2stack30: 21.0386
cropZ2stack31: 21.0368
cropZ2stack32: 21.0183
cropZ2stack33: 21.1077
cropZ2stack34: 21.0605
cropZ2stack35: 21.1492
cropZ2stack36: 21.1478
cropZ2stack37: 21.1199
cropZ2stack38: 21.1117
cropZ2stack39: 21.2057
cropZ2stack40: 21.1764
*/

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

(same background used for all images)
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
