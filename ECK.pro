TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        arraymanipulator.c \
        main.c\
        filters.c\
        sensor.c\
        qsr.c

HEADERS += \
        arraymanipulator.h \
        filters.h\
        sensor.h\
        qsr.h

RESOURCES += \
    ECG.txt
