// GlobalData.h
#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <mutex>
#include <opencv2/opencv.hpp>

class GlobalData {
private:
    static GlobalData* instance;  // Declaration
    static std::mutex mtx;        // Declaration
    cv::Mat frameData;
    bool* resultsData;
    bool ready;

    // Private constructor
    GlobalData();

public:
    ~GlobalData();

    // Delete copy constructor and assignment operator
    GlobalData(const GlobalData&) = delete;
    GlobalData& operator=(const GlobalData&) = delete;

    // Singleton access method
    static GlobalData* getInstance();

    // Data access methods
    void setFrameData(const cv::Mat& newFrameData);
    [[nodiscard]] cv::Mat getFrameData() const;
    void setResultsData(bool* value);
    [[nodiscard]] bool* getResultsData() const;
    void setReadyStatus(bool value);
    [[nodiscard]] bool getReadyStatus() const;
};

#endif // GLOBALDATA_H
