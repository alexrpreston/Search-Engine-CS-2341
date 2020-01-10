TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        documentparser.cpp \
        main.cpp \
        queryprocessor.cpp \
        userinterface.cpp

HEADERS += \
    AVLTree.h \
    HashMap.h \
    IndexInterface.h \
    documentparser.h \
    queryprocessor.h \
    userinterface.h


