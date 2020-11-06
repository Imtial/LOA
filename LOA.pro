QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

#QMAKE_CXXFLAGS += -O3 -mpreferred-stack-boundary=3 -finline-small-functions -momit-leaf-frame-pointer
QMAKE_CXXFLAGS += -O3

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ai.cpp \
    bcircle.cpp \
    bmark.cpp \
    board.cpp \
    boardstate.cpp \
    bsquare.cpp \
    cell.cpp \
    controller.cpp \
    grid.cpp \
    human.cpp \
    main.cpp \
    mainwindow.cpp \
    movearrow.cpp \
    player.cpp \
    simulator.cpp

HEADERS += \
    ai.h \
    bcircle.h \
    bmark.h \
    board.h \
    boardstate.h \
    bsquare.h \
    cell.h \
    constants.h \
    controller.h \
    grid.h \
    human.h \
    mainwindow.h \
    movearrow.h \
    player.h \
    simulator.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
