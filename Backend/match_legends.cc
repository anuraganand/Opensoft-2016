#include "opencv2/opencv.hpp"
#include <fstream>

const int HSV_THRESH = 15;

using namespace std;
using namespace cv;

struct HSVColor {
    int h, s, v;

    HSVColor() : h(0), s(0), v(0) {}
    HSVColor(int h, int s, int v) : h(h), s(s), v(v) {}
};

vector <HSVColor> colors;
vector <string> entityName;

void readColors(const string& path) {
	ifstream is(path);
	int h, s, v;
	while (is >> h >> s >> v) {
		colors.push_back(HSVColor(h, s, v));
	}

	entityName.resize(colors.size());
	for (int i = 1; i <= entityName.size(); ++i) {
		char name[50];
		sprintf(name, "E%02d", i);
		entityName[i - 1] = name;
	}

	cerr << "Colors read" << endl;
	for (int i = 0; i < colors.size(); ++i) {
		cout << "[" << colors[i].h << ", " << colors[i].s << ", " << colors[i].v << "]\n";
	}
}

const int NEG_DELTA = 50;
const int POS_DELTA = 50;

int MIN_X, MAX_X;
int MIN_Y, MAX_Y;

inline bool isWhite(int h, int s, int v) {
    return h <= HSV_THRESH and s <= HSV_THRESH and v >= 100;
}

HSVColor getColor(Mat& img, const string& text, int x1, int y1, int x2, int y2) {
    vector <int> cnt(colors.size(), 0);
    int maxCount = 0, bestIdx = -1;

    for (int x = x1; x <= x2; ++x) {
    	for (int y = y1 - NEG_DELTA; y <= y1 and y < img.cols; ++y) {
    		Vec3b val = img.at<Vec3b>(x, y);
    		int h = val.val[0];
    		int s = val.val[1];
    		int v = val.val[2];
    		
    		if (isWhite(h, s, v)) {
    			continue;
    		}

    		// Vec3b black;
    		// black[0] = 0, black[1] = 0, black[2] = 0;

    		// img.at<Vec3b>(x, y) = black;

    		for (int i = 0; i < colors.size(); ++i) {
    			if (abs(colors[i].h - h) <= HSV_THRESH) {
    				++cnt[i];
    				if (maxCount < cnt[i]) {
    					maxCount = cnt[i];
    					bestIdx = i;
    				}
    			}
    		}
    	}
    }

    // if (bestIdx != -1) {
    // 	cout << text << " " << colors[bestIdx].h << " " << colors[bestIdx].s << " " << colors[bestIdx].v << endl;	
    // 	return colors[bestIdx];
    // }

    for (int x = x1; x <= x2; ++x) {
    	for (int y = y2; y <= y2 + POS_DELTA; ++y) {
    		Vec3b val = img.at<Vec3b>(x, y);
    		int h = val.val[0];
    		int s = val.val[1];
    		int v = val.val[2];
    		
    		if (isWhite(h, s, v)) {
    			continue;
    		}

    		// Vec3b black;
    		// black[0] = 0, black[1] = 0, black[2] = 0;

    		// img.at<Vec3b>(x, y) = black;
    		
    		for (int i = 0; i < colors.size(); ++i) {
    			if (abs(colors[i].h - h) <= HSV_THRESH) {
    				++cnt[i];
    				if (maxCount < cnt[i]) {
    					maxCount = cnt[i];
    					bestIdx = i;
    				}
    			}
    		}
    	}
    }

    if (bestIdx != -1) {
    	entityName[bestIdx] = text;
    	// cout << text << " " << colors[bestIdx].h << " " << colors[bestIdx].s << " " << colors[bestIdx].v << endl;

    	return colors[bestIdx];
    }

    // cout << "No color found for " << text << endl;
    return HSVColor(-1, -1, -1);
}

double SCALING_X = 1.0;
double SCALING_Y = 1.0;
int MAX_AREA = (int)1e6;

void getLegendColors(Mat& img, const string& path) {
	ifstream is(path);
	string text;
	int x1, y1, x2, y2;
	while (is >> text >> y1 >> x1 >> y2 >> x2) {
		x1 = (int)(SCALING_X * x1);
		x2 = (int)(SCALING_X * x2);
		y1 = (int)(SCALING_Y * y1);
		y2 = (int)(SCALING_Y * y2);
		getColor(img, text, x1, y1, x2, y2);
	}
}


int main(int argc, char const *argv[]) {
	if(argc != 4) {
	    cerr << "Provide image file, legend file and colors file" << endl;
	    return -1;
	}

	string imageFile  = argv[1];
	string legendFile = argv[2];
	string colorsFile = argv[3];

	Mat image = imread(imageFile, CV_LOAD_IMAGE_COLOR);
	if (!image.data) {
		cerr << "Image reading failed." << endl;
		return -1;
	}

	int area = image.rows * image.cols;
	// cerr << "area : " << area << "\n";
	if (area > MAX_AREA) {
		double aspect = (double)image.cols / image.rows;
		int rows = (int)sqrt(MAX_AREA / aspect);
		int cols = MAX_AREA / rows;
		SCALING_X = (double)rows / image.rows;
		SCALING_Y = (double)cols / image.cols;
		// cerr << "rows : " << rows << " cols : " << cols << "\n";
		resize(image, image, Size(cols, rows), INTER_LANCZOS4);
	}
	cvtColor(image, image, CV_BGR2HSV);

	readColors(colorsFile);

	getLegendColors(image, legendFile);

	ofstream os("file_legend_map.txt");
	for (int i = 0; i < entityName.size(); ++i) {
		os << "color_" << colors[i].h << "_" << colors[i].s << "_" << colors[i].v << " " << entityName[i] << "\n";
		// char name[50];
    	// sprintf(name, "color_%d_%d_%d.png", colors[bestIdx].h, colors[bestIdx].s, colors[bestIdx].v);
	}
    os.close();
	// namedWindow("Image", WINDOW_NORMAL);
	// imshow("Image", image);
	// waitKey(0);
	// destroyAllWindows();

	return 0;
}