//
// Created by williamwestwood on 26/05/24.
//

#include "SvgWidget.h"
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QPainter>

SvgWidget::SvgWidget(const QString &filePath, QWidget *parent)
    : QWidget(parent), renderer(nullptr) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    // Modify the SVG content
    QDomElement root = doc.documentElement();
    QDomElement path = root.firstChildElement("path");
    if (!path.isNull()) {
        path.setAttribute("fill", "#FF0000");  // Change fill color to red
    }

    // Set modified SVG to renderer
    QByteArray byteArray;
    QTextStream stream(&byteArray);
    doc.save(stream, 4);
    renderer = new QSvgRenderer(byteArray, this);
}

void SvgWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    if (renderer) {
        renderer->render(&painter);
    }
}
