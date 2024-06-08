//
// Created by williamwestwood on 26/05/24.
//

#include "GlobalData.h"

#include "displaywidget.h"

GlobalData* GlobalData::instance = nullptr;  // Definition
std::mutex GlobalData::mtx;                  // Definition

GlobalData::GlobalData() : frameData(), resultsData(new bool[40]), ready(false) {}

GlobalData::~GlobalData() {
    delete[] resultsData;
}

GlobalData* GlobalData::getInstance() {
    std::lock_guard<std::mutex> lock(mtx);
    if (instance == nullptr) {
        instance = new GlobalData();
    }
    return instance;
}

void GlobalData::setFaceCameraIdx(int index) {
    std::lock_guard<std::mutex> lock(mtx);
    faceCameraIdx = index;
}

int GlobalData::getFaceCameraIdx() const {
    std::lock_guard<std::mutex> lock(mtx);
    return faceCameraIdx;
}

void GlobalData::setFormCameraIdx(int index) {
    std::lock_guard<std::mutex> lock(mtx);
    formCameraIdx = index;
}

int GlobalData::getFormCameraIdx() const {
    std::lock_guard<std::mutex> lock(mtx);
    return formCameraIdx;
}

void GlobalData::setDisplayWidget(DisplayWidget* widget) {
    std::lock_guard<std::mutex> lock(mtx);
    displayWidget = widget;
}

DisplayWidget* GlobalData::getDisplayWidget() const {
    std::lock_guard<std::mutex> lock(mtx);
    return displayWidget;
}

void GlobalData::setFrameData(const cv::Mat& newFrameData) {
    std::lock_guard<std::mutex> lock(mtx);
    frameData = newFrameData.clone();
}

cv::Mat GlobalData::getFrameData() const {
    std::lock_guard<std::mutex> lock(mtx);
    return frameData.clone();
}

void GlobalData::setResultsData(bool* value) {
    std::lock_guard<std::mutex> lock(mtx);
    resultsData = value;
}

bool* GlobalData::getResultsData() const {
    std::lock_guard<std::mutex> lock(mtx);
    return resultsData;
}

void GlobalData::setReadyStatus(const bool value) {
    std::lock_guard<std::mutex> lock(mtx);
    ready = value;
}

bool GlobalData::getReadyStatus() const {
    std::lock_guard<std::mutex> lock(mtx);
    return ready;
}

void GlobalData::setFaceFrame(const cv::Mat& newFaceFrame) {
    std::lock_guard<std::mutex> lock(mtx);
    faceFrame = newFaceFrame.clone();
}

cv::Mat GlobalData::getFaceFrame() const {
    std::lock_guard<std::mutex> lock(mtx);
    return faceFrame.clone();
}

void GlobalData::setFacesDetected(const bool value) {
    std::lock_guard<std::mutex> lock(mtx);
    facesDetected = value;
}

bool GlobalData::getFacesDetected() const {
    std::lock_guard<std::mutex> lock(mtx);
    return facesDetected;
}

void GlobalData::setAnalsysResults(const std::string& value) {
    std::lock_guard<std::mutex> lock(mtx);
    analysisResults = value;
}

std::string GlobalData::getAnalsysResults() const {
    std::lock_guard<std::mutex> lock(mtx);
    return analysisResults;
}

void GlobalData::setProgressText(const std::string& value) {
    std::lock_guard<std::mutex> lock(mtx);
    progressText = value;
}

std::string GlobalData::getProgressText() const {
    std::lock_guard<std::mutex> lock(mtx);
    return progressText;
}

void GlobalData::setShowProgress(const bool value) {
    std::lock_guard<std::mutex> lock(mtx);
    showProgress = value;
}

bool GlobalData::getShowProgress() const {
    std::lock_guard<std::mutex> lock(mtx);
    return showProgress;
}