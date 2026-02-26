/********************************************************************************
** Form generated from reading UI file 'customtripwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMTRIPWINDOW_H
#define UI_CUSTOMTRIPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomTripWindow
{
public:
    QWidget *centralwidget;
    QComboBox *selectStartingCollegeDropdownCT;
    QPushButton *confirmButtonCT;
    QLabel *label_2;
    QPushButton *startTripButtonCT;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CustomTripWindow)
    {
        if (CustomTripWindow->objectName().isEmpty())
            CustomTripWindow->setObjectName("CustomTripWindow");
        CustomTripWindow->resize(976, 689);
        centralwidget = new QWidget(CustomTripWindow);
        centralwidget->setObjectName("centralwidget");
        selectStartingCollegeDropdownCT = new QComboBox(centralwidget);
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->addItem(QString());
        selectStartingCollegeDropdownCT->setObjectName("selectStartingCollegeDropdownCT");
        selectStartingCollegeDropdownCT->setGeometry(QRect(20, 40, 191, 25));
        confirmButtonCT = new QPushButton(centralwidget);
        confirmButtonCT->setObjectName("confirmButtonCT");
        confirmButtonCT->setGeometry(QRect(220, 40, 81, 25));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(60, 90, 171, 20));
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        startTripButtonCT = new QPushButton(centralwidget);
        startTripButtonCT->setObjectName("startTripButtonCT");
        startTripButtonCT->setGeometry(QRect(120, 380, 80, 25));
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(10, 130, 311, 241));
        CustomTripWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CustomTripWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 976, 25));
        CustomTripWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CustomTripWindow);
        statusbar->setObjectName("statusbar");
        CustomTripWindow->setStatusBar(statusbar);

        retranslateUi(CustomTripWindow);
        QObject::connect(startTripButtonCT, SIGNAL(clicked()), CustomTripWindow, SLOT(startTripClickedCT()));

        QMetaObject::connectSlotsByName(CustomTripWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CustomTripWindow)
    {
        CustomTripWindow->setWindowTitle(QCoreApplication::translate("CustomTripWindow", "MainWindow", nullptr));
        selectStartingCollegeDropdownCT->setItemText(0, QCoreApplication::translate("CustomTripWindow", "Select Starting College...", nullptr));
        selectStartingCollegeDropdownCT->setItemText(1, QCoreApplication::translate("CustomTripWindow", "Arizona State University", nullptr));
        selectStartingCollegeDropdownCT->setItemText(2, QCoreApplication::translate("CustomTripWindow", "Northwestern", nullptr));
        selectStartingCollegeDropdownCT->setItemText(3, QCoreApplication::translate("CustomTripWindow", "Ohio State University", nullptr));
        selectStartingCollegeDropdownCT->setItemText(4, QCoreApplication::translate("CustomTripWindow", "Saddleback College", nullptr));
        selectStartingCollegeDropdownCT->setItemText(5, QCoreApplication::translate("CustomTripWindow", "University of Michigan", nullptr));
        selectStartingCollegeDropdownCT->setItemText(6, QCoreApplication::translate("CustomTripWindow", "University of California, Irvine (UCI)", nullptr));
        selectStartingCollegeDropdownCT->setItemText(7, QCoreApplication::translate("CustomTripWindow", "University of California, Los Angeles (UCLA)", nullptr));
        selectStartingCollegeDropdownCT->setItemText(8, QCoreApplication::translate("CustomTripWindow", "University of Oregon", nullptr));
        selectStartingCollegeDropdownCT->setItemText(9, QCoreApplication::translate("CustomTripWindow", "University of the Pacific", nullptr));
        selectStartingCollegeDropdownCT->setItemText(10, QCoreApplication::translate("CustomTripWindow", "University of Wisconsin", nullptr));

        confirmButtonCT->setText(QCoreApplication::translate("CustomTripWindow", "Confirm", nullptr));
        label_2->setText(QCoreApplication::translate("CustomTripWindow", "Select Colleges to Visit", nullptr));
        startTripButtonCT->setText(QCoreApplication::translate("CustomTripWindow", "Start Trip", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CustomTripWindow: public Ui_CustomTripWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMTRIPWINDOW_H
