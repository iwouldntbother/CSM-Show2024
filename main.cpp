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

#include "httplib/httplib.h"
#include "base64/base64.h"

#include "GlobalData.h"

using namespace std;
using namespace httplib;

int main(int argc, char *argv[]) {
    cout << "Hello :)" << endl;

    SharedData sharedData;
    auto container = sharedData.getDataContainer();

    std::array<bool, 40> boolArray{};
    boolArray.fill(true);

    container->setResults(boolArray.data());

    GlobalData::getInstance()->setResultsData(boolArray.data());

    std::thread DetectionThread(detect_markers);
    QApplication app(argc, argv);

    // DataContainer& dataContainer = DataContainer::getInstance();

    DisplayWidget displayWidget;
    displayWidget.show();

    return QApplication::exec();
    return 0;
}