/********************************************************************************
** Form generated from reading UI file 'MyMessageBox.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYMESSAGEBOX_H
#define UI_MYMESSAGEBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyMessageBox
{
public:
    QPushButton *pushButton;
    QLabel *label_message;
    QFrame *line;

    void setupUi(QWidget *MyMessageBox)
    {
        if (MyMessageBox->objectName().isEmpty())
            MyMessageBox->setObjectName(QStringLiteral("MyMessageBox"));
        MyMessageBox->resize(410, 117);
        pushButton = new QPushButton(MyMessageBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(310, 80, 75, 28));
        QFont font;
        font.setPointSize(12);
        font.setStyleStrategy(QFont::PreferAntialias);
        pushButton->setFont(font);
        label_message = new QLabel(MyMessageBox);
        label_message->setObjectName(QStringLiteral("label_message"));
        label_message->setGeometry(QRect(20, 10, 381, 51));
        QFont font1;
        font1.setFamily(QStringLiteral("SimSun-ExtB"));
        font1.setPointSize(12);
        font1.setKerning(true);
        font1.setStyleStrategy(QFont::PreferAntialias);
        label_message->setFont(font1);
        label_message->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        line = new QFrame(MyMessageBox);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 70, 391, 3));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        retranslateUi(MyMessageBox);

        QMetaObject::connectSlotsByName(MyMessageBox);
    } // setupUi

    void retranslateUi(QWidget *MyMessageBox)
    {
        MyMessageBox->setWindowTitle(QApplication::translate("MyMessageBox", "MyMessageBox", nullptr));
        pushButton->setText(QApplication::translate("MyMessageBox", "\347\241\256\345\256\232", nullptr));
        label_message->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MyMessageBox: public Ui_MyMessageBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYMESSAGEBOX_H
