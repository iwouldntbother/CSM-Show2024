//
// Created by williamwestwood on 20/05/24.
//

#ifndef ALIGN_AND_CROP_H
#define ALIGN_AND_CROP_H

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <vector>

using namespace cv;
using namespace std;

void align_and_crop(Mat &image, const Ptr<aruco::Dictionary>& arucoDict, const Ptr<aruco::DetectorParameters>& arucoParams, bool rotateImage);

#endif // ALIGN_AND_CROP_H
