TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpcap

SOURCES += \
        main.cpp

HEADERS += \
    sniff_ethernet.h \
    sniff_ip.h \
    sniff_tcp.h
