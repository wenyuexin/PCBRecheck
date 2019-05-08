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
#include <QtWidgets/QFrame>
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
    QLabel *label_cameraFrame;
    QFrame *line;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label_background;

    void setupUi(QWidget *SerialNumberUI)
    {
        if (SerialNumberUI->objectName().isEmpty())
            SerialNumberUI->setObjectName(QStringLiteral("SerialNumberUI"));
        SerialNumberUI->resize(839, 701);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SerialNumberUI->sizePolicy().hasHeightForWidth());
        SerialNumberUI->setSizePolicy(sizePolicy);
        SerialNumberUI->setStyleSheet(QStringLiteral("background-color: rgb(235, 238, 238);"));
        label = new QLabel(SerialNumberUI);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(70, 630, 101, 35));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEdit_serialNum = new QLineEdit(SerialNumberUI);
        lineEdit_serialNum->setObjectName(QStringLiteral("lineEdit_serialNum"));
        lineEdit_serialNum->setGeometry(QRect(190, 630, 311, 37));
        QFont font1;
        font1.setPointSize(17);
        lineEdit_serialNum->setFont(font1);
        lineEdit_serialNum->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        lineEdit_serialNum->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(SerialNumberUI);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(520, 630, 261, 35));
        QFont font2;
        font2.setPointSize(16);
        label_2->setFont(font2);
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label_2->setAlignment(Qt::AlignCenter);
        label_cameraFrame = new QLabel(SerialNumberUI);
        label_cameraFrame->setObjectName(QStringLiteral("label_cameraFrame"));
        label_cameraFrame->setGeometry(QRect(90, 110, 668, 501));
        label_cameraFrame->setStyleSheet(QStringLiteral("background-color: rgb(250, 250, 250);"));
        label_cameraFrame->setFrameShape(QFrame::NoFrame);
        label_cameraFrame->setAlignment(Qt::AlignCenter);
        line = new QFrame(SerialNumberUI);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(30, 60, 781, 5));
        line->setStyleSheet(QStringLiteral("background-color: rgb(240, 240, 248);"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_5 = new QLabel(SerialNumberUI);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(230, 20, 381, 41));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setMinimumSize(QSize(50, 30));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font3.setPointSize(20);
        font3.setBold(false);
        font3.setWeight(50);
        label_5->setFont(font3);
        label_5->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label_5->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(SerialNumberUI);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(70, 70, 111, 35));
        label_4->setFont(font);
        label_4->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_background = new QLabel(SerialNumberUI);
        label_background->setObjectName(QStringLiteral("label_background"));
        label_background->setGeometry(QRect(10, 10, 821, 681));
        label_background->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label_background->setFrameShape(QFrame::Box);
        label_background->setFrameShadow(QFrame::Sunken);
        label_background->setLineWidth(1);
        label_background->setMidLineWidth(1);
        label_background->raise();
        label->raise();
        lineEdit_serialNum->raise();
        label_2->raise();
        label_cameraFrame->raise();
        line->raise();
        label_5->raise();
        label_4->raise();

        retranslateUi(SerialNumberUI);

        QMetaObject::connectSlotsByName(SerialNumberUI);
    } // setupUi

    void retranslateUi(QWidget *SerialNumberUI)
    {
        SerialNumberUI->setWindowTitle(QApplication::translate("SerialNumberUI", "SerialNumberUI", nullptr));
        label->setText(QApplication::translate("SerialNumberUI", "\344\272\247\345\223\201\345\272\217\345\217\267", nullptr));
        label_2->setText(QApplication::translate("SerialNumberUI", "\357\274\210\351\200\200\345\207\272\347\263\273\347\273\237\350\257\267\350\276\223\345\205\24500000000\357\274\211", nullptr));
        label_cameraFrame->setText(QString());
        label_5->setText(QApplication::translate("SerialNumberUI", "PCB\345\244\215\346\237\245\347\263\273\347\273\237 - \344\272\247\345\223\201\345\272\217\345\217\267\350\257\206\345\210\253", nullptr));
        label_4->setText(QApplication::translate("SerialNumberUI", "\345\255\227\347\254\246\345\214\272\345\237\237", nullptr));
        label_background->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SerialNumberUI: public Ui_SerialNumberUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALNUMBERUI_H
