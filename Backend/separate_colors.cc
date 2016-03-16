#include "opencv2/opencv.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace cv;

const int HSV_THRESH = 15;
RNG rng(12345);

int MIN_X = 0, MAX_X = 1e9;
int MIN_Y = 0, MAX_Y = 1e9;

int LEGEND_MIN_X, LEGEND_MIN_Y;
int LEGEND_MAX_X, LEGEND_MAX_Y;

const int dx[] = {-1, -1, -1, 0, 0, +1, +1, +1};
const int dy[] = {-1, 0, +1, -1, +1, -1, 0, +1};

inline bool isBlack(int h, int s, int v) {
    return s <= 30;
}

inline bool isWhite(int h, int s, int v) {
    return h <= HSV_THRESH and s <= HSV_THRESH and v >= 100;
}

vector < vector <bool> > isLegend;

inline bool isInsideLegend(int x, int y) {
    return isLegend[x][y];
    // return x >= LEGEND_MIN_X and x <= LEGEND_MAX_X and y >= LEGEND_MIN_Y and y <= LEGEND_MAX_Y;
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
            if (nx >= MIN_X and nx <= MAX_X and ny >= MIN_Y and ny <= MAX_Y and visited[nx][ny] == -1) {
                if (isInsideLegend(nx, ny))
                    continue;
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

void removeIsolated(Mat& img) {
    for (int x = 0; x < img.rows; ++x) {
        for (int y = 0; y < img.cols; ++y) {
            int val = img.at<uchar>(x, y);
            if (val > 200) {
                int countWhiteNeighs = 0;
                for (int k = 0; k < 8; ++k) {
                    int nx = x + dx[k];
                    int ny = y + dy[k];
                    if (nx >= 0 and nx < img.rows and ny >= 0 and ny < img.cols) {
                        if (img.at<uchar>(nx, ny) > 200)
                            ++countWhiteNeighs;
                    }
                }
                if (countWhiteNeighs <= 2) {
                    img.at<uchar>(x, y) = 0;
                }
            }
        }
    }
}

void erode(Mat& img) {
    vector < pair <int, int> > toBlack;
    for (int x = 0; x < img.rows; ++x) {
        for (int y = 0; y < img.cols; ++y) {
            if (img.at<uchar>(x, y) < 200)
                continue;
            int cnt = 0;
            for (int dx = -1; dx <= +1; ++dx) {
                for (int dy = -1; dy <= +1; ++dy) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 and nx < img.rows and ny >= 0 and ny < img.cols) {
                        if (img.at<uchar>(nx, ny) > 200)
                            ++cnt;
                    }
                }
            }
            if (cnt <= 5)
                toBlack.push_back(make_pair(x, y));
        }
    }
    for (int i = 0; i < toBlack.size(); ++i) {
        img.at<uchar>(toBlack[i].first, toBlack[i].second) = 0;
    }
}

void dilate(Mat& img) {
    vector < pair <int, int> > toWhite;
    for (int x = 0; x < img.rows; ++x) {
        for (int y = 0; y < img.cols; ++y) {
            if (img.at<uchar>(x, y) > 200)
                continue;
            int cnt = 0;
            for (int dx = -1; dx <= +1; ++dx) {
                for (int dy = -1; dy <= +1; ++dy) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 and nx < img.rows and ny >= 0 and ny < img.cols) {
                        if (img.at<uchar>(nx, ny) > 200)
                            ++cnt;
                    }
                }
            }
            if (cnt >= 2)
                toWhite.push_back(make_pair(x, y));
        }
    }
    for (int i = 0; i < toWhite.size(); ++i) {
        img.at<uchar>(toWhite[i].first, toWhite[i].second) = 255;
    }
}

struct Data {
    int x, y;
    int w;

    Data(int x, int y, int w) : x(x), y(y), w(w) {}

    bool operator < (const Data& o) const {
        return w > o.w;
    }
};

const int MAX_DIST = 50;

void dijkstra(Mat& img, const pair <int, int>& src, const pair <int, int>& dst) {
    if (abs(src.first - dst.first) * abs(src.first - dst.first) + abs(src.second - dst.second) * abs(src.second - dst.second) <= 5000)
        line(img, Point(src.second, src.first), Point(dst.second, dst.first), Scalar(255), 1);

    /*
    // cout<<src.first<<" "<<src.second<<"  "<<dst.first<<" "<<dst.second<<endl;
    priority_queue <Data> Q;
    
    map < pair <int, int>, bool> visited;
    map < pair <int, int>, int> dist;
    map < pair <int, int>, pair <int, int> > parent;
    
    Q.push (Data(src.first, src.second, 0));
    dist[src] = 0;
    parent[src] = make_pair(-1, -1);

    while (!Q.empty()) {
        Data p = Q.top();
        Q.pop();

        if (visited.count(make_pair(p.x, p.y)))
            continue;
        
        visited[make_pair(p.x, p.y)] = true;

        if (p.x == dst.first and p.y == dst.second) {
            break;
        }

        int x = p.x;
        int y = p.y;
        int d = dist[make_pair(x, y)];

        for (int k = 0; k < 8; ++k) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            pair <int, int> q(nx, ny);
            if (nx >= 0 and nx < img.rows and ny >= y and ny < img.cols) {
                if (visited.count(q))
                    continue;
                if (abs(nx - src.first) <= MAX_DIST and abs(ny - src.second) <= MAX_DIST) {
                    if (!dist.count(q)) {
                        dist[q] = d + 1;
                        Q.push(Data(nx, ny, d + 1));
                        parent[q] = make_pair(x, y);
                    } else if (d + 1 < dist[q]) {
                        dist[q] = d + 1;
                        Q.push(Data(nx, ny, d + 1));
                        parent[q] = make_pair(x, y);
                    }
                }
            }
        }
    }


    int x, y;

    auto p = parent[{x, y}];
    int i = dst.first;
    int j = dst.second;
    if (!parent.count(dst))
        return;
    
    while(i != src.first && j != src.second){
        assert(parent.count({i, j}));
        img.at<uchar>(i, j) = 255;
        i = parent[{i,j}].first;
        j = parent[{i,j}].second;
    }

    */
}

