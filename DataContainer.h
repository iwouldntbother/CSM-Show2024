//
// Created by williamwestwood on 24/05/24.
//

#ifndef DATACONTAINER_H
#define DATACONTAINER_H
#include <opencv2/core/mat.hpp>
#include <shared_mutex>

class DataContainer {
private:
    cv::Mat frame;
    bool* results;
    mutable std::shared_mutex mtx;

public:
    DataContainer() : frame(), results(new bool[40]) {}

    void setFrameData(const cv::Mat &image) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        std::cout << "Frame data recieved: " << image.size() << std::endl;
        this->frame = image.clone();
        // std::cout << "Frame data set: " << this->frame.size() << std::endl;
    }

    cv::Mat getFrameData() {
        std::shared_lock<std::shared_mutex> lock(mtx);
        std::cout << "Frame data sent: " << frame.size() << std::endl;
        return frame.clone();
    }

    void setResults(const bool* results) const {
        for (int i = 0; i < 40; i++) {
            this->results[i] = results[i];
        }
    }

    bool* getResults() const {
        return results;
    }

private:

};

#endif //DATACONTAINER_H
