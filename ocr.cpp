//
// Created by williamwestwood on 22/05/24.
//

#include "ocr.h"
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>  // For Tesseract

void ocr(cv::Mat &image) {

    // Load image using OpenCV
    // cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        // return -1;
    }

    // Convert the image to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Initialize Tesseract
    tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();
    if (ocr->Init(nullptr, "eng", tesseract::OEM_LSTM_ONLY)) {
        std::cerr << "Could not initialize tesseract." << std::endl;
        // return -1;
    }

    // Set the image for Tesseract to process
    ocr->SetImage(gray.data, gray.cols, gray.rows, gray.channels(), gray.step);

    // Get OCR result
    char* text = ocr->GetUTF8Text();
    std::cout << "OCR output:\n" << text << std::endl;

    // Cleanup
    delete[] text;
    ocr->End();

    // return 0;
}
