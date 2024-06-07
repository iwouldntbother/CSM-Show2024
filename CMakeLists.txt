cmake_minimum_required(VERSION 3.28)
project(form_scan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# DONT MOVE THE BELLOW LINE !!
set(CMAKE_PREFIX_PATH "/home/williamwestwood/Qt/6.7.1/gcc_64/lib/cmake")
# set(Qt6_DIR "/home/admin/Qt/6.7.1/gcc_64/lib/cmake/Qt6")



find_package(OpenCV REQUIRED)
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        Xml
        SvgWidgets
        OpenGL
        OpenGLWidgets
        REQUIRED)
find_package(Tesseract REQUIRED)

add_executable(form_scan main.cpp
        detect_markers.cpp
        detect_markers.h
        align_and_crop.h
        align_and_crop.cpp
        ocr.cpp
        ocr.h
        get_results.cpp
        get_results.h
        detect_markers.h
        displaywidget.cpp
        displaywidget.h
        displaywidget.ui
        DataContainer.h
        SharedData.h
        GlobalData.h
        GlobalData.cpp
        SvgWidget.h
        SvgWidget.cpp
        GLImageProcessor.cpp
        GLImageProcessor.h
        detect_faces.cpp
        detect_faces.h
        analyse_results.cpp
        analyse_results.h
        libescpos/
        printer.cpp
        printer.h
)

# Include OpenCV headers
target_include_directories(form_scan PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${Tesseract_INCLUDE_DIRS}
        ${Qt6_INCLUDE_DIRS}
        assets
)

target_sources(form_scan PRIVATE displaywidget.ui)

# Link OpenCV libraries
target_link_libraries(form_scan ${OpenCV_LIBS} ${Tesseract_LIBRARIES}
        Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Svg Qt6::Xml Qt6::SvgWidgets Qt6::OpenGL Qt6::OpenGLWidgets)

qt_add_resources(form_scan "assets"
        PREFIX "/"
        FILES
        assets/form-min.svg
        assets/images/form-tiny.svg
        assets/images/logo.png
        assets/fonts/ChakraPetch-Regular.ttf
        )

configure_file(
        "${CMAKE_SOURCE_DIR}/assets/haarcascades/haarcascade_frontalface_alt.xml"
        "${CMAKE_BINARY_DIR}/assets/haarcascades/haarcascade_frontalface_alt.xml"
        COPYONLY
)

configure_file(
        "${CMAKE_SOURCE_DIR}/llm_python/summarise.py"
        "${CMAKE_BINARY_DIR}/llm_python/summarise.py"
        COPYONLY
)

configure_file(
        "${CMAKE_SOURCE_DIR}/llm_python/llm-model.gguf"
        "${CMAKE_BINARY_DIR}/llm_python/llm-model.gguf"
        COPYONLY
)

configure_file(
        "${CMAKE_SOURCE_DIR}/print_python/print_receipt.py"
        "${CMAKE_BINARY_DIR}/print_python/print_receipt.py"
        COPYONLY
)

configure_file(
        "${CMAKE_SOURCE_DIR}/assets/images/print_logo.png"
        "${CMAKE_BINARY_DIR}/assets/images/print_logo.png"
        COPYONLY
)