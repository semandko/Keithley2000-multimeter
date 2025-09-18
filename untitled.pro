QT += core gui serialport widgets charts

greaterThan(QT_MAJOR_VERSION, 5): QT += charts

CONFIG += c++17

SOURCES += \
    keithleyapp.cpp \
    main.cpp

HEADERS += \
    keithleyapp.h

FORMS += \
    mainwindow.ui
