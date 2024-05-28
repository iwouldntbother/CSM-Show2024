//
// Created by williamwestwood on 22/05/24.
//

#ifndef GET_RESULTS_H
#define GET_RESULTS_H

#include <opencv2/opencv.hpp>
// #include <string.h>

using namespace cv;

bool* get_results(Mat &image);
void display_bounding(Mat &image);
bool* get_circle_results(Mat &image);

#endif //GET_RESULTS_H
