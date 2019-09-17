TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c\
        filters.c\
        sensor.c\
        qsr.c

HEADERS += \
        filters.h\
        sensor.h\
        qsr.h

RESOURCES += \
    ECG.txt
