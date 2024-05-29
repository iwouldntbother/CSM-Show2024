//
// Created by williamwestwood on 29/05/24.
//

#include "detect_faces.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
// #include <iostream>
// #include <filesystem>

#include <thread>

#include "GlobalData.h"

using namespace cv;

Mat cropToSquare(const Mat &frame) {
    int dimension = std::min(frame.cols, frame.rows); // Get the smaller of the width or height
    int x_start = (frame.cols - dimension) / 2;       // Center the square horizontally
    int y_start = (frame.rows - dimension) / 2;       // Center the square vertically
    Rect roi(x_start, y_start, dimension, dimension); // Define the square region of interest
    return frame(roi); // Crop and return the square image
}

bool detect_faces(Mat &frame) {

    // std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    CascadeClassifier face_cascade;
    face_cascade.load("assets/haarcascades/haarcascade_frontalface_alt.xml");
    // if (!face_cascade.load("assets/haarcascades/haarcascade_frontalface_alt.xml")) {
    //     std::cout << "Failed to load face cascade" << std::endl;
    //     return false;
    // };

    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    face_cascade.detectMultiScale(frame_gray, faces);

    for (const auto face : faces) {
        rectangle(frame, face, Scalar(255, 255, 255), 2);
    }

    // if (!faces.empty()) {
    //     std::cout << "Faces detected: " << faces.size() << std::endl;
    // }

    GlobalData::getInstance()->setFaceFrame(frame);

    return !faces.empty();
}

void start_face_cam() {
    VideoCapture cap(2, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return;
    }

    while (true) {
        Mat frame;
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "No captured frame" << std::endl;
            break;
        }

        frame = cropToSquare(frame);

        // std::cout << "Cropped frame size:" << frame.size() << std::endl;

        // GlobalData::getInstance()->setFaceFrame(frame);

        GlobalData::getInstance()->setFacesDetected(detect_faces(frame));


        // imshow("Face Detection", frame);

        // if (waitKey(10) == 27) {
        //     break;
        // }
    }
    // cap.release();
    // destroyAllWindows();
}