#include <iostream>
#include "opencv2/core/utility.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <algorithm>
#include <string>

#define WIDTH 381
#define HEIGHT 579

using namespace std;
using namespace cv;

//==================
//   Declarations
//==================
void Analysis(string name, Mat& anImg, bool save);
void CalcAndShowHist(string name, Mat &anImage, bool save);
void dftshift(cv::Mat& mag); //taken from "03_freq_domain_template.cpp" from BlackBoard!!!
void CalcAndShowFourierMag(string name, Mat &anImage, bool save);
void MedianFilter(Mat &anImage, Mat &res, int size); //size must be an odd number
void MaxFilter(Mat &anImage, Mat &res, int size);//size must be an odd number
void adaptiveMaxFilter(Mat &anImage, Mat &res, int maxsize);
void FredsAdaptiveMedian(Mat &anImage, Mat &res, int maxSize);
void adaptiveMedianFilter(Mat &anImage, Mat &res, int maxsize);
void gaussianBandReject(Mat& inputImg, Mat& outputImg, int bandWidth, int cutoff);
void ApplyFreqFilter(Mat &anImage, Mat &aFilter); //apply complex filter in the frequency domain


//=================
//   Definitions
//=================
void Analysis(string name, Mat& anImg, bool save)
{
    //show original image
    namedWindow("Image" + name, WINDOW_NORMAL);
    resizeWindow("Image" + name, WIDTH, HEIGHT);
    imshow("Image" + name, anImg);
    if (save)
	imwrite("Image" + name + ".png", anImg);

    CalcAndShowHist("Original" + name, anImg, save);

    Rect rect(825, 1424, 675, 384); //(corner_x, corner_y, length_x, length_y)
    Mat roi = anImg(rect);
    CalcAndShowHist("ROI" + name, roi, save);

    CalcAndShowFourierMag("Magnitude" + name, anImg, save);
}

void CalcAndShowHist(string name, Mat &anImage, bool save)
{
    //calculate histogram
    Mat hist;

    float range[] = {0, 256};
	const float* ranges = {range};
	int bins = 256;

    //(images, nimages, channels, mask, hist, dims, histSize, ranges, uniform, accumulate)
    calcHist(&anImage, 1, 0, Mat(), hist, 1, &bins, &ranges, true, false);

    //find the largest bin value
	int largestBinVal = 0;
	for (int i = 0; i < hist.rows; i++)
	{
		if (hist.at<float>(i, 0) > largestBinVal)
			largestBinVal = hist.at<float>(i, 0);
	}

    //create histogram image
    Mat histImg(HEIGHT, WIDTH, CV_8UC1, 255);

	for (int x = 0; x < histImg.cols; x++)
	{
		int bin = ((double)(hist.rows - 1) / (double)(histImg.cols - 1)) * x; // (bin / pix) * pix = bin
		int height = ((double)hist.at<float>(bin, 0) / (double)largestBinVal) * histImg.rows;

		for (int y = 0; y < histImg.rows; y++)
		{
			if (y < height)
				histImg.at<uchar>(histImg.rows - 1 - y, x) = 0; //draw from the bottom
		}
	}

    imshow(name, histImg);
    if (save)
	imwrite(name + ".png", histImg);
}

 //taken from "03_freq_domain_template.cpp" from BlackBoard!!!
