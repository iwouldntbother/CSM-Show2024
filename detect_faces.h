//
// Created by williamwestwood on 29/05/24.
//

#ifndef DETECT_FACES_H
#define DETECT_FACES_H

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

// Function to detect faces in a given frame.
// Returns true if faces are detected, otherwise false.
// bool detect_faces(cv::Mat &frame);

// Function to start the face detection camera.
void start_face_cam();

#endif // DETECT_FACES_H
