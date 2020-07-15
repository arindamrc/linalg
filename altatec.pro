TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \

HEADERS += \
    matrix.hpp \
    vector.hpp

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

QMAKE_CFLAGS -= -O2
QMAKE_CFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS -= -O1
QMAKE_CFLAGS += -O3
QMAKE_LFLAGS += -O3
QMAKE_CXXFLAGS += -O3

LIBS += -lgomp
