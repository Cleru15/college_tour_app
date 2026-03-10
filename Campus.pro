# Campus.pro
# Qt project configuration file.
#
# Defines the modules, source files, header files, UI forms,
# and deployment settings required to build the Campus Tour
# application using qmake.

# ================================================================
# QT MODULES
# ---------------------------------------------------------------
# These modules enable different Qt libraries used in the project.
# core    -> base Qt functionality
# gui     -> graphical user interface utilities
# quick   -> Qt Quick / QML components
# sql     -> database functionality (SQLite queries)
# ================================================================

QT += core gui quick sql

# If Qt version is greater than Qt4, include the widgets module.
# Widgets provides classes such as QMainWindow, QPushButton, etc.
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


# ================================================================
# PROJECT CONFIGURATION
# ---------------------------------------------------------------
# Specifies the C++ standard used by the compiler.
# c++17 is required for modern language features used in the project.
# ================================================================

CONFIG += c++17


# ================================================================
# DEPRECATED API CONTROL (OPTIONAL)
# ---------------------------------------------------------------
# Uncommenting the line below will cause compilation to fail if the
# code uses APIs deprecated before Qt 6. This helps enforce modern Qt
# development practices.
# ================================================================

# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000


# ================================================================
# SOURCE FILES
# ---------------------------------------------------------------
# These are the main implementation (.cpp) files compiled into the
# application executable.
# Each file implements the logic for a specific window or feature.
# ================================================================

SOURCES += \
    admin.cpp \
    adminaddsouvenir.cpp \
    adminchangeprice.cpp \
    admindeletesouvenir.cpp \
    basictripwindow.cpp \
    customtripwindow.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    modcolleges.cpp \
    modsouvenirs.cpp \
    summarywindow.cpp \
    tripwindow.cpp


# ================================================================
# HEADER FILES
# ---------------------------------------------------------------
# These define class declarations, member variables, and function
# prototypes for the application components.
# ================================================================

HEADERS += \
    admin.h \
    adminaddsouvenir.h \
    adminchangeprice.h \
    admindeletesouvenir.h \
    basictripwindow.h \
    customtripwindow.h \
    login.h \
    mainwindow.h \
    modcolleges.h \
    modsouvenirs.h \
    summarywindow.h \
    tripwindow.h


# ================================================================
# UI FORMS
# ---------------------------------------------------------------
# These Qt Designer (.ui) files define the visual layout for each
# window in the application.
# During compilation, Qt converts them into C++ code automatically.
# ================================================================

FORMS += \
    admin.ui \
    adminaddsouvenir.ui \
    adminchangeprice.ui \
    admindeletesouvenir.ui \
    basictripwindow.ui \
    customtripwindow.ui \
    login.ui \
    mainwindow.ui \
    modcolleges.ui \
    modsouvenirs.ui \
    summarywindow.ui \
    tripwindow.ui


# ================================================================
# DEPLOYMENT RULES
# ---------------------------------------------------------------
# Specifies where the application should be installed on different
# platforms when deployment occurs.
# ================================================================

# QNX deployment location
qnx: target.path = /tmp/$${TARGET}/bin

# Unix/Linux deployment location (excluding Android)
else: unix:!android: target.path = /opt/$${TARGET}/bin

# If a deployment path exists, include it in install rules
!isEmpty(target.path): INSTALLS += target


# ================================================================
# DISTRIBUTION FILES
# ---------------------------------------------------------------
# Additional files that should be included with the project when it
# is packaged or distributed. In this case, the SQLite database used
# by the application.
# ================================================================

DISTFILES += \
    college_tour.sqlite
