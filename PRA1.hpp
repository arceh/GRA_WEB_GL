#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "opencv2\highgui.hpp"
#include "opencv2\core.hpp"
#include "opencv2\imgproc.hpp"



#define PFADH "..\\Head_F\\"


using namespace cv;
using namespace std;

string genFileName(string pfad, string dateipre, string dateisuffix, string bildnummer) {
	return pfad + dateipre + "." + bildnummer + "." + dateisuffix;
}

void conImg(string cPfad, Mat *bild16) {
	ifstream datei;
	
	int length = 0, payload = 0, plength = 0;

	datei.open(cPfad, ifstream::in | ifstream::binary);

	if (datei) {
		datei.seekg(0, datei.end);
		
		//Erechnet die Dateilaenge ohne Header
		length = datei.tellg();
		payload = length - (512 * 512 * 2);
		plength = length - payload;
		datei.seekg(0, datei.beg);
		datei.seekg(payload);
		
		//Ließt die datei in die Mat ein
		datei.read((char*)bild16->data, plength);

		datei.close();
	}
	else {
		const String filePfa = "ERROR file " + cPfad + " not Loaded";
		const char *error = filePfa.c_str();
		throw exception(error);
	}	
}

void segment(int schwellwert, Mat *src, Mat *dst) {

	Size size_k(3, 3);

	GaussianBlur(*src, *dst, size_k, 3);

	dst->convertTo(*dst, CV_32F);

	threshold(*dst, *dst, schwellwert, 255, THRESH_BINARY);

	dst->convertTo(*dst, CV_8U);

	medianBlur(*dst, *dst, 7);
}

vector<vector<Point>> konture(Mat *src) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(*src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	return contours;
}