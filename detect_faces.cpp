//
// Created by williamwestwood on 29/05/24.
//

#include "detect_faces.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <thread>

#include "GlobalData.h"

using namespace cv;

Mat cropToSquare(const Mat &frame) {
    const int dimension = std::min(frame.cols, frame.rows); // Get the smaller of the width or height
    const int x_start = (frame.cols - dimension) / 2;       // Center the square horizontally
    const int y_start = (frame.rows - dimension) / 2;       // Center the square vertically
    const Rect roi(x_start, y_start, dimension, dimension); // Define the square region of interest
    return frame(roi); // Crop and return the square image
}

CascadeClassifier global_face_cascade;


bool detect_faces(Mat &frame) {

//	std::cout << "[detect_faces] Loading haarcascade" << std::endl;

//    CascadeClassifier face_cascade;
//    if (!face_cascade.load("/home/admin/form-scan/debug/assets/haarcascades/haarcascade_frontalface_alt.xml")) {
//	std::cout << "[detect_faces] haarcascade load failed" << std::endl;
//    }

//	std::cout << "[detect_faces] Loaded haarcascade" << std::endl;

    std::vector<Rect> faces;
    Mat frame_gray;

//	std::cout << "[detect_faces] Converting frame to gray" << std::endl;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

//	std::cout << "[detect_faces] Detecting faces" << std::endl;

// TODO: You commented the line bellow out to improve performance btw ;*

    //global_face_cascade.detectMultiScale(frame_gray, faces);

//	std::cout << "[detect_faces] Looping through faces" << std::endl;

    for (const auto face : faces) {
        rectangle(frame, face, Scalar(255, 255, 255), 2);
    }

//	std::cout << "[detect_faces] Pushing frame to GlobalData" << std::endl;

    GlobalData::getInstance()->setFaceFrame(frame);

    return !faces.empty();
}

void start_face_cam() {
    int cameraIdx = GlobalData::getInstance()->GlobalData::getFaceCameraIdx();
    std::cout << "[INFO] Using camera at index: " << cameraIdx << " For face camera" << std::endl;
    VideoCapture cap(cameraIdx, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return;
    }

    if (!global_face_cascade.load("/home/admin/form-scan/build/assets/haarcascades/haarcascade_frontalface_alt.xml")) {
        std::cout << "[Error] Global face cascade failed to load" << std::endl;
    }

    while (true) {
        Mat frame;
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "No captured frame" << std::endl;
            break;
        }

        frame = cropToSquare(frame);

        GlobalData::getInstance()->setFacesDetected(detect_faces(frame));
    }
}
