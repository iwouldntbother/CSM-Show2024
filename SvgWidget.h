#ifndef SVGWIDGET_H
#define SVGWIDGET_H

#include <QWidget>
#include <QSvgRenderer>

class SvgWidget : public QWidget {
    Q_OBJECT
    QSvgRenderer *renderer;

public:
    explicit SvgWidget(const QString &filePath, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
};

#endif // SVGWIDGET_H
