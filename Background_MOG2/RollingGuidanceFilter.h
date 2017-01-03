/***************************************************************/
/*
*   Distribution code Version 1.0 -- 08/14/2014 by Qi Zhang Copyright 2014, The Chinese University of Hong Kong.
*
*   The Code is created based on the method described in the following paper 
*   [1] "Rolling Guidance Filter", Qi Zhang, Xiaoyong Shen, Li Xu, Jiaya Jia, European Conference on Computer Vision (ECCV) 2014
*   
*   The code includes the bilateral filter implementation of:
*	[2] "Fast High-Dimensional Filtering Using the Permutohedral Lattice", Adams, Andrew, Jongmin Baek, and Myers Abraham Davis, Computer Graphics Forum. Vol. 29. No. 2. Blackwell Publishing Ltd, 2010.
*   
*	PLEASE use the code in visual studio 2010/2012, Release Win32 Mode. (We only provide the openCV .lib/.dll files for this mode).
*
*   The code and the algorithm are for non-comercial use only.
*
/***************************************************************/

#ifndef RGF_H
#define RGF_H

/***************************************************************/
/* 
 * Standard IO library is required.
 * STL String library is required.
 *
/***************************************************************/

#include <cstdio>
#include <string>

/***************************************************************/
/* 
 * OpenCV 2.4 is required. 
 * The following code is already tested on OpenCV 2.4.2.
 *
/***************************************************************/

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\ximgproc.hpp"

//Use the namespace of CV and STD
using namespace std;
using namespace cv;

class RollingGuidanceFilter{

public:

	/***************************************************************/

	static Mat filter(Mat img, float sigma_s=3.0f, float sigma_r=0.1, int iteration=4, int space = 10){
		
		Mat I;
		
		//Change type to float
		img.convertTo(I,CV_MAKETYPE(CV_32F,img.channels()));

		Mat res = I.clone();

		// Filtering
		for(int i=0;i<iteration;i++){
			if (i)
				ximgproc::jointBilateralFilter(res, I, res, space, sigma_r, sigma_s);
				//res = bilateralPermutohedral(I,res,sigma_s,sigma_r);
			else 
				GaussianBlur(I,res,Size(0,0),sigma_s,sigma_s);
		}

		// Change type back
		res.convertTo(res,CV_MAKETYPE(img.depth(),img.channels()));

		return res;
	}

	
};

#endif