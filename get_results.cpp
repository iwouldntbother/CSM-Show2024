//
// Created by williamwestwood on 22/05/24.
//

#include "get_results.h"

#include <bitset>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <string>

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

    // Debugging: Show the threshold and morph images
    // imshow("thresh.jpg", thresh);
    // imshow("morph.jpg", morph);

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

    // Debugging: Show the mask image
    // imshow("mask.jpg", mask);

    // Calculate the filled area within the circle
    Mat circleArea;
    bitwise_and(morph, mask, circleArea);

    // Debugging: Show the circle area image
    // imshow("circleArea.jpg", circleArea);

    double totalPixels = countNonZero(mask);
    double filledPixels = countNonZero(circleArea);

    double fillRatio = filledPixels / totalPixels;

    // cout << "Total Pixels in Circle: " << totalPixels << endl;
    // cout << "Filled Pixels in Circle: " << filledPixels << endl;
    // cout << "Fill Ratio: " << fillRatio << endl;

    // Determine if the circle is filled based on the threshold
    return fillRatio > fillThreshold;
}

double get_contrast(cv::Mat &image, Point p1, Point p2) {
    cv::Rect roi(p1.x, p1.y, p2.x-p1.x, p2.y-p1.y);
    cv::Mat roiImage = image(roi);


    cv::Mat grayROI;
    cv::cvtColor(roiImage, grayROI, cv::COLOR_BGR2GRAY);

    cv::Mat bwImage;
    double thresholdValue = 128;
    double maxValue = 255;
    cv::threshold(grayROI, bwImage, thresholdValue, maxValue, cv::THRESH_BINARY);


    cv::Scalar meanLightness = cv::mean(roiImage);

    // imshow("ROI: " + to_string(meanLightness[0]), bwImage);

    return meanLightness[0];
}

bool draw_box(cv::Mat &image, Point p1, Point p2) {
    // cout << "[INFO] Drawing " << p1.x << ", " << p1.y << " and " << p2.x << ", " << p2.y << endl;
    double lightness = get_contrast(image, p1, p2);
    rectangle(image, p1, p2, Scalar(255,0,0));
    if (lightness < 220) {
    putText(image, to_string(lightness), p1,
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
    } else {
        putText(image, to_string(lightness), p1,
                    FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
    }
    return lightness<220;
}

void display_bounding(cv::Mat &image) {
    const int image_width = image.cols;
    const int image_height = image.rows;

    cout << "[INFO] Image width " << image_width << endl;
    cout << "[INFO] Image height " << image_height << "\n" << endl;

    draw_box(
        image,
        Point(image_width * 0.5, image_height * 0.01),
        Point(image_width * 0.56, image_height * 0.02)
            );

    bool* results = new bool[40];

    // Loop through all points and draw boxes and lighness value.
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 4; ++k) {
                bool checked = draw_box(
                    image,
                    Point(static_cast<int>(std::round(image_width * points[i][j][k][0][0])), static_cast<int>(std::round(image_height * points[i][j][k][0][1]))),
                    Point(static_cast<int>(std::round(image_width * points[i][j][k][1][0])), static_cast<int>(std::round(image_height * points[i][j][k][1][1])))
                    );
                cout << "Q" << i+1 << ".";
                if(j>0){cout << "Against: ";}else{cout << "For: ";}
                cout << checked << endl;
                if (checked) {
                    results[i*8+j*4+k] = true;
                } else {
                    results[i*8+j*4+k] = false;
                }
                // cout << "" << endl;
            }
        }
    }

    cout << "[RESULTS] ";
    for (int i=0; i<40; i++) {
        cout << results[i] << " ";
    }
    cout << endl;

    imshow("Bounding boxes", image);
    waitKey(0);
    destroyAllWindows();
}

string get_resultsString(bool* &results) {
    string resultsString;
    for (int i = 0; i < 5; ++i) {
        resultsString += "Question " + to_string(i) + ". ";
        for (int j = 0; j < 2; ++j) {
            if (j == 0) {
                resultsString += "For: ";
            } else {
                resultsString += "Against: ";
            }
            for (int k = 0; k < 4; ++k) {
                if (results[i*8+j*4+k]) {
                    resultsString += "True, ";
                } else {
                    resultsString += "False, ";
                }
                // resultsString += results[i*8+j*4+k] + ", ";
            }
        }
    }
    return resultsString;
}

bool* get_results(cv::Mat &image) {
    const int image_width = image.cols;
    const int image_height = image.rows;

    double whiteLevel = get_contrast(
                    image,
                    Point(image_width * 0.5, image_height * 0.01),
                    Point(image_width * 0.56, image_height * 0.02)
                    );

    cout << "[White Level] " << whiteLevel << endl;

    bool* results = new bool[40];

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 4; ++k) {
                double lightness = get_contrast(
                    image,
                    Point(static_cast<int>(std::round(image_width * points[i][j][k][0][0])), static_cast<int>(std::round(image_height * points[i][j][k][0][1]))),
                    Point(static_cast<int>(std::round(image_width * points[i][j][k][1][0])), static_cast<int>(std::round(image_height * points[i][j][k][1][1])))
                    );
                // results.set(i*j*k) = lightness<220;
                cout << "[LIGHTNESS] " << lightness << endl;
                if (lightness < 220) {
                    results[i*8+j*4+k] = true;
                } else {
                    results[i*8+j*4+k] = false;
                }
            }
        }
    }

    // results.set(0);

    cout << "[RESULTS] ";
    for (int i=0; i<40; i++) {
        cout << results[i] << " ";
    }
    cout << endl;

    cout << get_resultsString(results) << endl;

    return results;
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

    // imshow("Circles", image);

    cout << "[RESULTS] ";
    for (int i=0; i<40; i++) {
        cout << results[i] << " ";
    }
    cout << endl;

    GlobalData::getInstance()->setResultsData(results);

    waitKey(0);
    destroyAllWindows();

    return results;
}

// int main() {
//     cv::Mat test_image = imread("/home/williamwestwood/Documents/test_image.jpg");
//
//     get_circle_results(test_image);
//
//     waitKey(0);
//     destroyAllWindows();
//     // get_results(test_image);
//
//     return 0;
// }