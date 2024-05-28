//
// Created by williamwestwood on 28/05/24.
//

#ifndef GLIMAGEPROCESSOR_H
#define GLIMAGEPROCESSOR_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <opencv2/opencv.hpp>

class GLImageProcessor : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLImageProcessor(QWidget *parent = nullptr);
    ~GLImageProcessor();

    void updateFrame(const cv::Mat &frame);  // Public method to update frame from outside

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void teardownGL();

private:
    void setupShaders();
    void setupTexture(int width, int height);

    QOpenGLShaderProgram *shaderProgram;
    QOpenGLTexture *texture;
    QOpenGLBuffer *pbo;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    cv::Mat currentFrame;
};

#endif // GLIMAGEPROCESSOR_H

