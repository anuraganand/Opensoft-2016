#include "opencv2/opencv.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
using namespace cv;

const int HSV_THRESH = 15;

const int dx[] = {-1, -1, -1, 0, 0, +1, +1, +1};
const int dy[] = {-1, 0, +1, -1, +1, -1, 0, +1};

inline bool isBlack(int h, int s, int v) {
    return s <= 30;
}

inline bool isWhite(int h, int s, int v) {
    return h <= HSV_THRESH and s <= HSV_THRESH and v >= 100;
}

void bfs(const Mat& img, vector < vector <int> >& visited, int x, int y, int hVal, int colorId) {
    queue < pair <int, int> > q;
    q.push(make_pair(x, y));
    visited[x][y] = colorId;
    while (!q.empty()) {
        x = q.front().first;
        y = q.front().second;
        q.pop();
        for (int k = 0; k < 8; ++k) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (nx >= 0 and nx < img.rows and ny >= 0 and ny < img.cols and visited[nx][ny] == -1) {
                Vec3b val = img.at<Vec3b>(nx, ny);
                int h = val.val[0];
                int s = val.val[1];
                int v = val.val[2];

                if (isWhite(h, s, v)) {
                    // white
                    continue;
                }

                if (isBlack(h, s, v)) {
                    // black
                    continue;
                }

                if (abs(h - hVal) <= HSV_THRESH) {
                    visited[nx][ny] = colorId;
                    q.push(make_pair(nx, ny));
                }
            }
        }
    }
}

void separateColors(Mat& img) {
    Mat dst;
    cvtColor(img, img, CV_BGR2HSV);
    namedWindow("Input Image", WINDOW_NORMAL);
    imshow("Input Image", img);

    // namedWindow("Output Image", WINDOW_NORMAL);
    // imshow("Output Image", dst);

    map <uchar, int> hueCount;
    for (int x = 0; x < img.rows; ++x) {
        for (int y = 0; y < img.cols; ++y) {
            Vec3b val = img.at<Vec3b>(x, y);
            int h = val.val[0];
            int s = val.val[1];
            int v = val.val[2];

            if (isWhite(h, s, v)) {
                // white
                continue;
            }

            if (isBlack(h, s, v)) {
                // black
                continue;
            }

            hueCount[val.val[0]]++;
        }
    }

    vector < pair <int, int> > hueVals;
    for (auto p : hueCount) {
        hueVals.push_back(make_pair(p.second, p.first));
    }

    sort(hueVals.rbegin(), hueVals.rend());

    for (int i = 0; i < 30 and i < hueVals.size(); ++i) {
        printf("i : %d [%d]\n", i, hueVals[i].second);
    }

    vector < vector <int> > visited(img.rows, vector <int> (img.cols, -1));
    int colorId = 0;
    map <int, int> idxMap;

    for (int x = 0; x < img.rows; ++x) {
        for (int y = 0; y < img.cols; ++y) {
            if (visited[x][y] != -1)
                continue;

            Vec3b val = img.at<Vec3b>(x, y);
            int h = val.val[0];
            int s = val.val[1];
            int v = val.val[2];

            if (isWhite(h, s, v)) {
                // white
                continue;
            }

            if (isBlack(h, s, v)) {
                // black
                continue;
            }

            int idx = -1;
            for (int i = 0; i < hueVals.size(); ++i) {
                if (abs(hueVals[i].second - h) <= HSV_THRESH) {
                    idx = i;
                    break;
                }
            }
            if (!idxMap.count(idx)) {
                printf("idx : %d colorId : %d [%d, %d, %d] at (%d, %d)\n", idx, colorId, hueVals[idx].second, s, v, x, y);
                idxMap[idx] = colorId++;
            }

            bfs(img, visited, x, y, hueVals[idx].second, idxMap[idx]);
        }
    }

    map <int, int> eqClass;
    for (int i = 0; i < hueVals.size(); ++i) {
        if (idxMap.count(i)) {
            for (int j = 0; j <= i; ++j) if (idxMap.count(j)) {
                if (abs(hueVals[i].second - hueVals[j].second) <= HSV_THRESH) {
                    printf("same %d and %d\n", i, j);
                    eqClass[idxMap[i]] = idxMap[j];
                    break;
                }
            }
        }
    }
    cout << idxMap.size() << "\n";
    vector <Mat> separated(idxMap.size());
    for (int i = 0; i < separated.size(); ++i) {
        separated[i] = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
    }

    vector <int> cnt(separated.size(), 0);
    for (int x = 0; x < img.rows; ++x) {
        for (int y = 0; y < img.cols; ++y) {
            if (visited[x][y] == -1)
                continue;
            // printf("setting for %d\n", visited[x][y]);
            int id = eqClass[visited[x][y]];
            separated[id].at<uchar>(x, y) = 255;
            ++cnt[id];
        }
    }

    const Mat kernel = cv::getStructuringElement(
                     cv::MORPH_RECT, cv::Size(2, 2));

    for (int i = 0; i < separated.size(); ++i) {
        if (cnt[i] < 150)
            continue;
        string win(1, 'a' + i);
        // morphologyEx(separated[i], separated[i], MORPH_OPEN, kernel, Point(-1,-1), 1);
        namedWindow(win, WINDOW_NORMAL);
        imshow(win, separated[i]);
    }
}

int main(int argc, char const *argv[]) {
    if( argc != 2) {
        cout <<"Provide image file." << endl;
        return -1;
    }

    Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    if(!image.data) {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    separateColors(image);
    waitKey(0);
    destroyAllWindows();
    return 0;
}