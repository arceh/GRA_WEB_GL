
#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include "PRA1.hpp"
#include "PRA2.hpp"
#include "PRA3.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\core.hpp"
#include "opencv2\imgproc.hpp"

#define START 1501
#define END 1734
#define SCHWELLWERT 600


using namespace std;
using namespace cv;


int main()
{
	cout << "---GRAPHEN PRAKTIKUM #1---" << endl;

	cout << "3.1 GEN DATA NAME" << endl;

	namedWindow("DISPLAY_WINDOW1", WINDOW_AUTOSIZE);
	namedWindow("DISPLAY_WINDOW2", WINDOW_AUTOSIZE);
	namedWindow("DISPLAY_WINDOW3", WINDOW_AUTOSIZE);

	moveWindow("DISPLAY_WINDOW1", 0, 0);
	moveWindow("DISPLAY_WINDOW2", 520, 0);
	moveWindow("DISPLAY_WINDOW3", 1050, 0);

	vector<vector<Point>> tempConturs;
	vector<vector<Point>> allcontinone;

	try
	{
		vector<Mat> allebilder;
		for (int i = START; i <= END; i++) {
			Mat bild16(512, 512, CV_16U);
			Mat bild8(512, 512, CV_8U);
			string filename;
			ofstream outfile("..\\contures\\" + to_string(i) + ".json");

			double minVal = 0, maxVal = 0, scale = 0, shift = 0;

			filename = genFileName(PFADH, "vhf", "dcm", to_string(i));

			conImg(filename, &bild16);
			//Convertierung zu bild8
			minMaxLoc(bild16, &minVal, &maxVal);
			scale = 255 / (maxVal - minVal);
			shift = -minVal * 255 / (maxVal - minVal);

			convertScaleAbs(bild16, bild8, scale, shift);

			imshow("DISPLAY_WINDOW1", bild8);

			segment(SCHWELLWERT, &bild16, &bild8);

			imshow("DISPLAY_WINDOW2", bild8);

			//Hier neues Bild erzeugen

			imshow("DISPLAY_WINDOW3", bild8);

			waitKey(10);

			tempConturs =konture(&bild8);

			//string tempString;

			outfile << "{ \"vertices\": [";
			//tempString.append("{ \"vertices\": [");
			for (int j = 0; j < tempConturs.size(); j++) {
				for (int k = 0; k < tempConturs[j].size(); k++) {
					outfile << tempConturs[j][k].x << ", " << tempConturs[j][k].y << ", " << i;
					//tempString.append(to_string(tempConturs[j][k].x) + ", " + to_string(tempConturs[j][k].y) + ", " + to_string(i));
					if (!(j == tempConturs.size() - 1 && k == tempConturs[j].size() - 1)) {
						outfile << ", ";
					}
				}
			}
			outfile << "] }";
			outfile.close();
			allcontinone.insert(allcontinone.end(), convertToVec(tempConturs));
			allebilder.insert(allebilder.end(), bild8);

		}
		vector<int> test;
		//test = vecIndice(allcontinone);
		//cout << test[0];
		vector<vector<double>> allOberNormalen=oberflaeche(allebilder,allcontinone);
		vecIndice3(allcontinone, allOberNormalen);
	}
	catch (const std::exception& error)
	{
		cout << error.what() << endl;
	}

	//waitKey();

	return 0;
}

