/********************************************************************************
** Form generated from reading UI file 'basictripwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BASICTRIPWINDOW_H
#define UI_BASICTRIPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BasicTripWindow
{
public:
    QWidget *centralwidget;
    QPushButton *startTripButtonBT;
    QComboBox *numCollegestoVisitDropdownBT;
    QComboBox *selectStartingCollegeDropdownBT;
    QLabel *selectStartingCollegeLabelBT;
    QLabel *numCollegestoVisitLabelBT;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *BasicTripWindow)
    {
        if (BasicTripWindow->objectName().isEmpty())
            BasicTripWindow->setObjectName("BasicTripWindow");
        BasicTripWindow->resize(954, 625);
        centralwidget = new QWidget(BasicTripWindow);
        centralwidget->setObjectName("centralwidget");
        startTripButtonBT = new QPushButton(centralwidget);
        startTripButtonBT->setObjectName("startTripButtonBT");
        startTripButtonBT->setGeometry(QRect(80, 190, 80, 25));
        numCollegestoVisitDropdownBT = new QComboBox(centralwidget);
        numCollegestoVisitDropdownBT->setObjectName("numCollegestoVisitDropdownBT");
        numCollegestoVisitDropdownBT->setGeometry(QRect(45, 140, 150, 25));
        selectStartingCollegeDropdownBT = new QComboBox(centralwidget);
        selectStartingCollegeDropdownBT->setObjectName("selectStartingCollegeDropdownBT");
        selectStartingCollegeDropdownBT->setGeometry(QRect(45, 70, 161, 25));
        selectStartingCollegeLabelBT = new QLabel(centralwidget);
        selectStartingCollegeLabelBT->setObjectName("selectStartingCollegeLabelBT");
        selectStartingCollegeLabelBT->setGeometry(QRect(45, 50, 171, 16));
        selectStartingCollegeLabelBT->setAlignment(Qt::AlignmentFlag::AlignCenter);
        numCollegestoVisitLabelBT = new QLabel(centralwidget);
        numCollegestoVisitLabelBT->setObjectName("numCollegestoVisitLabelBT");
        numCollegestoVisitLabelBT->setGeometry(QRect(45, 120, 191, 20));
        numCollegestoVisitLabelBT->setAlignment(Qt::AlignmentFlag::AlignCenter);
        BasicTripWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(BasicTripWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 954, 25));
        BasicTripWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(BasicTripWindow);
        statusbar->setObjectName("statusbar");
        BasicTripWindow->setStatusBar(statusbar);

        retranslateUi(BasicTripWindow);

        QMetaObject::connectSlotsByName(BasicTripWindow);
    } // setupUi

    void retranslateUi(QMainWindow *BasicTripWindow)
    {
        BasicTripWindow->setWindowTitle(QCoreApplication::translate("BasicTripWindow", "MainWindow", nullptr));
        startTripButtonBT->setText(QCoreApplication::translate("BasicTripWindow", "Start Trip", nullptr));
        selectStartingCollegeLabelBT->setText(QCoreApplication::translate("BasicTripWindow", "Select Starting College:", nullptr));
        numCollegestoVisitLabelBT->setText(QCoreApplication::translate("BasicTripWindow", "Number of Colleges to Visit:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BasicTripWindow: public Ui_BasicTripWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BASICTRIPWINDOW_H