void fillGaps(Mat& img) {
    vector <int> temp(img.cols);
    int a = -1, b = -1;
    for (int y = 0; y < img.cols; ++y)
            temp[y] = 0;

    vector < pair < pair <int, int>, pair <int, int> > > toFill;
    for (int y = 0; y < img.cols; ++y) {
        for (int x = 0; x < img.rows; ++x) {
            if(img.at<uchar>(x,y) > 200 && y > 0){
                temp[y] = 1;
                if(temp[y-1] == 1) {
                    a = x;
                    b = y;
                } else if (a != -1 and b != -1) {
                    toFill.push_back({{a, b}, {x, y}});
                }
            }
       }
    }

    for (int i = 0; i < toFill.size(); ++i) {
        dijkstra(img, toFill[i].first, toFill[i].second);
    }
}

void filterImage(Mat& img, Mat& mask) {
    long long satSum = 0;
    long long valSum = 0;
    int cnt = 0;
    for (int x = 0; x < img.rows; ++x) {
        for (int y = 0; y < img.cols; ++y) {
            if (mask.at<uchar>(x, y) > 100) {
                satSum += img.at<Vec3b>(x, y)[1];
                valSum += img.at<Vec3b>(x, y)[2];
                ++cnt;
            }
        }
    }

    satSum /= cnt;
    valSum /= cnt;

    for (int x = 0; x < img.rows; ++x) {
        for (int y = 0; y < img.cols; ++y) {
            if (mask.at<uchar>(x, y) > 100) {
                if (abs(satSum - img.at<Vec3b>(x, y)[1]) > 100 or abs(valSum - img.at<Vec3b>(x, y)[2]) > 100)
                    mask.at<uchar>(x, y) = 0;
            }
        }
    }
}

void showContours(Mat& src_gray) {
    blur( src_gray, src_gray, Size(3,3) );
    static string str = "a";
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using Threshold
    threshold( src_gray, threshold_output, 100, 255, THRESH_BINARY );
    /// Find contours
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
     { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
       boundRect[i] = boundingRect( Mat(contours_poly[i]) );
       minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }


    /// Draw polygonal contour + bonding rects + circles
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
       circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
     }

    /// Show in a window
    namedWindow( "Contours_" + str, CV_WINDOW_NORMAL);
    imshow( "Contours_" + str, drawing );
    str[0]++;
}

int countWhitePixels(const Mat& img) {
    int cnt = 0;
    for (int x = 0; x < img.rows; ++x) {
        for (int y = 0; y < img.cols; ++y) {
            if (img.at<uchar>(x, y) > 100)
                ++cnt;
        }
    }
    return cnt;
}

typedef long long ll;

struct HSVColor {
    ll h, s, v;

    HSVColor() : h(0), s(0), v(0) {}
    HSVColor(ll h, ll s, ll v) : h(h), s(s), v(v) {}
};

string COLOR_FILE_PATH = "colors.txt";
string COLOR_IMAGES_PREF = "./";

