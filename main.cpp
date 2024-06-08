//
// Created by williamwestwood on 22/05/24.
//

#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <fstream>
#include <QApplication>
#include <opencv2/imgcodecs.hpp>

#include "displaywidget.h"
#include "detect_markers.h"
#include "SharedData.h"
#include <thread>

#include "detect_faces.h"
#include "GlobalData.h"

#include "analyse_results.h"

using namespace std;

std::string exec_cmd(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::map<std::string, int> parseDeviceIndecies(const std::string& deviceList) {
    std::map<std::string, int> cameraIndecies;
    std::istringstream stream(deviceList);
    std::string line;
    std::regex deviceRegex("/dev/video(\\d+)");
    std:string currentCamera;

    while (std::getline(stream, line)) {
        if (line.back() ==':') {
            currentCamera = line.substr(0, line.size() - 1);
        } else {
            std::smatch match;
            if (std::regex_search(line, match, deviceRegex)) {
                if (!currentCamera.empty()) {
                    cameraIndecies[currentCamera] = std::stoi(match[1]);
                    currentCamera.clear();
                }
            }
        }
    }
    return cameraIndecies;
}


int main(int argc, char *argv[]) {
    cout << R"(
██╗    ██╗██╗██╗     ██╗     ██╗ █████╗ ███╗   ███╗    ██╗    ██╗███████╗███████╗████████╗██╗    ██╗ ██████╗  ██████╗ ██████╗
██║    ██║██║██║     ██║     ██║██╔══██╗████╗ ████║    ██║    ██║██╔════╝██╔════╝╚══██╔══╝██║    ██║██╔═══██╗██╔═══██╗██╔══██╗
██║ █╗ ██║██║██║     ██║     ██║███████║██╔████╔██║    ██║ █╗ ██║█████╗  ███████╗   ██║   ██║ █╗ ██║██║   ██║██║   ██║██║  ██║
██║███╗██║██║██║     ██║     ██║██╔══██║██║╚██╔╝██║    ██║███╗██║██╔══╝  ╚════██║   ██║   ██║███╗██║██║   ██║██║   ██║██║  ██║
╚███╔███╔╝██║███████╗███████╗██║██║  ██║██║ ╚═╝ ██║    ╚███╔███╔╝███████╗███████║   ██║   ╚███╔███╔╝╚██████╔╝╚██████╔╝██████╔╝
 ╚══╝╚══╝ ╚═╝╚══════╝╚══════╝╚═╝╚═╝  ╚═╝╚═╝     ╚═╝     ╚══╝╚══╝ ╚══════╝╚══════╝   ╚═╝    ╚══╝╚══╝  ╚═════╝  ╚═════╝ ╚═════╝
    )" << endl;

    //std::cout << "Webcams:" << std::endl;
    //std::cout << exec_cmd("v4l2-ctl --list-devices") << endl;

    try {
        std::string result = exec_cmd("v4l2-ctl --list-devices");

        std::map<std::string, int> cameraIndecies = parseDeviceIndecies(result);

        for (const auto& camera : cameraIndecies) {
            std::cout << "Camera: " << camera.first << ", Index: " << camera.second << std::endl;
// if camera.first contains 'Full HD', set it's index as faceCamIdx
// if camera.first contains 'USB Camera', set it's index as formCamIdx
// TODO: Add above cameraIdxVars to GlobalData
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::array<bool, 40> boolArray{};
    boolArray.fill(false);

    GlobalData::getInstance()->setResultsData(boolArray.data());

    std::thread DetectionThread(detect_markers);
    std::thread FaceDetectionThread(start_face_cam);

    QApplication app(argc, argv);

    DisplayWidget displayWidget;
    GlobalData::getInstance()->setDisplayWidget(&displayWidget);
    displayWidget.show();

    return QApplication::exec();
}

// TODO: Add SQLLite database to store results
// sudo dnf install sqlite sqlite-devel
// ${SQLite3_LIBRARIES} ${SQLite3_INCLUDE_DIRS} SQLite3vscode


// TODO: Read camera devices and set index acordingly
// v4l2-ctl --list-devices
// Output:
// UVC Camera (046d:08ca) (usb-0000:00:14.0-1):
// /dev/video0
// /dev/video1
// /dev/media0
