//
// Created by williamwestwood on 23/05/24.
//



#include "displaywidget.h"
#include "ui_displaywidget.h"

// #include <QTimer>
#include <qdom.h>
#include <QFile>
#include <QSvgWidget>
#include <QFont>
#include <QFontDatabase>
#include <QPainter>
#include <QImage>
#include <QTextStream>
#include <QRegularExpression>

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

    this->setCursor(Qt::BlankCursor);

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


    showSvg();

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
    //glProcessor->setParent(ui->faceWidget);
    //glProcessor->setFixedSize(504, 504);
    // const QImage logo = QImage::load(":/assets/images/print_logo.png");

    ui->logoLabel->setPixmap(QPixmap(":/assets/images/logo.png"));
    // ui->logoLabel->setPixmap(QPixmap::fromImage(logo.scaled(ui->logoLabel->size(), Qt::KeepAspectRatio)));
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

    DisplayWidget::updateSystemStats();
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
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
    const QImage qimg(grayFrame.data, grayFrame.cols, grayFrame.rows, grayFrame.step, QImage::Format_Grayscale8);
    ui->faceWidget->setPixmap(QPixmap::fromImage(qimg.scaled(ui->faceWidget->size(), Qt::KeepAspectRatio)));
    //glProcessor->updateFrame(resized_frame);
}

void DisplayWidget::showResults(const bool* results) const {

// TODO: Implement this function to actually show results on svg yk

    string resultsString;

    for (int i = 0; i < 40; i++) {
        resultsString += to_string(results[i])+", ";
    }

    ui->resultsLabel->setText("Results: " + QString::fromStdString(resultsString));
}


void DisplayWidget::showSvg() const {
    // Convert the file path to QString
    QString const qSvgPath = ":/" + QString::fromStdString("assets/images/form-tiny.svg");

    // Load the SVG file into a QByteArray
    QFile file(qSvgPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Cannot open file");
        return;
    }
    QByteArray const svgData = file.readAll();

    // Create an SVG renderer
    QSvgRenderer renderer(svgData);

    // Modify the SVG data if needed
    // For example, changing the color of an element with id 'elementId'
    QDomDocument doc;
    if (!doc.setContent(svgData)) {
        qWarning("Cannot parse SVG content");
        return;
    }

    const bool* results = GlobalData::getInstance()->getResultsData();

    QDomElement resultsGroup = doc.documentElement().firstChildElement("g");
    if (resultsGroup.isNull() || resultsGroup.attribute("id") != "resultsGroup") {
        qDebug() << "Results group not found or incorrect ID";
        return;
    }

    QDomElement path = resultsGroup.firstChildElement("path");
    while (!path.isNull()) {
        QString pathId = path.attribute("id");
        bool condition = results[pathId.remove("result").toInt()];
        QString newColor = condition ? "#000" : "#fff"; // Black if condition is true, otherwise White

        path.setAttribute("fill", newColor);
        path = path.nextSiblingElement("path");
    }


    // Convert the modified QDomDocument back to QByteArray
    QByteArray modifiedSvg;
    QTextStream stream(&modifiedSvg);
    doc.save(stream, 4); // Save the document back to a QByteArray

    // Update the renderer with modified SVG
    renderer.load(modifiedSvg);

    // Render the modified SVG on the widget
    QImage image(ui->svgWidget->size(), QImage::Format_ARGB32);
    QPainter painter(&image);
    renderer.render(&painter);

    // Set the QImage on a QLabel or convert it to a QPixmap to set on existing widgets
    ui->svgWidget->setPixmap(QPixmap::fromImage(image.scaled(ui->svgWidget->size(), Qt::KeepAspectRatio)));
}

QRegularExpression const memorySplitRegEx = QRegularExpression("\\s+");

int parseMemoryLine(const QString &line) {
    return line.split(memorySplitRegEx, Qt::SkipEmptyParts)[1].toInt();
}

void DisplayWidget::updateSystemStats() const {
    if (QFile file("/proc/stat"); file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        const QString line = stream.readLine();
        file.close();

        if (QStringList values = line.split(memorySplitRegEx, Qt::SkipEmptyParts); values.size() > 4) {
            const long long user = values[1].toLongLong();
            const long long nice = values[2].toLongLong();
            const long long system = values[3].toLongLong();
            const long long idle = values[4].toLongLong();

            const long long total = user + nice + system + idle;
            const long long idleTime = idle;

            static long long prevTotal = 0;
            static long long prevIdle = 0;

            const long long totalDelta = total - prevTotal;
            const long long idleDelta = idleTime - prevIdle;

            const double cpuUsage = 100.0 * (totalDelta - idleDelta) / totalDelta;

            ui->cpuLabel->setText(QString("CPU Usage: %1%").arg(cpuUsage, 0, 'f', 2));
            ui->cpuBar->setValue(static_cast<int>(cpuUsage));

            prevTotal = total;
            prevIdle = idleTime;
        }

    }

    if (QFile memFile("/proc/meminfo"); memFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&memFile);
        const QString totalLine = stream.readLine();
        const QString freeLine = stream.readLine();
        memFile.close();

        const int totalMemory = parseMemoryLine(totalLine);
        const int freeMemory = parseMemoryLine(freeLine);

        const int usedMemory = totalMemory - freeMemory;

        const double memUsagePercentage = (static_cast<double>(usedMemory) / totalMemory) * 100.0;

        ui->memLabel->setText(QString("Memory Usage: %1 MB Used").arg(usedMemory / 1024));
        ui->memBar->setValue(static_cast<int>(memUsagePercentage));
    }

    if (QFile upFile("/proc/uptime"); upFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&upFile);
        const QString uptime = stream.readLine();
        upFile.close();

        const int uptimeSeconds = uptime.split(" ")[0].toInt();
        const int uptimeMinutes = uptimeSeconds / 60;
        const int uptimeHours = uptimeMinutes / 60;
        const int uptimeDays = uptimeHours / 24;

        ui->uptimeLabel->setText(QString("Uptime: %1 days, %2 hours, %3 minutes")
                                 .arg(uptimeDays).arg(uptimeHours % 24).arg(uptimeMinutes % 60));
    }
}