void separateColors(Mat& img) {
    Mat dst;
    // namedWindow("Input Image BGR", WINDOW_NORMAL);
    // imshow("Input Image BGR", img);
    cvtColor(img, img, CV_BGR2HSV);
    // namedWindow("Input Image HSV", WINDOW_NORMAL);
    // imshow("Input Image HSV", img);

    // namedWindow("Output Image", WINDOW_NORMAL);
    // imshow("Output Image", dst);

    map <uchar, int> hueCount;
    for (int x = MIN_X; x <= MAX_X; ++x) {
        for (int y = MIN_Y; y <= MAX_Y; ++y) {
            if (isInsideLegend(x, y))
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

            hueCount[val.val[0]]++;
        }
    }

    vector < pair <int, int> > hueVals;
    for (auto p : hueCount) {
        hueVals.push_back(make_pair(p.second, p.first));
    }

    sort(hueVals.rbegin(), hueVals.rend());
    // cerr << hueVals.size() << "\n";
    for (int i = 0; i < 30 and i < hueVals.size(); ++i) {
        // printf("i : %d [%d]\n", i, hueVals[i].second);
    }

    vector < vector <int> > visited(img.rows, vector <int> (img.cols, -1));
    int colorId = 0;
    map <int, int> idxMap;

    for (int x = MIN_X; x <= MAX_X; ++x) {
        for (int y = MIN_Y; y <= MAX_Y; ++y) {
            if (isInsideLegend(x, y))
                continue;

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
                // printf("idx : %d colorId : %d [%d, %d, %d] at (%d, %d)\n", idx, colorId, hueVals[idx].second, s, v, x, y);
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
                    // printf("same %d and %d\n", i, j);
                    eqClass[idxMap[i]] = idxMap[j];
                    break;
                }
            }
        }
    }
    // cout << idxMap.size() << "\n";
    vector <Mat> separated(idxMap.size());
    for (int i = 0; i < separated.size(); ++i) {
        separated[i] = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
    }

    vector <HSVColor> colorSums(separated.size());
    vector <int> cnt(separated.size(), 0);

    for (int x = MIN_X; x <= MAX_X; ++x) {
        for (int y = MIN_Y; y <= MAX_Y; ++y) {
            if (isInsideLegend(x, y))
                continue;

            if (visited[x][y] == -1)
                continue;
            int id = eqClass[visited[x][y]];
            separated[id].at<uchar>(x, y) = 255;
            ++cnt[id];
            Vec3b val = img.at<Vec3b>(x, y);
            colorSums[id].h += val.val[0];
            colorSums[id].s += val.val[1];
            colorSums[id].v += val.val[2];
        }
    }

    const Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    ofstream os("colors.txt");
    for (int i = 0; i < separated.size(); ++i) {
        if (cnt[i] < 200)
            continue;;

        colorSums[i].h /= cnt[i];
        colorSums[i].s /= cnt[i];
        colorSums[i].v /= cnt[i];

        erode(separated[i]);
        dilate(separated[i]);
        // morphologyEx(separated[i], separated[i], MORPH_OPEN, kernel, Point(-1,-1), 1);
        erode(separated[i]);
        // fillGaps(separated[i]);

        filterImage(img, separated[i]);

        if (countWhitePixels(separated[i]) < 150)
            continue;

        char name[50];
        sprintf(name, "color_%lld_%lld_%lld.png", colorSums[i].h, colorSums[i].s, colorSums[i].v);

        // cerr << name << " " << countWhitePixels(separated[i]) << "\n";

        // namedWindow(name, WINDOW_NORMAL);
        // imshow(name, separated[i]);

        // showContours(separated[i]);   

        
        imwrite(name, separated[i]);

        os << colorSums[i].h << ' ' << colorSums[i].s << ' ' << colorSums[i].v << "\n";
    }
    os.close();
}

void loadBoundaries(const string& path) {
    ifstream is(path);
    is >> MIN_Y >> MIN_X >> MAX_Y >> MAX_X;
    MIN_X += 5;
    MIN_Y += 5;
    MAX_X -= 5;
    MAX_Y -= 5;
}

void loadLegendBoundaries(const string& path) {
    ifstream is(path);
    string text;
    int x1, y1, x2, y2;

    while (is >> text >> y1 >> x1 >> y2 >> x2) {
        LEGEND_MIN_X = min(LEGEND_MIN_X, x1);
        LEGEND_MAX_X = max(LEGEND_MAX_X, x2);
        LEGEND_MIN_Y = min(LEGEND_MIN_Y, y1);
        LEGEND_MAX_Y = max(LEGEND_MAX_Y, y2);
    }

    
}

int main(int argc, char const *argv[]) {
    if(argc < 2) {
        cout <<"Provide [image file] [description file] [legend file]" << endl;
        return -1;
    }

    Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if(!image.data) {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    if (argc > 2)
        loadBoundaries(argv[2]);
    MIN_X = max(MIN_X, 0);
    MAX_X = min(MAX_X, image.rows - 1);

    MIN_Y = max(MIN_Y, 0);
    MAX_Y = min(MAX_Y, image.cols - 1);

    LEGEND_MIN_X = LEGEND_MIN_Y = 1e9;
    LEGEND_MAX_X = LEGEND_MAX_Y = 0;
    if (argc > 2)
        loadLegendBoundaries(argv[3]);

    // printf("LEGEND [%d, %d] to [%d, %d]\n", LEGEND_MIN_X, LEGEND_MIN_Y, LEGEND_MAX_X, LEGEND_MAX_Y);

    isLegend.assign(image.rows, vector <bool>(image.cols, false));

    for (int i = max(0, LEGEND_MIN_X - 2); i <= LEGEND_MAX_X and i < image.rows; ++i) {
        for (int j = max(0, LEGEND_MIN_Y - 2); j <= LEGEND_MAX_Y and j < image.cols; ++j) {
            image.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
            isLegend[i][j] = true;
        }
    }
    
    separateColors(image);
    // waitKey(0);
    // destroyAllWindows();

    return 0;
}