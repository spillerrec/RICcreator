######################################################################
# Automatically generated by qmake (2.01a) ti 19. jun 00:01:09 2012
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . \
              debug \
              release \
              src \
              src\GUI \
              src\resources \
              src\riclib \
              src\GUI\nxtVarEdits \
              src\GUI\pugixml
INCLUDEPATH += .

# Input
HEADERS += src/GUI/about.h \
           src/GUI/importImageDialog.h \
           src/GUI/mainwindow.h \
           src/GUI/nxtCanvasWidget.h \
           src/GUI/nxtCanvasWidgetContainer.h \
           src/GUI/openRicfile.h \
           src/GUI/programSettings.h \
           src/GUI/ric_value.h \
           src/GUI/ricfileEditor.h \
           src/GUI/ricfileEditorAdvanced.h \
           src/GUI/ricfileEditorSimple.h \
           src/GUI/ricObjectAbstract.h \
           src/GUI/ricObjectAbstractChildren.h \
           src/GUI/ricObjectContainer.h \
           src/GUI/ricObjectModel.h \
           src/GUI/ricObjectTexter.h \
           src/GUI/ricParametersModel.h \
           src/riclib/nxt_default_font.h \
           src/riclib/nxtCanvas.h \
           src/riclib/nxtCopyOptions.h \
           src/riclib/nxtCopyOptionsBase.h \
           src/riclib/nxtFile.h \
           src/riclib/nxtIO.h \
           src/riclib/nxtStream.h \
           src/riclib/nxtVariable.h \
           src/riclib/nxtVarRicCopyOptions.h \
           src/riclib/nxtVarRicWord.h \
           src/riclib/pointArray.h \
           src/riclib/ricfile.h \
           src/riclib/ricObject.h \
           src/riclib/ricObjectChildren.h \
           src/GUI/nxtVarEdits/copyoptions_value.h \
           src/GUI/nxtVarEdits/nxtRicIdValue.h \
           src/GUI/nxtVarEdits/nxtRicWordExtraValue.h \
           src/GUI/nxtVarEdits/nxtVarEditAbstract.h \
           src/GUI/nxtVarEdits/nxtVarWordValue.h \
           src/GUI/nxtVarEdits/optionsValue.h \
           src/GUI/nxtVarEdits/pointArrayValue.h \
           src/GUI/nxtVarEdits/pointArrayValueItem.h \
           src/GUI/pugixml/pugiconfig.hpp \
           src/GUI/pugixml/pugixml.hpp
FORMS += src/GUI/about.ui \
         src/GUI/importImageDialog.ui \
         src/GUI/mainwindow.ui \
         src/GUI/ric_value.ui \
         src/GUI/ricfile.ui \
         src/GUI/nxtVarEdits/copyoptions_value.ui
SOURCES += src/main.cpp \
           src/GUI/about.cpp \
           src/GUI/importImageDialog.cpp \
           src/GUI/mainwindow.cpp \
           src/GUI/nxtCanvasWidget.cpp \
           src/GUI/nxtCanvasWidgetContainer.cpp \
           src/GUI/openRicfile.cpp \
           src/GUI/programSettings.cpp \
           src/GUI/ric_value.cpp \
           src/GUI/ricfileEditor.cpp \
           src/GUI/ricfileEditorAdvanced.cpp \
           src/GUI/ricfileEditorSimple.cpp \
           src/GUI/ricObjectAbstract.cpp \
           src/GUI/ricObjectContainer.cpp \
           src/GUI/ricObjectModel.cpp \
           src/GUI/ricObjectTexter.cpp \
           src/GUI/ricParametersModel.cpp \
           src/riclib/nxtCanvas.cpp \
           src/riclib/nxtFile.cpp \
           src/riclib/nxtIO.cpp \
           src/riclib/nxtStream.cpp \
           src/riclib/nxtVariable.cpp \
           src/riclib/nxtVarRicWord.cpp \
           src/riclib/pointArray.cpp \
           src/riclib/ricfile.cpp \
           src/riclib/ricObject.cpp \
           src/riclib/ricObjectDraw.cpp \
           src/riclib/ricObjectOther.cpp \
           src/riclib/ricObjectRicscript.cpp \
           src/GUI/nxtVarEdits/copyoptions_value.cpp \
           src/GUI/nxtVarEdits/nxtRicIdValue.cpp \
           src/GUI/nxtVarEdits/nxtRicWordExtraValue.cpp \
           src/GUI/nxtVarEdits/nxtVarEditAbstract.cpp \
           src/GUI/nxtVarEdits/nxtVarWordValue.cpp \
           src/GUI/nxtVarEdits/optionsValue.cpp \
           src/GUI/nxtVarEdits/pointArrayValue.cpp \
           src/GUI/nxtVarEdits/pointArrayValueItem.cpp \
           src/GUI/pugixml/pugixml.cpp
RESOURCES += src/resources/resources.qrc

RC_FILE = src/resources/RICcreator.rc
