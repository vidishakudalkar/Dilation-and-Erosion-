
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;


int min(int list[]) {
	int min = 255;
	for (int i = 0; i < sizeof(list); i++) {
		if (list[i] < min) {
			min = list[i];
		}
	}
	return min;
}

int max(int list[]) {
	int max = 0;
	for (int i = 0; i < sizeof(list); i++) {
		if (list[i] > max) {
			max = list[i];
		}
	}
	return max;
}

int get_Threshold(int intensityfrequency[], int threshold, int newthreshold)
{
	if (newthreshold == threshold)
	{
		return threshold;
	}
	else
	{
		float region1 = 0, region2 = 0, region1mean = 0, region2mean = 0, region1sum = 0, region2sum = 0;

		for (int i = 0; i < threshold; i++)
			region1 += intensityfrequency[i];

		for (int i = int(threshold + 1); i <= 255; i++)
			region2 += intensityfrequency[i];

		for (int i = 0; i < threshold; i++)
			region1sum += i*intensityfrequency[i];

		region1mean = region1sum / region1;

		for (int i = int(threshold + 1); i <= 255; i++)
			region2sum += i * intensityfrequency[i];

		region2mean = region2sum / region2;

		newthreshold = (region1mean + region2mean) / 2;

		if (newthreshold == threshold)
			return threshold;

		else
		{
			threshold = newthreshold;
			newthreshold = 0;
		}

		return get_Threshold(intensityfrequency, threshold, newthreshold);
	}
}
// Maximum Size of Structuring element is set to 15*15
void MorphologicalErosion(Mat binaryImage, int structuringelementlength) {

	int structuringelementerosion[15][15];
	for (int x = 0; x < structuringelementlength; x++) {
		for (int y = 0; y < structuringelementlength; y++) {
			structuringelementerosion[x][y] = 255;
		}
	}

	Mat tempbinaryimage = binaryImage.clone();

	for (int i = 0; i < binaryImage.rows; i++) {
		for (int j = 0; j < binaryImage.cols; j++) {
			binaryImage.at<uchar>(i, j) = 0;
		}
	}


	for (int i = structuringelementlength / 2; i < binaryImage.rows - structuringelementlength / 2; i++) {
		for (int j = structuringelementlength / 2; j < binaryImage.cols - structuringelementlength / 2; j++) {
			int min = 255, flag = 0;
			for (int x = -structuringelementlength / 2; x <= structuringelementlength / 2; x++) {

				for (int y = -structuringelementlength / 2; y <= structuringelementlength / 2; y++) {

					if (structuringelementerosion[x + structuringelementlength / 2][y + structuringelementlength / 2] == tempbinaryimage.at<uchar>(i + x, j + y)) {

					}
					else { flag = 1; }

				}

			}
			if (flag == 0)
				binaryImage.at<uchar>(i, j) = min;

		}

	}


}
// Maximum Size of Structuring element is set to 15*15
void MorphologicalDilation(Mat binaryImage, int structuringelementlength) {
	Mat ImageOnDilation;
	ImageOnDilation = binaryImage.clone();
	int structuringelement[15][15];
	for (int i = 0; i < structuringelementlength; i++) {
		for (int j = 0; j < structuringelementlength; j++) {
			structuringelement[i][j] = 255;
		}
	}
	for (int i = 0; i < binaryImage.rows; i++) {
		for (int j = 0; j < binaryImage.cols; j++) {
			binaryImage.at<uchar>(i, j) = 0;
		}
	}


	for (int i = structuringelementlength / 2; i < binaryImage.rows - structuringelementlength / 2; i++) {
		for (int j = structuringelementlength / 2; j < binaryImage.cols - structuringelementlength / 2; j++) {
			int max = 255, flag = 0;
			for (int x = -structuringelementlength / 2; x <= structuringelementlength / 2; x++) {


				for (int y = -structuringelementlength / 2; y <= structuringelementlength / 2; y++) {

					if (structuringelement[x + structuringelementlength / 2][y + structuringelementlength / 2] == ImageOnDilation.at<uchar>(i + x, j + y)) {
						flag = 1;
					}

				}

			}
			if (flag == 1)
				binaryImage.at<uchar>(i, j) = max;

		}

	}

}


void MorphologicalErosionOnGrayScaleImage(Mat GrayScaleImage) {
	int structuringelement[3][3];
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			structuringelement[x][y] = 1;
		}
	}
	int list[10];

	cv::Mat ImageOnErosion(GrayScaleImage.size(), GrayScaleImage.type());
	for (int i = 1; i < GrayScaleImage.rows - 1; i++) {
		for (int j = 1; j < GrayScaleImage.cols - 1; j++) {
			int  n = 0;
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					list[n] = GrayScaleImage.at<uchar>(i + x, j + y)*structuringelement[x + 1][y + 1];
					n++;
				}
			}
			ImageOnErosion.at<uchar>(i, j) = min(list);
		}
	}
	namedWindow("Gray Scale Eroded Image", CV_WINDOW_AUTOSIZE);
	imshow("Gray Scale Eroded Image", ImageOnErosion);

}

void MorphologicalDilationOnGrayScaleImage(Mat GrayScaleImage) {

	int structuringelement[3][3];
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			structuringelement[x][y] = 1;
		}
	}
	int list[10];

	cv::Mat ImageOnDilation(GrayScaleImage.size(), GrayScaleImage.type());
	for (int i = 1; i < GrayScaleImage.rows - 2; i++) {
		for (int j = 1; j < GrayScaleImage.cols - 2; j++) {
			int  n = 0;
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					list[n] = GrayScaleImage.at<uchar>(i + x, j + y)*structuringelement[x + 1][y + 1];
					n++;
				}
			}
			ImageOnDilation.at<uchar>(i, j) = max(list);
		}
	}
	namedWindow("Gray Scale Dilated Image", CV_WINDOW_AUTOSIZE);
	imshow("Gray Scale Dilated Image", ImageOnDilation);

}

int main()
{
	Mat image = imread("TestImage.bmp", CV_LOAD_IMAGE_GRAYSCALE);


	cv::Mat binaryImage(image.size(), image.type());
	cv::Mat GrayScaleErosionImage(image.size(), image.type());
	cv::Mat GrayScaleDilationImage(image.size(), image.type());


	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) < 190) {
				binaryImage.at<uchar>(i, j) = 0;
			}
			else binaryImage.at<uchar>(i, j) = 255;
		}
	}

	namedWindow("Gray Scale Image", CV_WINDOW_AUTOSIZE);
	imshow("Gray Scale Image", image);
	namedWindow("Binary Image", CV_WINDOW_AUTOSIZE);
	imshow("Binary Image", binaryImage);


	GrayScaleErosionImage = image.clone();
	MorphologicalErosionOnGrayScaleImage(GrayScaleErosionImage);

	GrayScaleDilationImage = image.clone();
	MorphologicalDilationOnGrayScaleImage(GrayScaleDilationImage);

	MorphologicalDilation(binaryImage, 3);
	MorphologicalErosion(binaryImage, 15);
	MorphologicalErosion(binaryImage, 15);
	MorphologicalErosion(binaryImage, 3);
	MorphologicalDilation(binaryImage, 15);
	MorphologicalDilation(binaryImage, 15);


	namedWindow("Binary Image after MorphologicalErosion MorphologicalDilation Iterations", CV_WINDOW_AUTOSIZE);
	imshow("Binary Image after MorphologicalErosion MorphologicalDilation Iterations", binaryImage);
	waitKey(0);

	return 0;
}