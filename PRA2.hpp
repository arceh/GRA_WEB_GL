#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "opencv2\highgui.hpp"
#include "opencv2\core.hpp"
#include "opencv2\imgproc.hpp"



#define PFADH "..\\Head_F\\"


using namespace cv;
using namespace std;



vector<Point> convertToVec(vector<vector<Point>> old) {
	vector<Point> neu;
	for (int i = 0; i < old.size();i++) {
		for (int j = 0; j < old[i].size(); j++) {
			neu.push_back(old[i][j]);
		}
	}
	return neu;

}

double euklid(Point big, Point small) {
	return sqrt(pow((big.x - big.y), 2.0) + pow((small.x - small.y), 2.0));
}

int vecIndice(vector<vector<Point>> vvPoint) {
	vector<int> tempIndices;
	double tempDist = DBL_MAX;
	double tempERG;
	int tempIndex;
	int index = 0;
	int smallPcounter = 0;
	int lastSmallPointIndex = 0;
	for (vector<vector<Point>>::iterator itvp = vvPoint.begin(); itvp != vvPoint.end(); ++itvp) {
		if ((itvp + 1)->size() <= itvp->size()) {
			for (vector<Point>::iterator itpBig = itvp->begin(); itpBig != itvp->end(); ++itpBig) {
				smallPcounter = 0;
				for (vector<Point>::iterator itpSmalls = (itvp + 1)->begin(); itpSmalls != (itvp + 1)->end(); ++itpSmalls) {
					tempERG = euklid(itpBig->dot, itpSmalls->dot);
					if (tempDist > tempERG) {
						tempDist = tempERG;
						tempIndex = smallPcounter;
					}
					smallPcounter++;
				}
				tempIndices.push_back(index);
				tempIndices.push_back(smallPcounter + itvp->size());
				tempIndices.push_back(index + 1);
				lastSmallPointIndex = smallPcounter;
			}
			constructJSONTriangle(*itvp, *(itvp + 1), index, true, tempIndices);
		}
		else {
			for (vector<Point>::iterator itp = (itvp + 1)->begin(); itp != (itvp + 1)->end(); ++itp) {

			}
			constructJSONTriangle(*(itvp + 1), *itvp, index, false, tempIndices);
		}
		index++;
	}

	return 0;
}

int constructJSONTriangle(vector<Point> &vPointBig, vector<Point> &vPointSmall, int index, bool big, vector<int> &indices) {
	if (big) {
		ofstream outfile("../contures/" + to_string(index) + ".json");
		outfile << "{ \"vertices\": [";
		for (vector<Point>::iterator itpb = vPointBig.begin(); itpb != vPointBig.end(); ++itpb) {
				outfile << itpb->x << ", " << itpb->y << ", " << index << ", ";
			}
		for (vector<Point>::iterator itps = vPointSmall.begin(); itps != vPointSmall.end(); ++itps) {
			outfile << itps->x << ", " << itps->y << ", " << index + 1;
			if (((itps - 1) != vPointSmall.end())) {
				outfile << ", ";
			}
		}
		outfile << "], ";
		outfile << " \"indices\": [";
		for (vector<int>::iterator itindi = indices.begin(); itindi != indices.end(); ++itindi) {
			outfile << &itindi;
			if (((itindi - 1) != indices.end())) {
				outfile << ", ";
			}
		}
		outfile << "] }";
		outfile.close();
	}
	else {
		ofstream outfile("../contures/" + to_string(index + 1) + ".json");
		outfile << "{ \"vertices\": [";
		for (vector<Point>::iterator itpb = vPointBig.begin(); itpb != vPointBig.end(); ++itpb) {
			outfile << itpb->x << ", " << itpb->y << ", " << index + 1 << ", ";
		}
		for (vector<Point>::iterator itps = vPointSmall.begin(); itps != vPointSmall.end(); ++itps) {
			outfile << itps->x << ", " << itps->y << ", " << index;
			if (((itps - 1) != vPointSmall.end())) {
				outfile << ", ";
			}
		}
		outfile << "], ";
		outfile << " \"indices\": [";
		for (vector<int>::iterator itindi = indices.begin(); itindi != indices.end(); ++itindi) {
			outfile << &itindi;
			if (((itindi - 1) != indices.end())) {
				outfile << ", ";
			}
		}
		outfile << "] }";
		outfile.close();
	}
	return 0;

	
}