# Campus.pro is the Qt project build file.
# It lists which Qt modules we use (widgets for UI, sql for database, quick if needed),
# and which source/header/ui files get compiled into the application.

QT += core gui widgets sql

CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basictripwindow.cpp \
    customtripwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    newwindow.cpp \
    summarywindow.cpp \
    tripwindow.cpp

HEADERS += \
    basictripwindow.h \
    customtripwindow.h \
    mainwindow.h \
    newwindow.h \
    summarywindow.h \
    tripwindow.h

FORMS += \
    basictripwindow.ui \
    customtripwindow.ui \
    mainwindow.ui \
    newwindow.ui \
    summarywindow.ui \
    tripwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    college_tour.sqlite
