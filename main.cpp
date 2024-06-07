//
// Created by williamwestwood on 22/05/24.
//

#include <iostream>
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


int main(int argc, char *argv[]) {
    cout << R"(
██╗    ██╗██╗██╗     ██╗     ██╗ █████╗ ███╗   ███╗    ██╗    ██╗███████╗███████╗████████╗██╗    ██╗ ██████╗  ██████╗ ██████╗
██║    ██║██║██║     ██║     ██║██╔══██╗████╗ ████║    ██║    ██║██╔════╝██╔════╝╚══██╔══╝██║    ██║██╔═══██╗██╔═══██╗██╔══██╗
██║ █╗ ██║██║██║     ██║     ██║███████║██╔████╔██║    ██║ █╗ ██║█████╗  ███████╗   ██║   ██║ █╗ ██║██║   ██║██║   ██║██║  ██║
██║███╗██║██║██║     ██║     ██║██╔══██║██║╚██╔╝██║    ██║███╗██║██╔══╝  ╚════██║   ██║   ██║███╗██║██║   ██║██║   ██║██║  ██║
╚███╔███╔╝██║███████╗███████╗██║██║  ██║██║ ╚═╝ ██║    ╚███╔███╔╝███████╗███████║   ██║   ╚███╔███╔╝╚██████╔╝╚██████╔╝██████╔╝
 ╚══╝╚══╝ ╚═╝╚══════╝╚══════╝╚═╝╚═╝  ╚═╝╚═╝     ╚═╝     ╚══╝╚══╝ ╚══════╝╚══════╝   ╚═╝    ╚══╝╚══╝  ╚═════╝  ╚═════╝ ╚═════╝
    )" << endl;


    std::array<bool, 40> boolArray{};
    boolArray.fill(false);
    // boolArray[0] = true;
    // boolArray[3] = true;

    // cout << "Analyzing results..." << endl;
    // std::string const analysis = analyse_results(boolArray.data());
    // cout << "Reply from analysis:" << endl;
    // cout << analysis << endl;

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
