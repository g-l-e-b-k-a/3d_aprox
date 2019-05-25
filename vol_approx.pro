#-------------------------------------------------
#
# Project created by QtCreator 2015-05-25T10:43:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vol_approx
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    startprog.cpp \
    algorithm.cpp \
    fill_matrix.cpp \
    visualisation.cpp \
    help_funcs.cpp \
    transformations.cpp

HEADERS  += window.h \
    algorithm.h


QT += opengl