void dftshift(cv::Mat& mag) //rearranges the quadrants of a Fourier image so that the origin is at the center of the image.
{
    int cx = mag.cols / 2;
    int cy = mag.rows / 2;

    cv::Mat tmp;
    cv::Mat q0(mag, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(mag, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(mag, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(mag, cv::Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void CalcAndShowFourierMag(string name, Mat &anImage, bool save)
{
    //calc optimal size
    int height = getOptimalDFTSize((2 * anImage.rows) - 1);
    int width = getOptimalDFTSize((2 * anImage.cols) - 1);

    //pad image equally on all edges to make it optimally sized
    Mat imgWithPad;
    copyMakeBorder(anImage, imgWithPad, (height - anImage.rows) / 2, (height - anImage.rows) / 2, (width - anImage.cols) / 2, (width - anImage.cols) / 2, BORDER_CONSTANT, 0);

    //merge
    Mat img2Channel;
    Mat temp[2] = {Mat_<float>(imgWithPad), (Mat_<float>(imgWithPad)) = 0};
    merge(temp, 2, img2Channel);

    //dft
    dft(img2Channel, img2Channel, DFT_COMPLEX_OUTPUT);

    //split
    split(img2Channel, temp);
    Mat mag;
    magnitude(temp[0], temp[1], mag);

    //prepare for visualization
    dftshift(mag);
    mag += 1;
    log(mag, mag);
    normalize(mag, mag, 0, 1, NORM_MINMAX);

    //visualize
    namedWindow(name, WINDOW_NORMAL);
    resizeWindow(name, WIDTH, HEIGHT);
    imshow(name, mag);

    if (save)
    {
	//gamma transformation (for visualization)
	mag.convertTo(mag, CV_8U, 255);
	for (int i = 0; i < mag.rows; i++)
	{
	    for (int j = 0; j < mag.cols; j++)
	    {
		mag.at<uchar>(i, j) = pow(mag.at<uchar>(i, j) / 255.0, 1.0) * 255.0;
	    }
	}

	imwrite(name + ".png", mag);
    }
}

void MedianFilter(Mat &anImage, Mat &res, int size, int numberFromList)
{
    anImage.copyTo(res);
    for (int i = 0; i < anImage.rows; i++)
    {
        for (int j = 0; j < anImage.cols; j++)
        {
            //find all neighbours
            vector<int> arr;
            for (int k = -size/2; k <= size/2; k++)
            {
                for (int l = -size/2; l < size/2; l++)
                {
                    if (i + k > 0 && i + k < anImage.rows && j + l > 0 && j + l < anImage.cols) //if not out of bounds
                        arr.push_back(anImage.at<uchar>(i + k, j + l));
                    else
                        arr.push_back(0); //padding value
                }
            }

            std::sort(arr.begin(), arr.end());

            res.at<uchar>(i, j) = arr[18];
        }
    }
}

void MaxFilter(Mat &anImage, Mat &res, int size)
{
    anImage.copyTo(res);
    for (int i = 0; i < anImage.rows; i++)
    {
        for (int j = 0; j < anImage.cols; j++)
        {
            //find all neighbours
            vector<int> arr;
            for (int k = -size/2; k <= size/2; k++)
            {
                for (int l = -size/2; l < size/2; l++)
                {
                    if (i + k > 0 && i + k < anImage.rows && j + l > 0 && j + l < anImage.cols) //if not out of bounds
                        arr.push_back(anImage.at<uchar>(i + k, j + l));
                    else
                        arr.push_back(0); //padding value
                }
            }

            std::sort(arr.begin(), arr.end());

            res.at<uchar>(i, j) = arr[arr.size()-1]; // take max value to remove pepper
        }
    }
}

void adaptiveMaxFilter(Mat &anImage, Mat &res, int maxsize)
{
    anImage.copyTo(res);
    for (int i = 0; i < anImage.rows; i++)
    {
        for (int j = 0; j < anImage.cols; j++)
        {
          for (int m = 3; m < maxsize; m++)
          {
            //find all neighbours
            vector<int> arr;
            for (int k = -m/2; k <= m/2; k++)
            {
              for (int l = -m/2; l < m/2; l++)
              {
                  if (i + k > 0 && i + k < anImage.rows && j + l > 0 && j + l < anImage.cols) //if not out of bounds
                      arr.push_back(anImage.at<uchar>(i + k, j + l));
                  else
                      arr.push_back(0); //padding value
              }
            }

            std::sort(arr.begin(), arr.end());

            if (arr[arr.size()-1]> 0 || m == maxsize)
            {
              res.at<uchar>(i, j) = arr[arr.size()-1]; // take max value to remove pepper
              break;
            }
          }
        }
    }
}

void FredsAdaptiveMedian(Mat &anImage, Mat &res, int maxSize)
{
    anImage.copyTo(res);
    for (int i = 0; i < anImage.rows; i++)
    {
        for (int j = 0; j < anImage.cols; j++) //for all pixels in the image
        {
            int size = 3;
            while (true)
            {
                //find all neighbours
                vector<int> arr;
                for (int k = -size/2; k <= size/2; k++)
                {
                    for (int l = -size/2; l < size/2; l++)
                    {
                        if (i + k > 0 && i + k < anImage.rows && j + l > 0 && j + l < anImage.cols) //if not out of bounds
                            arr.push_back(anImage.at<uchar>(i + k, j + l));
                        else
                            arr.push_back(0); //padding value
                    }
                }

		//sort
                std::sort(arr.begin(), arr.end());

		//the actual work
                int z_min = arr[0];
		int z_max = arr[arr.end() - arr.begin()];
		int z_med = arr[(arr.end() - arr.begin()) / 2];
		int z_xy = anImage.at<uchar>(i, j);

		//stage A
		if ((z_med - z_min) > 0 && (z_med - z_max) < 0) //if A1 > 0 AND A2 < 0
		{
		    //stage B
		    if ((z_xy - z_min) > 0 && (z_xy - z_max) < 0)  //if B1 > 0 AND B2 < 0
		    {
			res.at<uchar>(i, j) = z_xy; //output z_xy
			break;
		    }
		    else
		    {
			res.at<uchar>(i, j) = z_med; //output z_med
			break;
		    }
		}
		else
		{
		    size++; //increase the window size
		}

		if (size <= maxSize)
		{
		    //repeat stage A
		}
		else
		{
		    anImage.at<uchar>(i, j) = z_med; //output z_med
		    break;
		}
            }
        }
    }
}

void adaptiveMedianFilter(Mat &anImage, Mat &res, int maxsize)
{
    anImage.copyTo(res);
    for (int i = 0; i < anImage.rows; i++)
    {
        for (int j = 0; j < anImage.cols; j++)
        {
          for (int m = 3; m < maxsize; m++)
          {
            //find all neighbours
            vector<int> arr;
            for (int k = -m/2; k <= m/2; k++)
            {
              for (int l = -m/2; l < m/2; l++)
              {
                  if (i + k > 0 && i + k < anImage.rows && j + l > 0 && j + l < anImage.cols) //if not out of bounds
                      arr.push_back(anImage.at<uchar>(i + k, j + l));
                  else
                      arr.push_back(0); //padding value
              }
            }

            std::sort(arr.begin(), arr.end());

            if (arr[(int)(arr.size()/1.2)]> 0 && arr[(int)(arr.size()/1.2)]<255 || m == maxsize) // preset to pick about 80%
            {
              res.at<uchar>(i, j) = arr[(int)arr.size()/1.2];  // preset to pick about 80%
              break;
            }
          }
        }
    }
}

void gaussianBandReject(Mat_<Vec2f>& output, int bandWidth, int cutoff)
{
    //Define parts of the filter, for readability:
    double numerator = 0;
    double denominator = 0;
    double dist = 0;
    int rows = output.rows;
    int cols = output.cols;

    /*DEBUG*/
    cout << "Rows: " << rows << ", Cols: " << cols << endl;

    //For every element:
    for(int u = 0; u < cols; u++)
    {
        for(int v = 0; v < rows; v++)
        {
            //Real part:
            dist = sqrt(pow((double)u - (double)(cols / 2.0), 2) + pow((double)v - ((double)rows / 2.0), 2)); //Distance from center of filter
            numerator = pow(dist , 2) - pow((double)cutoff, 2);
            denominator = dist * (double)bandWidth;
            output(v, u)[0] = 1.0 - exp(-pow(numerator/denominator, 2));
            ///*DEBUG*/ cout << "Val: " << 1.0 - exp(-pow(numerator/denominator, 2)) << endl;

            //Imaginary part:
            output(v, u)[1] = 0;
        }
    }
}

void ApplyFreqFilter(Mat &anImage, Mat &aFilter)
{
    //calc optimal size
    int height = getOptimalDFTSize((2 * anImage.rows) - 1);
    int width = getOptimalDFTSize((2 * anImage.cols) - 1);

    //pad image to make it optimally sized
    Mat imgWithPad;
    copyMakeBorder(anImage, imgWithPad, 0, height - anImage.rows, 0, width - anImage.cols, BORDER_CONSTANT, 0);

    //merge
    Mat img2Channel;
    Mat temp[2] = {Mat_<float>(imgWithPad), (Mat_<float>(imgWithPad)) = 0};
    merge(temp, 2, img2Channel);

    //dft
    dft(img2Channel, img2Channel, DFT_COMPLEX_OUTPUT);

    //shift the filter quadrants
    dftshift(filter);

    //multiply in frequency domain to apply filter
    Mat resFreq;
    img2Channel.copyTo(resFreq);
    mulSpectrums(img2Channel, aFilter, resFreq, 0, false);

    //inverse Fourier transform
    Mat res;
    dft(resFreq, res, DFT_INVERSE + DFT_SCALE + DFT_REAL_OUTPUT);

    //crop to remove padding
    res = Mat(res, Rect(Point(0, 0), img.size()));

    //save
    res.copyTo(anImage);
}
