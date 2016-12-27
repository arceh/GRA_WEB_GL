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
	return sqrt(pow((big.x - small.x), 2.0) + pow((big.y - small.y), 2.0));
}

void constructJSONTriangle(vector<Point> vPointBig, vector<Point> vPointSmall, int index, int smal, int indexbig, vector<int> indices) {

	ofstream outfile("..\\vortex\\" + to_string(index) + ".json");
	outfile << "{ \"vertices\": [";

	for (int j = 0; j < vPointBig.size(); j++) {
		outfile << vPointBig[j].x << ", " << vPointBig[j].y << ", " << indexbig << ",";

	}
	for (int j = 0; j < vPointSmall.size(); j++) {
		outfile << vPointSmall[j].x << ", " << vPointSmall[j].y << ", " << smal;

		if (!(j == vPointSmall.size() - 1)) {
			outfile << ", ";
		}
	}

	outfile << "],\n";
	outfile << "\"indices\": [";
	for (int i = 0;i < indices.size();i++) {
		outfile << indices[i];
		if (!(i == indices.size() - 1)) {
			outfile << ", ";
		}
	}
	outfile << "] }";
	outfile.close();


}

vector<int> vecIndice(vector<vector<Point>> vvPoint) {
	vector<int> tempIndices;
	vector<int>realini;
	int tempindex = 0;
	
	for (int i=0 ;(i+1) < vvPoint.size();i++) {
		if (vvPoint[i].size() > vvPoint[i + 1].size()) {
			for (int j = 0;j < vvPoint[i].size();j++) {
				double tempgrr = 900000;
				for (int k = 0;k < vvPoint[i+1].size();k++) {
					if (10.0 > euklid(vvPoint[i][j], vvPoint[i + 1][k])) {
						if (tempgrr > euklid(vvPoint[i][j], vvPoint[i + 1][k])) {
							tempgrr = euklid(vvPoint[i][j], vvPoint[i + 1][k]);
							tempindex = k;
						}
					}
				}
				tempIndices.insert(tempIndices.end(), j);
				tempIndices.insert(tempIndices.end(), tempindex);
			}
			for (int p = 0;(p+3) < tempIndices.size();p=p+2) {
				realini.insert(realini.end(), tempIndices[p]);
				realini.insert(realini.end(), vvPoint[i].size()+tempIndices[p+1]);
				realini.insert(realini.end(), vvPoint[i].size()+tempIndices[p + 3]);

				realini.insert(realini.end(), tempIndices[p]);
				realini.insert(realini.end(), vvPoint[i].size()+tempIndices[p + 3]);
				realini.insert(realini.end(), tempIndices[p + 2]);
			}
			constructJSONTriangle(vvPoint[i], vvPoint[i+1], i, i+1, i, realini);
		}//if end
		else {
			for (int j = 0;j < vvPoint[i+1].size();j++) {
				double tempgrr = 900000;
				for (int k = 0;k < vvPoint[i].size();k++) {
					if (10.0 > euklid(vvPoint[i][j], vvPoint[i + 1][k])) {
						if (tempgrr > euklid(vvPoint[i][j], vvPoint[i + 1][k])) {
							tempgrr = euklid(vvPoint[i][j], vvPoint[i + 1][k]);
							tempindex = k;
						}
					}
				}
				tempIndices.insert(tempIndices.end(), j);
				tempIndices.insert(tempIndices.end(), tempindex);
			}
			for (int p = 0;(p + 3) < tempIndices.size();p = p + 2) {
				realini.insert(realini.end(), tempIndices[p]);
				realini.insert(realini.end(), vvPoint[i+1].size() + tempIndices[p + 1]);
				realini.insert(realini.end(), vvPoint[i+1].size() + tempIndices[p + 3]);

				realini.insert(realini.end(), tempIndices[p]);
				realini.insert(realini.end(), vvPoint[i+1].size() + tempIndices[p + 3]);
				realini.insert(realini.end(), tempIndices[p + 2]);
			}
			constructJSONTriangle(vvPoint[i + 1], vvPoint[i], i, i, i + 1, realini);
		}//else end
		tempIndices.clear();
		realini.clear();
	}//alleconturen end

	return realini;
}

