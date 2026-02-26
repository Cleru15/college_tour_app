/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QComboBox *dropdownSouvenirPreview;
    QPushButton *submitButtonSouvenirPreview;
    QPushButton *cancelButtonSouvenirPreview;
    QListView *distanceListView;
    QPushButton *submitButtonSouvenirPreview_2;
    QComboBox *dropdownSouvenirPreview_2;
    QLabel *label;
    QPushButton *submitButtonSouvenirPreview_3;
    QPushButton *submitButtonSouvenirPreview_4;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1115, 498);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        dropdownSouvenirPreview = new QComboBox(centralwidget);
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->addItem(QString());
        dropdownSouvenirPreview->setObjectName("dropdownSouvenirPreview");
        dropdownSouvenirPreview->setGeometry(QRect(10, 30, 190, 41));
        dropdownSouvenirPreview->setEditable(false);
        submitButtonSouvenirPreview = new QPushButton(centralwidget);
        submitButtonSouvenirPreview->setObjectName("submitButtonSouvenirPreview");
        submitButtonSouvenirPreview->setGeometry(QRect(10, 80, 90, 32));
        cancelButtonSouvenirPreview = new QPushButton(centralwidget);
        cancelButtonSouvenirPreview->setObjectName("cancelButtonSouvenirPreview");
        cancelButtonSouvenirPreview->setGeometry(QRect(110, 80, 90, 32));
        distanceListView = new QListView(centralwidget);
        distanceListView->setObjectName("distanceListView");
        distanceListView->setGeometry(QRect(260, 30, 541, 410));
        submitButtonSouvenirPreview_2 = new QPushButton(centralwidget);
        submitButtonSouvenirPreview_2->setObjectName("submitButtonSouvenirPreview_2");
        submitButtonSouvenirPreview_2->setGeometry(QRect(10, 190, 191, 32));
        dropdownSouvenirPreview_2 = new QComboBox(centralwidget);
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->addItem(QString());
        dropdownSouvenirPreview_2->setObjectName("dropdownSouvenirPreview_2");
        dropdownSouvenirPreview_2->setGeometry(QRect(10, 140, 190, 41));
        dropdownSouvenirPreview_2->setEditable(false);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(840, 30, 91, 20));
        submitButtonSouvenirPreview_3 = new QPushButton(centralwidget);
        submitButtonSouvenirPreview_3->setObjectName("submitButtonSouvenirPreview_3");
        submitButtonSouvenirPreview_3->setGeometry(QRect(840, 60, 191, 32));
        submitButtonSouvenirPreview_4 = new QPushButton(centralwidget);
        submitButtonSouvenirPreview_4->setObjectName("submitButtonSouvenirPreview_4");
        submitButtonSouvenirPreview_4->setGeometry(QRect(840, 110, 191, 32));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1115, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(submitButtonSouvenirPreview, SIGNAL(clicked()), MainWindow, SLOT(previewSouvenirButtonClick()));
        QObject::connect(cancelButtonSouvenirPreview, SIGNAL(clicked()), MainWindow, SLOT(cancelButtonClick()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        dropdownSouvenirPreview->setItemText(0, QCoreApplication::translate("MainWindow", "Display Souvenirs...", nullptr));
        dropdownSouvenirPreview->setItemText(1, QCoreApplication::translate("MainWindow", "Arizona State University", nullptr));
        dropdownSouvenirPreview->setItemText(2, QCoreApplication::translate("MainWindow", "Northwestern", nullptr));
        dropdownSouvenirPreview->setItemText(3, QCoreApplication::translate("MainWindow", "Ohio State University", nullptr));
        dropdownSouvenirPreview->setItemText(4, QCoreApplication::translate("MainWindow", "Saddleback College", nullptr));
        dropdownSouvenirPreview->setItemText(5, QCoreApplication::translate("MainWindow", "University of Michigan", nullptr));
        dropdownSouvenirPreview->setItemText(6, QCoreApplication::translate("MainWindow", "University of California, Irvine (UCI)", nullptr));
        dropdownSouvenirPreview->setItemText(7, QCoreApplication::translate("MainWindow", "University of California, Los Angeles (UCLA)", nullptr));
        dropdownSouvenirPreview->setItemText(8, QCoreApplication::translate("MainWindow", "University of Oregon", nullptr));
        dropdownSouvenirPreview->setItemText(9, QCoreApplication::translate("MainWindow", "University of the Pacific", nullptr));
        dropdownSouvenirPreview->setItemText(10, QCoreApplication::translate("MainWindow", "University of Wisconsin", nullptr));

        submitButtonSouvenirPreview->setText(QCoreApplication::translate("MainWindow", "Submit", nullptr));
        cancelButtonSouvenirPreview->setText(QCoreApplication::translate("MainWindow", "Cancel", nullptr));
        submitButtonSouvenirPreview_2->setText(QCoreApplication::translate("MainWindow", "Submit", nullptr));
        dropdownSouvenirPreview_2->setItemText(0, QCoreApplication::translate("MainWindow", "Distance Dropdown...", nullptr));
        dropdownSouvenirPreview_2->setItemText(1, QCoreApplication::translate("MainWindow", "Arizona State University", nullptr));
        dropdownSouvenirPreview_2->setItemText(2, QCoreApplication::translate("MainWindow", "Northwestern", nullptr));
        dropdownSouvenirPreview_2->setItemText(3, QCoreApplication::translate("MainWindow", "Ohio State University", nullptr));
        dropdownSouvenirPreview_2->setItemText(4, QCoreApplication::translate("MainWindow", "Saddleback College", nullptr));
        dropdownSouvenirPreview_2->setItemText(5, QCoreApplication::translate("MainWindow", "University of Michigan", nullptr));
        dropdownSouvenirPreview_2->setItemText(6, QCoreApplication::translate("MainWindow", "University of California, Irvine (UCI)", nullptr));
        dropdownSouvenirPreview_2->setItemText(7, QCoreApplication::translate("MainWindow", "University of California, Los Angeles (UCLA)", nullptr));
        dropdownSouvenirPreview_2->setItemText(8, QCoreApplication::translate("MainWindow", "University of Oregon", nullptr));
        dropdownSouvenirPreview_2->setItemText(9, QCoreApplication::translate("MainWindow", "University of the Pacific", nullptr));
        dropdownSouvenirPreview_2->setItemText(10, QCoreApplication::translate("MainWindow", "University of Wisconsin", nullptr));

        label->setText(QCoreApplication::translate("MainWindow", "Plan a Trip:", nullptr));
        submitButtonSouvenirPreview_3->setText(QCoreApplication::translate("MainWindow", "Basic Trip", nullptr));
        submitButtonSouvenirPreview_4->setText(QCoreApplication::translate("MainWindow", "Custom Trip", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
