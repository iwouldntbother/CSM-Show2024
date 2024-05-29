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

    // SharedData sharedData;
    // auto container = sharedData.getDataContainer();
    // DisplayWidget::showResults(container->getResults());

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
    glProcessor->setParent(ui->faceWidget);
    glProcessor->setFixedSize(504, 504);

}

DisplayWidget::~DisplayWidget() {
    delete ui;
}

void DisplayWidget::updateFrame() {
    const cv::Mat globalFrame = GlobalData::getInstance()->getFrameData();
    const bool* results = GlobalData::getInstance()->getResultsData();
    const cv::Mat faceFrame = GlobalData::getInstance()->getFaceFrame();

    showResults(results);

    if (!globalFrame.empty()) {
        showFrame(globalFrame);
    }

    if (!faceFrame.empty()) {
        showFace(faceFrame);
    }
}

void DisplayWidget::showFrame(const cv::Mat &frame) {
    // cout << "Showing frame" << frame.cols << " x " << frame.rows << endl;
    // Convert frame to QImage
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
    cv::rotate(grayFrame, grayFrame, cv::ROTATE_90_CLOCKWISE);
    const QImage qimg(grayFrame.data, grayFrame.cols, grayFrame.rows, grayFrame.step, QImage::Format_Grayscale8);
    // QImage scaled_frame = qimg.scaled(ui->frameWidget->size(), Qt::KeepAspectRatio);
    ui->frameWidget->setPixmap(QPixmap::fromImage(qimg.scaled(ui->frameWidget->size(), Qt::KeepAspectRatio)));
    // std::cout << "Frame updating" << std::endl;
}

void DisplayWidget::showFace(const cv::Mat &frame) const {
    cv::rotate(frame, frame, cv::ROTATE_180);
    cv::Mat resized_frame;
    cv::resize(frame, resized_frame, cv::Size(504,504), 0, 0, cv::INTER_LINEAR);
    glProcessor->updateFrame(resized_frame);
}

void DisplayWidget::showResults(const bool* results) const {

// TODO: Implement this function to actually show results on svg yk

    string resultsString;

    for (int i = 0; i < 40; i++) {
        resultsString += to_string(results[i])+", ";
    }

    ui->resultsLabel->setText("Results: " + QString::fromStdString(resultsString));
}



void DisplayWidget::showSvg(const string &svg) const {
    // cout << "Showing SVG" << endl;
    const QString qSvg = ":/" + QString::fromStdString(svg);

    ui->svgWidget->load(qSvg);
}