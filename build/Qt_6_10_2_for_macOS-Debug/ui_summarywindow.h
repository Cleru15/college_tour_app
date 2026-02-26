/********************************************************************************
** Form generated from reading UI file 'summarywindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUMMARYWINDOW_H
#define UI_SUMMARYWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>

QT_BEGIN_NAMESPACE

class Ui_summaryWindow
{
public:
    QListView *listView;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *summaryWindow)
    {
        if (summaryWindow->objectName().isEmpty())
            summaryWindow->setObjectName("summaryWindow");
        summaryWindow->resize(1114, 658);
        listView = new QListView(summaryWindow);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(430, 40, 631, 531));
        label = new QLabel(summaryWindow);
        label->setObjectName("label");
        label->setGeometry(QRect(80, 100, 171, 20));
        label_2 = new QLabel(summaryWindow);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(80, 260, 171, 20));
        label_3 = new QLabel(summaryWindow);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(80, 430, 171, 20));

        retranslateUi(summaryWindow);

        QMetaObject::connectSlotsByName(summaryWindow);
    } // setupUi

    void retranslateUi(QDialog *summaryWindow)
    {
        summaryWindow->setWindowTitle(QCoreApplication::translate("summaryWindow", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("summaryWindow", "Distance Traveled: ", nullptr));
        label_2->setText(QCoreApplication::translate("summaryWindow", "Souvenirs Purchased:", nullptr));
        label_3->setText(QCoreApplication::translate("summaryWindow", "Souvenir Costs:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class summaryWindow: public Ui_summaryWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUMMARYWINDOW_H
