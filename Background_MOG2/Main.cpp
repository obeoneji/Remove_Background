//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>

//filter
#include "RollingGuidanceFilter.h"

using namespace cv;
using namespace std;

string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

int main(int argc, char* argv[])
{
	Mat img = imread(argv[1]);
	Mat bg = imread(argv[2]);

	bg = RollingGuidanceFilter::filter(bg, 3, 25.5, 2, 10);
	img = RollingGuidanceFilter::filter(img, 3, 25.5, 2, 10);

	Mat Mask;

	Mat dbg;
	bg.convertTo(dbg, CV_32FC3, 1.0 / 255.0);

	Mat dimg;
	img.convertTo(dimg, CV_32FC3, 1.0 / 255.0);

	cvtColor(dbg, dbg, CV_BGR2HSV);
	cvtColor(dimg, dimg, CV_BGR2HSV);

	Mat bg_hsv[3];
	Mat img_hsv[3];

	split(dbg, bg_hsv);
	split(dimg, img_hsv);



	Mask = (abs(bg_hsv[0] - img_hsv[0]) / 180.0 + abs(bg_hsv[1] - img_hsv[1]) * 2.0);// +abs(bg_hsv[2] - img_hsv[2]) / 1.0);

	

	
	Mask = Mask > 0.32;

	/*Mat mask_s;
	resize(Mask, mask_s, Size(0, 0), 0.5, 0.5, 1);
	imshow("mask_s", mask_s);
	waitKey(0);*/

	Mat Mask_g = (img_hsv[0] > 135) & (img_hsv[0] < 175);

	string ty = type2str(Mask_g.type());
	printf("Matrix: %s %dx%d \n", ty.c_str(), Mask_g.cols, Mask_g.rows);

	float h_ave = 0;
	float s_ave = 0;
	float v_ave = 0;
	int h_count = 0;

	for (int i = 0; i < img_hsv[0].rows; i++)
	{
		for (int j = 0; j < img_hsv[0].cols; j++)
		{
			Scalar intensity = Mask_g.at<unsigned char>(i, j);
			if (intensity.val[0] > 1)
			{
				h_ave += img_hsv[0].at<float>(i, j);
				s_ave += img_hsv[1].at<float>(i, j);
				v_ave += img_hsv[2].at<float>(i, j);
				h_count++;
			}
		}
	}

	h_ave = h_ave / h_count;
	s_ave = s_ave / h_count;
	v_ave = v_ave / h_count;

	cout << h_ave << " " << s_ave << " " << v_ave << endl;

	Mat Mask_ng_1 = (img_hsv[0] < h_ave-18.0) | (img_hsv[0] > h_ave+18.0);

	/**/
	Mask = Mask & Mask_ng_1;

	Mat Mask_hole_fill = Mask.clone();

	cvtColor(Mask_hole_fill, Mask_hole_fill, CV_GRAY2BGR);
	int largest_area = 50000;
	Scalar color(255, 0, 0);

	vector<vector<Point>> contours; // Vector for storing contour
	vector<Vec4i> hierarchy;
	findContours(Mask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i< contours.size(); i++)
	{
		//  Find the area of contour
		double a = contourArea(contours[i], false);
		if (a<largest_area) 
		{
			drawContours(Mask_hole_fill, contours, i, color, CV_FILLED, 8, hierarchy);

		}
	}

	Mat Mask_hole_fill_g[3];
	split(Mask_hole_fill, Mask_hole_fill_g);


	Mat Mask_fn;
	Mask_hole_fill_g[0].convertTo(Mask_fn, CV_32FC1, 1.0 / 255.0);
	for (int i = 0; i < 3; i++)
	{
		Mask_fn.convertTo(Mask_fn, CV_32FC1);
		boxFilter(Mask_fn, Mask_fn, -1, Size(3, 3), Point(-1, -1), false, BORDER_DEFAULT);
		Mask_fn = Mask_fn > 2.0;
		/*Mat mask_s;
		resize(Mask_fn, mask_s, Size(0, 0), 0.5, 0.5, 1);
		imshow("mask_s", mask_s);
		waitKey(0);*/
	}


	imwrite(argv[3], Mask_fn);


	img.release();
	bg.release();
	Mask.release();
	dbg.release();
	dimg.release();
	bg_hsv[0].release();
	bg_hsv[1].release();
	bg_hsv[2].release();
	img_hsv[0].release();
	img_hsv[1].release();
	img_hsv[2].release();
	Mask_ng_1.release();
	Mask_fn.release();
	Mask_hole_fill.release();
	Mask_hole_fill_g[0].release();
	Mask_hole_fill_g[1].release();
	Mask_hole_fill_g[2].release();
	return 1;
	
}