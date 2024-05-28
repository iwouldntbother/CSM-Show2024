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

#include "GlobalData.h"

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
    boolArray.fill(true);

    GlobalData::getInstance()->setResultsData(boolArray.data());

    std::thread DetectionThread(detect_markers);
    QApplication app(argc, argv);

    DisplayWidget displayWidget;
    displayWidget.show();

    return QApplication::exec();
}