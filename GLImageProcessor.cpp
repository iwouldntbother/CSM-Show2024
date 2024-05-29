//
// Created by williamwestwood on 28/05/24.
//

#include "GLImageProcessor.h"
#include <QApplication>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>

GLImageProcessor::GLImageProcessor(QWidget *parent)
    : QOpenGLWidget(parent), shaderProgram(nullptr), texture(nullptr), pbo(new QOpenGLBuffer(QOpenGLBuffer::PixelUnpackBuffer)) {
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
}

GLImageProcessor::~GLImageProcessor() {
    makeCurrent();
    teardownGL();
    doneCurrent();
}


void GLImageProcessor::initializeGL() {
    initializeOpenGLFunctions();
    QOpenGLWidget::initializeGL(); // Ensure parent initialization

    setupShaders();
    setupTexture(504, 504);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup VAO and VBO
    vao.create();
    vao.bind();

    static constexpr GLfloat vertices[] = {
        // Positions    // TexCoords
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f,
    };

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    shaderProgram->bind();
    const int posLocation = shaderProgram->attributeLocation("aPos");
    const int texLocation = shaderProgram->attributeLocation("aTexCoords");

    shaderProgram->enableAttributeArray(posLocation);
    shaderProgram->setAttributeBuffer(posLocation, GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));

    shaderProgram->enableAttributeArray(texLocation);
    shaderProgram->setAttributeBuffer(texLocation, GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));

    vbo.release();
    vao.release();
    shaderProgram->release();
}

void GLImageProcessor::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}


void GLImageProcessor::paintGL() {
    if (!currentFrame.empty()) {
        updateFrame(currentFrame);
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error before draw:" << err;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram->bind();
    if (!texture) {
        return;
    }
    texture->bind();
    shaderProgram->setUniformValue("resolution", QVector2D(640, 480));
    shaderProgram->setUniformValue("u_tex0", 0);

    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error during draw:" << err;
    }

    vao.bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    vao.release();

    texture->release();
    shaderProgram->release();
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error after release:" << err;
    }
}


void GLImageProcessor::updateFrame(const cv::Mat &frame) {
    if (!texture || !texture->isCreated()) {
        qDebug() << "Texture not created";
        return;
    }

    currentFrame = frame.clone();
    cv::cvtColor(currentFrame, currentFrame, cv::COLOR_BGR2RGB);
    texture->bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, currentFrame.cols, currentFrame.rows, GL_RGB, GL_UNSIGNED_BYTE, currentFrame.data);
    texture->release();
    update();
}




void GLImageProcessor::setupShaders() {
    shaderProgram = new QOpenGLShaderProgram;

    // Vertex shader source code
    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoords;
    out vec2 TexCoords;
    void main() {
        TexCoords = aTexCoords;
        gl_Position = vec4(aPos.xy, 0.0, 1.0);
    }
    )";


    // Fragment shader source code
    const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoords;
    uniform sampler2D u_tex0;
    uniform vec2 resolution;
    const float bayerMatrix[64] = float[64](
        0.0/64.0, 32.0/64.0, 8.0/64.0, 40.0/64.0, 2.0/64.0, 34.0/64.0, 10.0/64.0, 42.0/64.0,
        48.0/64.0, 16.0/64.0, 56.0/64.0, 24.0/64.0, 50.0/64.0, 18.0/64.0, 58.0/64.0, 26.0/64.0,
        12.0/64.0, 44.0/64.0, 4.0/64.0, 36.0/64.0, 14.0/64.0, 46.0/64.0, 6.0/64.0, 38.0/64.0,
        60.0/64.0, 28.0/64.0, 52.0/64.0, 20.0/64.0, 62.0/64.0, 30.0/64.0, 54.0/64.0, 22.0/64.0,
        3.0/64.0, 35.0/64.0, 11.0/64.0, 43.0/64.0, 1.0/64.0, 33.0/64.0, 9.0/64.0, 41.0/64.0,
        51.0/64.0, 19.0/64.0, 59.0/64.0, 27.0/64.0, 49.0/64.0, 17.0/64.0, 57.0/64.0, 25.0/64.0,
        15.0/64.0, 47.0/64.0, 7.0/64.0, 39.0/64.0, 13.0/64.0, 45.0/64.0, 5.0/64.0, 37.0/64.0,
        63.0/64.0, 31.0/64.0, 55.0/64.0, 23.0/64.0, 61.0/64.0, 29.0/64.0, 53.0/64.0, 21.0/64.0
    );
    void main() {
        vec2 pos = gl_FragCoord.xy / resolution.xy;
        vec4 color = texture(u_tex0, TexCoords);
        float gray = (color.r + color.g + color.b) / 3.0; // Convert to grayscale
        // Get the corresponding value from the Bayer matrix
        int x = int(mod(gl_FragCoord.x, 8.0));
        int y = int(mod(gl_FragCoord.y, 8.0));
        int index = y * 8 + x;
        float threshold = bayerMatrix[index];
        // Apply the dithering effect
        float dithered = gray < threshold ? 0.0 : 1.0;
        FragColor = vec4(vec3(dithered), 1.0);
    }
    )";


    // Compile and link the shaders
    if (!shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)) {
        qDebug() << "Vertex shader compile error:" << shaderProgram->log();
    } else {
        std::cout << "Vertex shader compiled successfully" << std::endl;
    }

    if (!shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)) {
        qDebug() << "Fragment shader compile error:" << shaderProgram->log();
    } else {
        std::cout << "Fragment shader compiled successfully" << std::endl;
    }

    if (!shaderProgram->link()) {
        qDebug() << "Shader link error:" << shaderProgram->log();
    } else {
        std::cout << "Shader program linked successfully" << std::endl;
    }
}

void GLImageProcessor::setupTexture(int width, int height) {
    delete texture;
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    if (!texture->create()) {
        qDebug() << "Texture creation failed";
        return;
    }
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture->setSize(width, height);
    texture->setFormat(QOpenGLTexture::RGB8_UNorm);
    texture->allocateStorage();
    std::cout << "Texture created successfully" << std::endl;
}



void GLImageProcessor::teardownGL() {
    delete shaderProgram;
    delete texture;
    delete pbo;
    vbo.destroy();
    vao.destroy();
}
