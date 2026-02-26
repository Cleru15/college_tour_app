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
