/********************************************************************************
** Form generated from reading UI file 'newwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWWINDOW_H
#define UI_NEWWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_NewWindow
{
public:
    QTableView *tableView;
    QPushButton *pushButton;

    void setupUi(QDialog *NewWindow)
    {
        if (NewWindow->objectName().isEmpty())
            NewWindow->setObjectName("NewWindow");
        NewWindow->resize(1106, 606);
        tableView = new QTableView(NewWindow);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(-5, 31, 481, 491));
        pushButton = new QPushButton(NewWindow);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(150, 180, 90, 29));

        retranslateUi(NewWindow);

        QMetaObject::connectSlotsByName(NewWindow);
    } // setupUi

    void retranslateUi(QDialog *NewWindow)
    {
        NewWindow->setWindowTitle(QCoreApplication::translate("NewWindow", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("NewWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewWindow: public Ui_NewWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWWINDOW_H
