/********************************************************************************
** Form generated from reading UI file 'SerialNumberUI.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIALNUMBERUI_H
#define UI_SERIALNUMBERUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SerialNumberUI
{
public:
    QLabel *label;
    QLineEdit *lineEdit_serialNum;
    QLabel *label_2;
    QLabel *label_background;

    void setupUi(QWidget *SerialNumberUI)
    {
        if (SerialNumberUI->objectName().isEmpty())
            SerialNumberUI->setObjectName(QStringLiteral("SerialNumberUI"));
        SerialNumberUI->resize(400, 200);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SerialNumberUI->sizePolicy().hasHeightForWidth());
        SerialNumberUI->setSizePolicy(sizePolicy);
        SerialNumberUI->setStyleSheet(QStringLiteral("background-color: rgb(235, 238, 238);"));
        label = new QLabel(SerialNumberUI);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 40, 361, 31));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label->setAlignment(Qt::AlignCenter);
        lineEdit_serialNum = new QLineEdit(SerialNumberUI);
        lineEdit_serialNum->setObjectName(QStringLiteral("lineEdit_serialNum"));
        lineEdit_serialNum->setGeometry(QRect(90, 90, 221, 35));
        QFont font1;
        font1.setPointSize(16);
        lineEdit_serialNum->setFont(font1);
        lineEdit_serialNum->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        lineEdit_serialNum->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(SerialNumberUI);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 140, 361, 31));
        QFont font2;
        font2.setPointSize(14);
        label_2->setFont(font2);
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label_2->setAlignment(Qt::AlignCenter);
        label_background = new QLabel(SerialNumberUI);
        label_background->setObjectName(QStringLiteral("label_background"));
        label_background->setGeometry(QRect(10, 10, 381, 181));
        label_background->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label_background->setFrameShape(QFrame::Box);
        label_background->setFrameShadow(QFrame::Sunken);
        label_background->setLineWidth(1);
        label_background->setMidLineWidth(1);
        label_background->raise();
        label->raise();
        lineEdit_serialNum->raise();
        label_2->raise();

        retranslateUi(SerialNumberUI);

        QMetaObject::connectSlotsByName(SerialNumberUI);
    } // setupUi

    void retranslateUi(QWidget *SerialNumberUI)
    {
        SerialNumberUI->setWindowTitle(QApplication::translate("SerialNumberUI", "SerialNumberUI", nullptr));
        label->setText(QApplication::translate("SerialNumberUI", "\345\275\223\345\211\215\344\272\247\345\223\201\345\272\217\345\217\267", nullptr));
        label_2->setText(QApplication::translate("SerialNumberUI", "\357\274\210\351\200\200\345\207\272\350\257\267\350\276\223\345\205\24500000000\357\274\211", nullptr));
        label_background->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SerialNumberUI: public Ui_SerialNumberUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALNUMBERUI_H
