#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include <map>

#define MAXDIST 10

#define PFADH "../Head_F/Haut/"
#define PFADK "../Head_F/Knochen/"

using namespace std;
using namespace cv;

double pytago(int a, int b) {
	double c;
	c = sqrt(a*a + b*b);
	return c;
}

vector<Point3i> createTriangle(vector<Point3i> vec1, vector<Point3i> vec2) {
	double dist;
	double shortest = DBL_MAX;
	map<int, int> mapp;
	int veclang, py;
	vector<Point3i> indi;

	for (vector<Point3i>::iterator itvec1 = vec1.begin(); itvec1 != vec1.end(); ++itvec1) {
		for (vector<Point3i>::iterator itvec2 = vec2.begin(); itvec2 != vec1.end(); ++itvec2) {
			dist = pytago(vec1[x].x - vec2[y].x, vec1[x].y - vec2[y].y);

			if (dist < shortest) {
				shortest = dist;
				py = y;

			}
		}
		mapp.insert(pair<int, int>(x, py));

		shortest = DBL_MAX;
		py = 0;
	}

	/*for (int x = 0; x < vec1.size(); x++) {
		if (vec2.size()>0) {
			for (int y = 0; y < vec2.size(); y++) {


				dist = pytago(vec1[x].x - vec2[y].x, vec1[x].y - vec2[y].y);

				if (dist < shortest) {
					shortest = dist;
					py = y;

				}
			}


			mapp.insert(pair<int, int>(x, py));

			shortest = DBL_MAX;
			py = 0;
		}
	}*/

	veclang = vec1.size();

	for (map<int, int>::iterator itmapp = mapp.begin(); itmapp != mapp.end(); ++itmapp) {
		cout << itmapp->first;
		cout << itmapp->second;
	}

	/*for (int x = 0; x < mapp.size(); x++) {

		int next = (x + 1) % mapp.size();

		if (pytago(vec1[x].x - vec2[mapp[next]].x, vec1[x].y - vec2[mapp[next]].y) <= MAXDIST) {
			indi.insert(indi.end(), Point3i(x, mapp[x] + veclang, mapp[next] + veclang));
			indi.insert(indi.end(), Point3i(x, x + 1, mapp[next] + veclang));
		}
	}*/

	return indi;
}

void exportCombi(vector<Point3i> vec1, vector<Point3i> vec2, vector<Point3i> indi, vector<Point3f> onormale) {
	fstream datei;


	ostringstream pfad;
	ostringstream os, osi, oso;
	string json;

	if (vec2.size() != 0) {

		if (vec1[0].z <= vec2[0].z) {
			pfad << PFADK << "schicht" << to_string(vec1[0].z) << "_" << to_string(vec2[0].z) << ".json";
		}
		else {
			pfad << PFADK << "schicht" << to_string(vec2[0].z) << "_" << to_string(vec1[0].z) << ".json";
		}

		datei.open(pfad.str(), ios::trunc | ios::out);
		if (datei) {
			for (int x = 0; x < vec1.size(); x++) {

				os << "\n" << vec1[x].x << "," << vec1[x].y << "," << vec1[x].z << ",";

			}
			for (int x = 0; x < vec2.size(); x++) {
				if (x == vec2.size() - 1) {
					os << "\n" << vec2[x].x << "," << vec2[x].y << "," << vec2[x].z;
				}
				else {
					os << "\n" << vec2[x].x << "," << vec2[x].y << "," << vec2[x].z << ",";
				}
			}
			for (int x = 0; x < indi.size(); x++) {
				if (x == indi.size() - 1) {
					osi << "\n" << indi[x].x << "," << indi[x].y << "," << indi[x].z;
				}
				else {
					osi << "\n" << indi[x].x << "," << indi[x].y << "," << indi[x].z << ",";
				}
			}
			for (int x = 0; x < onormale.size(); x++) {
				if (x == onormale.size() - 1) {
					oso << "\n" << onormale[x].x << "," << onormale[x].y << "," << onormale[x].z;
				}
				else {
					oso << "\n" << onormale[x].x << "," << onormale[x].y << "," << onormale[x].z << ",";
				}
			}

			json = "{\n\t";
			json += 34;
			json += "vertices";
			json += 34;
			json += " : [ ";
			json += os.str();


			json += "\n\t],\n\n\t";
			json += 34;
			json += "indices";
			json += 34;
			json += " : [";

			json += osi.str();

			json += "\n\t],\n\n\t";
			json += 34;
			json += "normal";
			json += 34;
			json += " : [";

			json += oso.str();

			json += "\n\t]\n}";

			datei << json;
			datei.close();

		}
		else {
			cout << pfad.str() << " : Datei konnte nicht geöffnet werden" << endl;
		}
	}
}


void createindex(vector<vector<Point3i>> allvec, int schicht, vector<Mat> allbild) {


	vector<Point3i> vec1 = allvec[schicht];
	vector<Point3i> vec2 = allvec[schicht + 1];
	vector<Point3i> indi;
	vector<Point3f> onormale;

	onormale = Onormal(allbild, allvec, schicht);
	if (vec1.size() >= vec2.size()) {

		indi = createTriangle(vec1, vec2);
		exportCombi(vec1, vec2, indi, onormale);

	}
	else {

		indi = createTriangle(vec2, vec1);
		exportCombi(vec2, vec1, indi, onormale);

	}
}


