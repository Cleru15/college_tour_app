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
    QPushButton *buttonSouvenirPreview;
    QPushButton *buttonSouvenirCancel;
    QListView *distanceListView;
    QPushButton *buttonDistancesSubmit;
    QComboBox *dropdownDistances;
    QLabel *labelTrip;
    QPushButton *buttonBasicTrip;
    QPushButton *buttonCustomTrip;
    QPushButton *pushButton;
    QLabel *labelSouvenirs;
    QLabel *labelDistances;
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
        dropdownSouvenirPreview->setObjectName("dropdownSouvenirPreview");
        dropdownSouvenirPreview->setGeometry(QRect(10, 30, 190, 41));
        dropdownSouvenirPreview->setEditable(false);
        buttonSouvenirPreview = new QPushButton(centralwidget);
        buttonSouvenirPreview->setObjectName("buttonSouvenirPreview");
        buttonSouvenirPreview->setGeometry(QRect(10, 80, 191, 32));
        buttonSouvenirCancel = new QPushButton(centralwidget);
        buttonSouvenirCancel->setObjectName("buttonSouvenirCancel");
        buttonSouvenirCancel->setGeometry(QRect(20, 370, 81, 31));
        distanceListView = new QListView(centralwidget);
        distanceListView->setObjectName("distanceListView");
        distanceListView->setGeometry(QRect(260, 30, 541, 410));
        buttonDistancesSubmit = new QPushButton(centralwidget);
        buttonDistancesSubmit->setObjectName("buttonDistancesSubmit");
        buttonDistancesSubmit->setGeometry(QRect(10, 200, 191, 32));
        dropdownDistances = new QComboBox(centralwidget);
        dropdownDistances->addItem(QString());
        dropdownDistances->setObjectName("dropdownDistances");
        dropdownDistances->setGeometry(QRect(10, 150, 190, 41));
        dropdownDistances->setEditable(false);
        labelTrip = new QLabel(centralwidget);
        labelTrip->setObjectName("labelTrip");
        labelTrip->setGeometry(QRect(840, 30, 91, 20));
        QFont font;
        font.setBold(true);
        labelTrip->setFont(font);
        buttonBasicTrip = new QPushButton(centralwidget);
        buttonBasicTrip->setObjectName("buttonBasicTrip");
        buttonBasicTrip->setGeometry(QRect(840, 60, 191, 32));
        buttonCustomTrip = new QPushButton(centralwidget);
        buttonCustomTrip->setObjectName("buttonCustomTrip");
        buttonCustomTrip->setGeometry(QRect(840, 110, 191, 32));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(20, 410, 81, 31));
        labelSouvenirs = new QLabel(centralwidget);
        labelSouvenirs->setObjectName("labelSouvenirs");
        labelSouvenirs->setGeometry(QRect(10, 10, 231, 16));
        labelSouvenirs->setFont(font);
        labelDistances = new QLabel(centralwidget);
        labelDistances->setObjectName("labelDistances");
        labelDistances->setGeometry(QRect(10, 130, 241, 16));
        labelDistances->setFont(font);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1115, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(buttonSouvenirPreview, SIGNAL(clicked()), MainWindow, SLOT(previewSouvenirButtonClick()));
        QObject::connect(buttonSouvenirCancel, SIGNAL(clicked()), MainWindow, SLOT(cancelButtonClick()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        dropdownSouvenirPreview->setItemText(0, QCoreApplication::translate("MainWindow", "Select a College", nullptr));

        buttonSouvenirPreview->setText(QCoreApplication::translate("MainWindow", "Submit", nullptr));
        buttonSouvenirCancel->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        buttonDistancesSubmit->setText(QCoreApplication::translate("MainWindow", "Submit", nullptr));
        dropdownDistances->setItemText(0, QCoreApplication::translate("MainWindow", "Select a College", nullptr));

        labelTrip->setText(QCoreApplication::translate("MainWindow", "Plan a Trip:", nullptr));
        buttonBasicTrip->setText(QCoreApplication::translate("MainWindow", "Basic Trip", nullptr));
        buttonCustomTrip->setText(QCoreApplication::translate("MainWindow", "Custom Trip", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Back to Login", nullptr));
        labelSouvenirs->setText(QCoreApplication::translate("MainWindow", "Select a College to Display Souvenirs", nullptr));
        labelDistances->setText(QCoreApplication::translate("MainWindow", "Display Distances from a Select College:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
