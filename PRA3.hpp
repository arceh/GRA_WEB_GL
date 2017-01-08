#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "PRA2.hpp"
#include <cmath>
#include "opencv2\highgui.hpp"
#include "opencv2\core.hpp"
#include "opencv2\imgproc.hpp"


using namespace cv;
using namespace std;


void constructJSONTriangle3(vector<Point> vPointBig, vector<Point> vPointSmall, int index, int smal, int indexbig, vector<int> indices,vector<vector<double>> normalen) {

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
	outfile << "], ";
	outfile << "\"normal\":[";
	for (int i = 0;i < normalen[index].size();i++) {
		outfile << normalen[index][i];
		if (!(i == normalen[index].size() - 1)) {
			outfile << ", ";
		}
	}
	outfile << "] }";
	outfile.close();


}

vector<int> vecIndice3(vector<vector<Point>> vvPoint,vector<vector<double>> normalen) {
	vector<int> tempIndices;
	vector<int>realini;
	int tempindex = 0;

	for (int i = 0;(i + 1) < vvPoint.size();i++) {
		if (vvPoint[i].size() > vvPoint[i + 1].size()) {
			for (int j = 0;j < vvPoint[i].size();j++) {
				double tempgrr = 900000;
				for (int k = 0;k < vvPoint[i + 1].size();k++) {
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
				realini.insert(realini.end(), vvPoint[i].size() + tempIndices[p + 1]);
				realini.insert(realini.end(), vvPoint[i].size() + tempIndices[p + 3]);

				realini.insert(realini.end(), tempIndices[p]);
				realini.insert(realini.end(), vvPoint[i].size() + tempIndices[p + 3]);
				realini.insert(realini.end(), tempIndices[p + 2]);
			}
			constructJSONTriangle3(vvPoint[i], vvPoint[i + 1], i, i + 1, i, realini,normalen);
		}//if end
		else {
			for (int j = 0;j < vvPoint[i + 1].size();j++) {
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
				realini.insert(realini.end(), vvPoint[i + 1].size() + tempIndices[p + 1]);
				realini.insert(realini.end(), vvPoint[i + 1].size() + tempIndices[p + 3]);

				realini.insert(realini.end(), tempIndices[p]);
				realini.insert(realini.end(), vvPoint[i + 1].size() + tempIndices[p + 3]);
				realini.insert(realini.end(), tempIndices[p + 2]);
			}
			constructJSONTriangle3(vvPoint[i + 1], vvPoint[i], i, i, i + 1, realini, normalen);
		}//else end
		tempIndices.clear();
		realini.clear();
	}//alleconturen end

	return realini;
}

vector<vector<double>> oberflaeche(vector<Mat> bild,vector<vector<Point>> allepunkte) {
	vector<vector<double>>oberfaechennormale;
	vector<double> tmp;
	for (size_t i = 0; (i+1) < allepunkte.size(); i++)
	{
		oberfaechennormale.insert(oberfaechennormale.end(), tmp);
		for (size_t j = 0; j < allepunkte[i].size(); j++)
		{
			double x,y,z,norm;
			x = bild[i].at<uchar>(allepunkte[i][j].y, allepunkte[i][j].x + 1)- bild[i].at<uchar>(allepunkte[i][j].y, allepunkte[i][j].x - 1);
			y= bild[i].at<uchar>(allepunkte[i][j].y+1, allepunkte[i][j].x)- bild[i].at<uchar>( allepunkte[i][j].y - 1, allepunkte[i][j].x);
			if(j>=allepunkte[i+1].size())
				z = bild[i].at<uchar>(allepunkte[i + 1][0].y, allepunkte[i + 1][0].x) - bild[i].at<uchar>(allepunkte[i][j].y, allepunkte[i][j].x);
			else {
				z = bild[i].at<uchar>(allepunkte[i + 1][j].y, allepunkte[i + 1][j].x) - bild[i].at<uchar>(allepunkte[i][j].y, allepunkte[i][j].x);
			}
			norm = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
			cout << i << endl;
			x = (1 / norm)*x;
			y = (1 / norm)*y;
			z = (1 / norm)*z;
			oberfaechennormale[i].insert(oberfaechennormale[i].end(), x);
			oberfaechennormale[i].insert(oberfaechennormale[i].end(), y);
			oberfaechennormale[i].insert(oberfaechennormale[i].end(), z);

			
		}

	}

	return oberfaechennormale;
}



