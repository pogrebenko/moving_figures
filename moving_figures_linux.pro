QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app.cpp \
    canvas.cpp \
    common/Figure.cpp \
    common/IOStreamer.cpp \
    common/Options.cpp \
    common/Relation.cpp \
    common/Utils.cpp \
    data/Ellipse.cpp \
    data/FigureFactory.cpp \
    data/Rectangle.cpp \
    data/Triangle.cpp \
    engine/Builder.cpp \
    main.cpp \
    mainwindow.cpp \
    view/Painter.cpp

HEADERS += \
    app.h \
    canvas.h \
    common/Consts.h \
    common/Figure.h \
    common/IOStreamer.h \
    common/Logger.h \
    common/Options.h \
    common/Relation.h \
    common/Utils.h \
    common/VectorOfPtr.h \
    data/Ellipse.h \
    data/FigureFactory.h \
    data/Rectangle.h \
    data/Triangle.h \
    engine/Builder.h \
    mainwindow.h \
    view/Painter.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
