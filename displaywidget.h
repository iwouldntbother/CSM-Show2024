//
// Created by williamwestwood on 23/05/24.
//

#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include <QTimer>
#include "ui_displaywidget.h"
#include <opencv2/opencv.hpp>

#include "DataContainer.h"
#include "GLImageProcessor.h"

namespace Ui {
    class DisplayWidget;
}

class DisplayWidget : public QWidget {
    Q_OBJECT

public:
    explicit DisplayWidget(QWidget *parent = nullptr);
    ~DisplayWidget();

    private slots:
        void updateFrame();

private:
    Ui::DisplayWidget *ui;
    QTimer *timer;
    GLImageProcessor *glProcessor;
    void showFrame(const cv::Mat &frame);

    void showFace(const cv::Mat &frame) const;

    void showResults(const bool *results) const;

    void showSvg(const std::string &svg) const;
};

#endif // DISPLAYWIDGET_H

