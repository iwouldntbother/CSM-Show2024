//
// Created by williamwestwood on 26/05/24.
//

#include "GlobalData.h"

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
