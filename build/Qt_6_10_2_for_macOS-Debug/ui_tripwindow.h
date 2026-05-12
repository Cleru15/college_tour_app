<<<<<<< HEAD
/********************************************************************************
** Form generated from reading UI file 'tripwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRIPWINDOW_H
#define UI_TRIPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_tripWindow
{
public:
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QListView *listView;

    void setupUi(QDialog *tripWindow)
    {
        if (tripWindow->objectName().isEmpty())
            tripWindow->setObjectName("tripWindow");
        tripWindow->resize(1027, 633);
        pushButton = new QPushButton(tripWindow);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(60, 510, 161, 29));
        label = new QLabel(tripWindow);
        label->setObjectName("label");
        label->setGeometry(QRect(70, 430, 121, 20));
        label_2 = new QLabel(tripWindow);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(70, 470, 181, 20));
        label_3 = new QLabel(tripWindow);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(70, 230, 151, 20));
        label_4 = new QLabel(tripWindow);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(70, 270, 121, 20));
        label_5 = new QLabel(tripWindow);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(70, 70, 121, 20));
        label_6 = new QLabel(tripWindow);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(70, 100, 131, 20));
        listView = new QListView(tripWindow);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(290, 70, 681, 451));

        retranslateUi(tripWindow);

        QMetaObject::connectSlotsByName(tripWindow);
    } // setupUi

    void retranslateUi(QDialog *tripWindow)
    {
        tripWindow->setWindowTitle(QCoreApplication::translate("tripWindow", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("tripWindow", "See Summary", nullptr));
        label->setText(QCoreApplication::translate("tripWindow", "Next College:", nullptr));
        label_2->setText(QCoreApplication::translate("tripWindow", "Distance to Next College:", nullptr));
        label_3->setText(QCoreApplication::translate("tripWindow", "Souvenirs Purchased: ", nullptr));
        label_4->setText(QCoreApplication::translate("tripWindow", "Souvenirs Cost: ", nullptr));
        label_5->setText(QCoreApplication::translate("tripWindow", "Current College:", nullptr));
        label_6->setText(QCoreApplication::translate("tripWindow", "Distance Traveled: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class tripWindow: public Ui_tripWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIPWINDOW_H
=======
/********************************************************************************
** Form generated from reading UI file 'tripwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRIPWINDOW_H
#define UI_TRIPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_tripWindow
{
public:
    QPushButton *buttNextSum;
    QLabel *labelNextCol;
    QLabel *labelDistNext;
    QLabel *labelSouvPurc;
    QLabel *labelSouvCost;
    QLabel *labelCurrCol;
    QLabel *labelDistTrav;
    QTableWidget *souvDisplay;
    QLabel *labelCheckAll;
    QLabel *labelBuySouv;
    QPushButton *buyButt;
    QListView *collegeDisplay;
    QLineEdit *lineCurrColl;
    QLineEdit *lineDistTrav;
    QLineEdit *lineNextColl;
    QLineEdit *lineDistNext;
    QLineEdit *lineSouvPurc;
    QLineEdit *lineSouvCost;

    void setupUi(QDialog *tripWindow)
    {
        if (tripWindow->objectName().isEmpty())
            tripWindow->setObjectName("tripWindow");
        tripWindow->resize(939, 590);
        buttNextSum = new QPushButton(tripWindow);
        buttNextSum->setObjectName("buttNextSum");
        buttNextSum->setGeometry(QRect(30, 470, 161, 29));
        labelNextCol = new QLabel(tripWindow);
        labelNextCol->setObjectName("labelNextCol");
        labelNextCol->setGeometry(QRect(20, 340, 121, 20));
        labelDistNext = new QLabel(tripWindow);
        labelDistNext->setObjectName("labelDistNext");
        labelDistNext->setGeometry(QRect(20, 380, 181, 20));
        labelSouvPurc = new QLabel(tripWindow);
        labelSouvPurc->setObjectName("labelSouvPurc");
        labelSouvPurc->setGeometry(QRect(20, 230, 151, 20));
        labelSouvCost = new QLabel(tripWindow);
        labelSouvCost->setObjectName("labelSouvCost");
        labelSouvCost->setGeometry(QRect(20, 250, 121, 20));
        labelCurrCol = new QLabel(tripWindow);
        labelCurrCol->setObjectName("labelCurrCol");
        labelCurrCol->setGeometry(QRect(20, 40, 121, 20));
        labelDistTrav = new QLabel(tripWindow);
        labelDistTrav->setObjectName("labelDistTrav");
        labelDistTrav->setGeometry(QRect(20, 70, 131, 20));
        souvDisplay = new QTableWidget(tripWindow);
        souvDisplay->setObjectName("souvDisplay");
        souvDisplay->setGeometry(QRect(450, 320, 471, 211));
        labelCheckAll = new QLabel(tripWindow);
        labelCheckAll->setObjectName("labelCheckAll");
        labelCheckAll->setGeometry(QRect(450, 290, 461, 31));
        QFont font;
        font.setFamilies({QString::fromUtf8("Perpetua Titling MT")});
        font.setPointSize(8);
        font.setBold(true);
        labelCheckAll->setFont(font);
        labelBuySouv = new QLabel(tripWindow);
        labelBuySouv->setObjectName("labelBuySouv");
        labelBuySouv->setGeometry(QRect(450, 540, 211, 31));
        labelBuySouv->setFont(font);
        buyButt = new QPushButton(tripWindow);
        buyButt->setObjectName("buyButt");
        buyButt->setGeometry(QRect(830, 540, 91, 31));
        collegeDisplay = new QListView(tripWindow);
        collegeDisplay->setObjectName("collegeDisplay");
        collegeDisplay->setGeometry(QRect(450, 20, 471, 271));
        lineCurrColl = new QLineEdit(tripWindow);
        lineCurrColl->setObjectName("lineCurrColl");
        lineCurrColl->setGeometry(QRect(160, 40, 231, 21));
        lineDistTrav = new QLineEdit(tripWindow);
        lineDistTrav->setObjectName("lineDistTrav");
        lineDistTrav->setGeometry(QRect(160, 70, 231, 21));
        lineNextColl = new QLineEdit(tripWindow);
        lineNextColl->setObjectName("lineNextColl");
        lineNextColl->setGeometry(QRect(160, 340, 231, 21));
        lineDistNext = new QLineEdit(tripWindow);
        lineDistNext->setObjectName("lineDistNext");
        lineDistNext->setGeometry(QRect(160, 380, 231, 21));
        lineSouvPurc = new QLineEdit(tripWindow);
        lineSouvPurc->setObjectName("lineSouvPurc");
        lineSouvPurc->setGeometry(QRect(150, 230, 231, 21));
        lineSouvCost = new QLineEdit(tripWindow);
        lineSouvCost->setObjectName("lineSouvCost");
        lineSouvCost->setGeometry(QRect(150, 260, 231, 21));

        retranslateUi(tripWindow);

        QMetaObject::connectSlotsByName(tripWindow);
    } // setupUi

    void retranslateUi(QDialog *tripWindow)
    {
        tripWindow->setWindowTitle(QCoreApplication::translate("tripWindow", "Dialog", nullptr));
        buttNextSum->setText(QCoreApplication::translate("tripWindow", "See Summary", nullptr));
        labelNextCol->setText(QCoreApplication::translate("tripWindow", "Next College:", nullptr));
        labelDistNext->setText(QCoreApplication::translate("tripWindow", "Distance to Next College:", nullptr));
        labelSouvPurc->setText(QCoreApplication::translate("tripWindow", "Souvenirs Purchased: ", nullptr));
        labelSouvCost->setText(QCoreApplication::translate("tripWindow", "Souvenirs Cost: ", nullptr));
        labelCurrCol->setText(QCoreApplication::translate("tripWindow", "Current College:", nullptr));
        labelDistTrav->setText(QCoreApplication::translate("tripWindow", "Distance Traveled: ", nullptr));
        labelCheckAll->setText(QCoreApplication::translate("tripWindow", "Check All the Souvenirs that you want: ", nullptr));
        labelBuySouv->setText(QCoreApplication::translate("tripWindow", "Hit Buy to purshase your souvenirs:", nullptr));
        buyButt->setText(QCoreApplication::translate("tripWindow", "Buy", nullptr));
    } // retranslateUi

};

namespace Ui {
    class tripWindow: public Ui_tripWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIPWINDOW_H
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
