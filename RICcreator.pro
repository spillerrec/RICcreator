######################################################################
# Automatically generated by qmake (2.01a) ti 7. dec 22:48:15 2010
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . src src\GUI src\riclib
INCLUDEPATH += .
CONFIG += console

# Input
HEADERS += src/GUI/mainwindow.h \
           src/GUI/ricObjectModel.h \
           src/riclib/nxtCanvas.h \
           src/riclib/nxtConstants.h \
           src/riclib/nxtVariable.h \
           src/riclib/ricfile.h \
           src/riclib/ricObject.h
FORMS += src/GUI/mainwindow.ui
SOURCES += src/main.cpp \
           src/GUI/mainwindow.cpp \
           src/GUI/ricObjectModel.cpp \
           src/riclib/nxtCanvas.cpp \
           src/riclib/nxtVariable.cpp \
           src/riclib/ricfile.cpp \
           src/riclib/ricObjectDraw.cpp \
           src/riclib/ricObjectRead.cpp \
           src/riclib/ricObjectWrite.cpp