//
// Created by williamwestwood on 22/05/24.
//

#ifndef OCR_H
#define OCR_H

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

using namespace cv;
using namespace std;

void ocr(cv::Mat &image);

#endif //OCR_H
