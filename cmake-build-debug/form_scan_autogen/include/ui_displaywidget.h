/********************************************************************************
** Form generated from reading UI file 'displaywidget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYWIDGET_H
#define UI_DISPLAYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DisplayWidget
{
public:
    QLabel *logoLabel;
    QLabel *progressLabel;
    QLabel *frameTitle;
    QLabel *frameLabel;
    QLabel *frameWidget;
    QLabel *faceTitle;
    QLabel *faceLabel;
    QLabel *faceWidget;
    QLabel *statsTitle;
    QLabel *statsLabel;
    QLabel *cpuLabel;
    QProgressBar *cpuBar;
    QLabel *memLabel;
    QProgressBar *memBar;
    QLabel *uptimeLabel;
    QLabel *resultsTitle;
    QLabel *resultsLabel;
    QLabel *svgWidget;

    void setupUi(QWidget *DisplayWidget)
    {
        if (DisplayWidget->objectName().isEmpty())
            DisplayWidget->setObjectName("DisplayWidget");
        DisplayWidget->resize(1920, 1080);
        DisplayWidget->setStyleSheet(QString::fromUtf8("\n"
"    #DisplayWidget {\n"
"    background-color: rgb(14, 14, 14);\n"
"    }\n"
"\n"
"    QLabel {\n"
"    color: rgb(255, 255, 255);\n"
"    font: 75 12pt \"Roboto Mono\";\n"
"    }\n"
"\n"
"\n"
"    QProgressBar {\n"
"    border: none;\n"
"    border-radius: 0px;\n"
"    background-color: rgb(14, 14, 14);\n"
"    }\n"
"\n"
"    QProgressBar::chunk {\n"
"    background-color: #FFFFFF;\n"
"    width: 20px;\n"
"    margin: 2px;\n"
"    }\n"
"\n"
"   "));
        logoLabel = new QLabel(DisplayWidget);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setGeometry(QRect(20, 20, 297, 89));
        logoLabel->setFrameShape(QFrame::Shape::Box);
        progressLabel = new QLabel(DisplayWidget);
        progressLabel->setObjectName("progressLabel");
        progressLabel->setGeometry(QRect(367, 20, 1533, 89));
        progressLabel->setFrameShape(QFrame::Shape::Box);
        progressLabel->setStyleSheet(QString::fromUtf8("\n"
"     font-weight: 500;\n"
"     text-transform: uppercase;\n"
"     font-size: 23pt;\n"
"    "));
        frameTitle = new QLabel(DisplayWidget);
        frameTitle->setObjectName("frameTitle");
        frameTitle->setGeometry(QRect(20, 130, 419, 36));
        frameTitle->setFrameShape(QFrame::Shape::Box);
        frameTitle->setStyleSheet(QString::fromUtf8("\n"
"     background-color: rgb(255, 255, 255);\n"
"     color: rgb(0, 0, 0);\n"
"     font-weight: 600;\n"
"     text-transform: uppercase;\n"
"    "));
        frameLabel = new QLabel(DisplayWidget);
        frameLabel->setObjectName("frameLabel");
        frameLabel->setGeometry(QRect(20, 179, 633, 881));
        frameLabel->setFrameShape(QFrame::Shape::Box);
        frameWidget = new QLabel(frameLabel);
        frameWidget->setObjectName("frameWidget");
        frameWidget->setGeometry(QRect(5, 5, 623, 871));
        faceTitle = new QLabel(DisplayWidget);
        faceTitle->setObjectName("faceTitle");
        faceTitle->setGeometry(QRect(703, 128, 419, 36));
        faceTitle->setFrameShape(QFrame::Shape::Box);
        faceTitle->setStyleSheet(QString::fromUtf8("\n"
"     background-color: rgb(255, 255, 255);\n"
"     color: rgb(0, 0, 0);\n"
"     font-weight: 600;\n"
"     text-transform: uppercase;\n"
"    "));
        faceLabel = new QLabel(DisplayWidget);
        faceLabel->setObjectName("faceLabel");
        faceLabel->setGeometry(QRect(703, 179, 514, 514));
        faceLabel->setFrameShape(QFrame::Shape::Box);
        faceWidget = new QLabel(faceLabel);
        faceWidget->setObjectName("faceWidget");
        faceWidget->setGeometry(QRect(5, 5, 504, 504));
        statsTitle = new QLabel(DisplayWidget);
        statsTitle->setObjectName("statsTitle");
        statsTitle->setGeometry(QRect(703, 710, 419, 36));
        statsTitle->setFrameShape(QFrame::Shape::Box);
        statsTitle->setStyleSheet(QString::fromUtf8("\n"
"     background-color: rgb(255, 255, 255);\n"
"     color: rgb(0, 0, 0);\n"
"     font-weight: 600;\n"
"     text-transform: uppercase;\n"
"    "));
        statsLabel = new QLabel(DisplayWidget);
        statsLabel->setObjectName("statsLabel");
        statsLabel->setGeometry(QRect(703, 763, 514, 297));
        statsLabel->setFrameShape(QFrame::Shape::Box);
        cpuLabel = new QLabel(statsLabel);
        cpuLabel->setObjectName("cpuLabel");
        cpuLabel->setGeometry(QRect(10, 0, 504, 45));
        cpuLabel->setStyleSheet(QString::fromUtf8("\n"
"       font-weight: 400;\n"
"       font-size: 16pt;\n"
"       text-transform: uppercase;\n"
"      "));
        cpuBar = new QProgressBar(statsLabel);
        cpuBar->setObjectName("cpuBar");
        cpuBar->setGeometry(QRect(10, 45, 494, 40));
        cpuBar->setValue(0);
        cpuBar->setMinimum(0);
        cpuBar->setMaximum(100);
        cpuBar->setTextVisible(false);
        memLabel = new QLabel(statsLabel);
        memLabel->setObjectName("memLabel");
        memLabel->setGeometry(QRect(10, 99, 504, 45));
        memLabel->setStyleSheet(QString::fromUtf8("\n"
"       font-weight: 400;\n"
"       font-size: 16pt;\n"
"       text-transform: uppercase;\n"
"      "));
        memBar = new QProgressBar(statsLabel);
        memBar->setObjectName("memBar");
        memBar->setGeometry(QRect(10, 144, 494, 40));
        memBar->setValue(0);
        memBar->setMinimum(0);
        memBar->setMaximum(100);
        memBar->setTextVisible(false);
        uptimeLabel = new QLabel(statsLabel);
        uptimeLabel->setObjectName("uptimeLabel");
        uptimeLabel->setGeometry(QRect(5, 198, 504, 45));
        resultsTitle = new QLabel(DisplayWidget);
        resultsTitle->setObjectName("resultsTitle");
        resultsTitle->setGeometry(QRect(1267, 128, 419, 36));
        resultsTitle->setFrameShape(QFrame::Shape::Box);
        resultsTitle->setStyleSheet(QString::fromUtf8("\n"
"     background-color: rgb(255, 255, 255);\n"
"     color: rgb(0, 0, 0);\n"
"     font-weight: 600;\n"
"     text-transform: uppercase;\n"
"    "));
        resultsLabel = new QLabel(DisplayWidget);
        resultsLabel->setObjectName("resultsLabel");
        resultsLabel->setGeometry(QRect(1267, 179, 633, 881));
        resultsLabel->setFrameShape(QFrame::Shape::Box);
        svgWidget = new QLabel(resultsLabel);
        svgWidget->setObjectName("svgWidget");
        svgWidget->setGeometry(QRect(5, 5, 623, 871));

        retranslateUi(DisplayWidget);

        QMetaObject::connectSlotsByName(DisplayWidget);
    } // setupUi

    void retranslateUi(QWidget *DisplayWidget)
    {
        DisplayWidget->setWindowTitle(QCoreApplication::translate("DisplayWidget", "William Westwood - CSM2024", nullptr));
        logoLabel->setText(QCoreApplication::translate("DisplayWidget", "Hello :)", nullptr));
        progressLabel->setText(QCoreApplication::translate("DisplayWidget", "Harvesting emotional data for collective good\342\200\246 Individual sentiment is secondary.", nullptr));
        frameTitle->setText(QCoreApplication::translate("DisplayWidget", "Vision - Form Scanner Preview", nullptr));
        faceTitle->setText(QCoreApplication::translate("DisplayWidget", "Vision - Facial Recognition", nullptr));
        faceLabel->setText(QCoreApplication::translate("DisplayWidget", "Face Camera", nullptr));
        statsTitle->setText(QCoreApplication::translate("DisplayWidget", "Data - System Statistics", nullptr));
        resultsTitle->setText(QCoreApplication::translate("DisplayWidget", "Data - Form Results", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DisplayWidget: public Ui_DisplayWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYWIDGET_H
