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
#include <QtSvgWidgets/QSvgWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
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
    QLabel *faceLabel;
    QLabel *faceWidget;
    QLabel *statsLabel;
    QLabel *resultsLabel;
    QSvgWidget *svgWidget;

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
"   "));
        logoLabel = new QLabel(DisplayWidget);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setGeometry(QRect(20, 20, 297, 89));
        logoLabel->setFrameShape(QFrame::Shape::Box);
        progressLabel = new QLabel(DisplayWidget);
        progressLabel->setObjectName("progressLabel");
        progressLabel->setGeometry(QRect(367, 20, 1533, 89));
        progressLabel->setFrameShape(QFrame::Shape::Box);
        frameTitle = new QLabel(DisplayWidget);
        frameTitle->setObjectName("frameTitle");
        frameTitle->setGeometry(QRect(20, 130, 419, 36));
        frameTitle->setFrameShape(QFrame::Shape::Box);
        frameTitle->setStyleSheet(QString::fromUtf8("\n"
"     background-color: rgb(255, 255, 255);\n"
"     color: rgb(0, 0, 0);\n"
"    "));
        frameLabel = new QLabel(DisplayWidget);
        frameLabel->setObjectName("frameLabel");
        frameLabel->setGeometry(QRect(20, 179, 633, 881));
        frameLabel->setFrameShape(QFrame::Shape::Box);
        frameWidget = new QLabel(frameLabel);
        frameWidget->setObjectName("frameWidget");
        frameWidget->setGeometry(QRect(5, 5, 623, 871));
        faceLabel = new QLabel(DisplayWidget);
        faceLabel->setObjectName("faceLabel");
        faceLabel->setGeometry(QRect(703, 179, 514, 514));
        faceLabel->setFrameShape(QFrame::Shape::Box);
        faceWidget = new QLabel(faceLabel);
        faceWidget->setObjectName("faceWidget");
        faceWidget->setGeometry(QRect(5, 5, 504, 504));
        statsLabel = new QLabel(DisplayWidget);
        statsLabel->setObjectName("statsLabel");
        statsLabel->setGeometry(QRect(703, 763, 514, 297));
        statsLabel->setFrameShape(QFrame::Shape::Box);
        resultsLabel = new QLabel(DisplayWidget);
        resultsLabel->setObjectName("resultsLabel");
        resultsLabel->setGeometry(QRect(1267, 179, 633, 881));
        resultsLabel->setFrameShape(QFrame::Shape::Box);
        svgWidget = new QSvgWidget(resultsLabel);
        svgWidget->setObjectName("svgWidget");
        svgWidget->setGeometry(QRect(5, 5, 623, 871));

        retranslateUi(DisplayWidget);

        QMetaObject::connectSlotsByName(DisplayWidget);
    } // setupUi

    void retranslateUi(QWidget *DisplayWidget)
    {
        DisplayWidget->setWindowTitle(QCoreApplication::translate("DisplayWidget", "William Westwood - CSM2024", nullptr));
        logoLabel->setText(QCoreApplication::translate("DisplayWidget", "Hello :)", nullptr));
        progressLabel->setText(QCoreApplication::translate("DisplayWidget", "Progress Prompts", nullptr));
        frameTitle->setText(QCoreApplication::translate("DisplayWidget", "Vision - Form Scanner Preview", nullptr));
        faceLabel->setText(QCoreApplication::translate("DisplayWidget", "Face Camera", nullptr));
        statsLabel->setText(QCoreApplication::translate("DisplayWidget", "System Stats", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DisplayWidget: public Ui_DisplayWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYWIDGET_H
