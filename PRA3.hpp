#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "opencv2\highgui.hpp"
#include "opencv2\core.hpp"
#include "opencv2\imgproc.hpp"


using namespace cv;
using namespace std;

vector<vector<double>> oberflaeche(vector<Mat> bild,vector<vector<Point>> allepunkte) {
	vector<vector<double>>oberfaechennormale;
	for (size_t i = 0; (i+1) < allepunkte.size(); i++)
	{
		for (size_t j = 0; j < allepunkte[i].size(); j++)
		{
			double x,y,z;
			x=bild[i].at<uchar>(allepunkte[i][j].x+1,allepunkte[i][j].y)- bild[i].at<uchar>(allepunkte[i][j].x - 1, allepunkte[i][j].y);
			y= bild[i].at<uchar>(allepunkte[i][j].x,allepunkte[i][j].y+1)- bild[i].at<uchar>(allepunkte[i][j].x, allepunkte[i][j].y - 1);
			z = bild[i].at<uchar>(allepunkte[i][j+1].x, allepunkte[i][j+1].y)- bild[i].at<uchar>(allepunkte[i][j].x, allepunkte[i][j].y );
			oberfaechennormale[i].insert(oberfaechennormale[i].end(), x);

			
		}

	}

	return null;
}
