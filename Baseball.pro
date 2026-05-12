QT       += core gui \
    quick
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin.cpp \
    adminaddsouvenir.cpp \
    adminchangeprice.cpp \
    admindeletesouvenir.cpp \
    basictripwindow.cpp \
    custominordertrip.cpp \
    customtripwindow.cpp \
    dijkstraalgo.cpp \
    dijkstratripwindow.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    modinfo.cpp \
    modsouvenirs.cpp \
    modstadiums.cpp \
    summarywindow.cpp \
    specialalgorithms.cpp \
    tripwindow.cpp

HEADERS += \
    admin.h \
    adminaddsouvenir.h \
    adminchangeprice.h \
    admindeletesouvenir.h \
    basictripwindow.h \
    custominordertrip.h \
    customtripwindow.h \
    dijkstraalgo.h \
    dijkstratripwindow.h \
    login.h \
    mainwindow.h \
    modinfo.h \
    modsouvenirs.h \
    modstadiums.h \
    specialalgorithms.h \
    summarywindow.h \
    tripwindow.h

FORMS += \
    admin.ui \
    adminaddsouvenir.ui \
    adminchangeprice.ui \
    admindeletesouvenir.ui \
    basictripwindow.ui \
    custominordertrip.ui \
    customtripwindow.ui \
    dijkstratripwindow.ui \
    login.ui \
    mainwindow.ui \
    modinfo.ui \
    modsouvenirs.ui \
    modstadiums.ui \
    specialalgorithms.ui \
    summarywindow.ui \
    tripwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    BaseballDatabase.sqlite \
    style.qss
