//
// Created by williamwestwood on 22/05/24.
//

#include "get_results.h"

#include <bitset>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <string>
#include <ui_displaywidget.h>

#include "analyse_results.h"
#include "displaywidget.h"
#include "GlobalData.h"

using namespace std;
using namespace cv;

vector<vector<vector<vector<vector<float>>>>> points = {
    { // Question 1.
        { // For
            {{0.005, 0.155}, {0.029, 0.175}},
            {{0.005, 0.185}, {0.029, 0.205}},
            {{0.255, 0.155}, {0.279, 0.175}},
            {{0.255, 0.185}, {0.279, 0.205}},
        },
        { // Against
            {{0.503, 0.155}, {0.527, 0.175}},
            {{0.503, 0.185}, {0.527, 0.205}},
            {{0.753, 0.155}, {0.777, 0.175}},
            {{0.753, 0.185}, {0.777, 0.205}},
        }
    },
    { // Question 2.
        { // For
            {{0.005, 0.323}, {0.029, 0.343}},
            {{0.005, 0.353}, {0.029, 0.373}},
            {{0.255, 0.323}, {0.279, 0.343}},
            {{0.255, 0.353}, {0.279, 0.373}},
        },
        { // Against
            {{0.503, 0.323}, {0.527, 0.343}},
            {{0.503, 0.353}, {0.527, 0.373}},
            {{0.753, 0.323}, {0.777, 0.343}},
            {{0.753, 0.353}, {0.777, 0.373}},
        }
    },
    { // Question 3.
        { // For
            {{0.005, 0.489}, {0.029, 0.509}},
            {{0.005, 0.519}, {0.029, 0.539}},
            {{0.255, 0.489}, {0.279, 0.509}},
            {{0.255, 0.519}, {0.279, 0.539}},
        },
        { // Against
            {{0.503, 0.489}, {0.527, 0.509}},
            {{0.503, 0.519}, {0.527, 0.539}},
            {{0.753, 0.489}, {0.777, 0.509}},
            {{0.753, 0.519}, {0.777, 0.539}},
        }
    },
    { // Question 4.
        { // For
            {{0.005, 0.656}, {0.029, 0.676}},
            {{0.005, 0.686}, {0.029, 0.706}},
            {{0.255, 0.656}, {0.279, 0.676}},
            {{0.255, 0.686}, {0.279, 0.706}},
        },
        { // Against
            {{0.503, 0.656}, {0.527, 0.676}},
            {{0.503, 0.686}, {0.527, 0.706}},
            {{0.753, 0.656}, {0.777, 0.676}},
            {{0.753, 0.686}, {0.777, 0.706}},
        }
    },
    {  // Question 5.
        { // For
            {{0.005, 0.823}, {0.029, 0.843}},
            {{0.005, 0.853}, {0.029, 0.873}},
            {{0.255, 0.823}, {0.279, 0.843}},
            {{0.255, 0.853}, {0.279, 0.873}},
        },
        { // Against
            {{0.503, 0.823}, {0.527, 0.843}},
            {{0.503, 0.853}, {0.527, 0.873}},
            {{0.753, 0.823}, {0.777, 0.843}},
            {{0.753, 0.853}, {0.777, 0.873}},
        }
    },
};

bool isCircleFilled(Mat& image, Point p1, Point p2, double fillThreshold = 0.8) {

    cv::Rect roi(p1.x, p1.y, p2.x-p1.x, p2.y-p1.y);
    cv::Mat roiImage = image(roi);

    // Convert to grayscale
    Mat gray;
    cvtColor(roiImage, gray, COLOR_BGR2GRAY);

    // Apply Gaussian blur
    Mat blurred;
    GaussianBlur(gray, blurred, Size(9, 9), 2, 2);

    // Apply Otsu's thresholding
    Mat thresh;
    threshold(blurred, thresh, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

    // Apply morphological operations to close small gaps in the detected circles
    Mat morph;
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(thresh, morph, MORPH_CLOSE, kernel);


    // Detect circles using Hough Circle Transform with adjusted parameters
    vector<Vec3f> circles;
    HoughCircles(morph, circles, HOUGH_GRADIENT, 2, morph.rows / 8, 100, 30, morph.rows/4, morph.rows);

    if (circles.empty()) {
        cout << "No circles detected." << endl;
        return false;
    }

    // Assuming the first detected circle is our checkbox
    Vec3f circle = circles[0];
    Point center(cvRound(circle[0]), cvRound(circle[1]));
    int radius = cvRound(circle[2]);

    // Create a mask for the detected circle
    Mat mask = Mat::zeros(morph.size(), morph.type());
    cv::circle(mask, center, radius, Scalar(255, 255, 255), -1);


    // Calculate the filled area within the circle
    Mat circleArea;
    bitwise_and(morph, mask, circleArea);


    double totalPixels = countNonZero(mask);
    double filledPixels = countNonZero(circleArea);

    double fillRatio = filledPixels / totalPixels;

    // Determine if the circle is filled based on the threshold
    return fillRatio > fillThreshold;
}


bool* get_circle_results(cv::Mat &image) {
    const int image_width = image.cols;
    const int image_height = image.rows;

    bool* results = new bool[40];

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 4; ++k) {
                bool filled = isCircleFilled(
                    image,
                    Point(static_cast<int>(std::round(image_width * points[i][j][k][0][0])), static_cast<int>(std::round(image_height * points[i][j][k][0][1]))),
                    Point(static_cast<int>(std::round(image_width * points[i][j][k][1][0])), static_cast<int>(std::round(image_height * points[i][j][k][1][1])))
                    );
                if (filled) {
                    results[i*8+j*4+k] = true;
                } else {
                    results[i*8+j*4+k] = false;
                }
                rectangle(image,
                    Point(static_cast<int>(std::round(image_width * points[i][j][k][0][0])), static_cast<int>(std::round(image_height * points[i][j][k][0][1]))),
                    Point(static_cast<int>(std::round(image_width * points[i][j][k][1][0])), static_cast<int>(std::round(image_height * points[i][j][k][1][1]))),
                    Scalar(0,0,255));
                putText(image, to_string(i*8+j*4+k) + " - " + to_string(filled),
                        Point(static_cast<int>(std::round(image_width * points[i][j][k][0][0])),
                              static_cast<int>(std::round(image_height * points[i][j][k][0][1]))),
                              FONT_HERSHEY_SIMPLEX,
                        0.5,
                        Scalar(0, 255, 0),
                        2);
            }
        }
    }

    bool hasValue = false;

    cout << "[RESULTS] ";
    for (int i=0; i<40; i++) {
        if (results[i]) {
            hasValue = true;
        }
        cout << results[i] << " ";
    }
    cout << endl;

    GlobalData::getInstance()->setResultsData(results);

    DisplayWidget* displaywidget = GlobalData::getInstance()->getDisplayWidget();
    displaywidget->showSvg();

//    waitKey(0);
//    destroyAllWindows();

    if (hasValue) {
        analyse_results(results);
        return results;
    }

    return nullptr;
}

// TODO: Implement OCR for written responses
