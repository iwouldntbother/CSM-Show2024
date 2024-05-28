//
// Created by williamwestwood on 23/05/24.
//



#include "displaywidget.h"
#include "ui_displaywidget.h"

// #include <QTimer>
#include <QFile>
#include <QSvgWidget>
#include <QFont>
#include <QFontDatabase>
#include "SvgWidget.h"
#include "SharedData.h"
#include "GlobalData.h"

using namespace std;

DisplayWidget::DisplayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayWidget),
    timer(new QTimer(this)),
    glProcessor(new GLImageProcessor(this))
{
    ui->setupUi(this);

    // Load the font from resources
    int fontId = QFontDatabase::addApplicationFont(":/assets/fonts/ChakraPetch-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);

    if (fontId == -1) {
        qWarning("Failed to load the font.");
    }

    SharedData sharedData;
    auto container = sharedData.getDataContainer();
    DisplayWidget::showResults(container->getResults());

    connect(timer, &QTimer::timeout, this, &DisplayWidget::updateFrame);
    timer->start(40); // Adjust the interval as needed


    showSvg("assets/images/form-tiny.svg");

    // Set fonts
    QFont font(fontFamily);
    ui->frameTitle->setFont(font);

    ui->logoLabel->setLineWidth(3);
    ui->progressLabel->setLineWidth(3);
    ui->frameLabel->setLineWidth(3);
    ui->faceLabel->setLineWidth(3);
    ui->statsLabel->setLineWidth(3);
    ui->resultsLabel->setLineWidth(3);

    ui->frameWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->frameWidget->setScaledContents(true);

    // UI Styling
    // glProcessor->setParent(ui->frameWidget);
    // glProcessor->setFixedSize(640, 480);

}

DisplayWidget::~DisplayWidget() {
    delete ui;
}

void DisplayWidget::updateFrame() {
    cv::Mat globalFrame = GlobalData::getInstance()->getFrameData();
    bool* results = GlobalData::getInstance()->getResultsData();

    showResults(results);

    if (!globalFrame.empty()) {
        showFrame(globalFrame);
    }
}

void DisplayWidget::showFrame(const cv::Mat &frame) {
    // cout << "Showing frame" << frame.cols << " x " << frame.rows << endl;
    // Convert frame to QImage
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
    cv::rotate(grayFrame, grayFrame, cv::ROTATE_90_CLOCKWISE);
    const QImage qimg(grayFrame.data, grayFrame.cols, grayFrame.rows, grayFrame.step, QImage::Format_Grayscale8);
    QImage scaled_frame = qimg.scaled(ui->frameWidget->size(), Qt::KeepAspectRatio);
    ui->frameWidget->setPixmap(QPixmap::fromImage(qimg.scaled(ui->frameWidget->size(), Qt::KeepAspectRatio)));
    // std::cout << "Frame updating" << std::endl;
    // glProcessor->updateFrame(frame);
}

void DisplayWidget::showResults(const bool* results) {

    string resultsString;

    for (int i = 0; i < 40; i++) {
        resultsString += to_string(results[i])+", ";
    }

    ui->resultsLabel->setText("Results: " + QString::fromStdString(resultsString));
}



void DisplayWidget::showSvg(const string &svg) {
    cout << "Showing SVG" << endl;
    QString qSvg = ":/" + QString::fromStdString(svg);

    bool* testResults = new bool[40];
    testResults[0] = true;

    ui->svgWidget->load(qSvg);
}