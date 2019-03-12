/********************************************************************************
** Form generated from reading UI file 'ExitQueryUI.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXITQUERYUI_H
#define UI_EXITQUERYUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExitQueryUI
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_background;

    void setupUi(QWidget *ExitQueryUI)
    {
        if (ExitQueryUI->objectName().isEmpty())
            ExitQueryUI->setObjectName(QStringLiteral("ExitQueryUI"));
        ExitQueryUI->resize(400, 200);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ExitQueryUI->sizePolicy().hasHeightForWidth());
        ExitQueryUI->setSizePolicy(sizePolicy);
        ExitQueryUI->setStyleSheet(QStringLiteral("background-color: rgb(235, 235, 237);"));
        label = new QLabel(ExitQueryUI);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 40, 361, 31));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(ExitQueryUI);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 110, 361, 41));
        QFont font1;
        font1.setPointSize(14);
        label_2->setFont(font1);
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label_2->setAlignment(Qt::AlignCenter);
        label_background = new QLabel(ExitQueryUI);
        label_background->setObjectName(QStringLiteral("label_background"));
        label_background->setGeometry(QRect(10, 10, 381, 181));
        label_background->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 246);"));
        label_background->setFrameShape(QFrame::Box);
        label_background->setFrameShadow(QFrame::Sunken);
        label_background->setLineWidth(1);
        label_background->setMidLineWidth(1);
        label_background->raise();
        label->raise();
        label_2->raise();

        retranslateUi(ExitQueryUI);

        QMetaObject::connectSlotsByName(ExitQueryUI);
    } // setupUi

    void retranslateUi(QWidget *ExitQueryUI)
    {
        ExitQueryUI->setWindowTitle(QApplication::translate("ExitQueryUI", "ExitQueryUI", nullptr));
        label->setText(QApplication::translate("ExitQueryUI", "\346\230\257\345\220\246\346\243\200\344\277\256\344\270\213\344\270\200\346\240\267\346\234\254\357\274\237", nullptr));
        label_2->setText(QApplication::translate("ExitQueryUI", "\357\274\210\346\243\200\344\277\256\344\270\213\344\270\200\346\240\267\346\234\254\346\210\226\351\200\200\345\207\272\350\257\267\346\214\211\342\200\234+\342\200\235\n"
"  \350\277\224\345\233\236\345\275\223\345\211\215\346\243\200\344\277\256\347\225\214\351\235\242\350\257\267\346\214\211\342\200\234-\342\200\235\357\274\211", nullptr));
        label_background->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ExitQueryUI: public Ui_ExitQueryUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXITQUERYUI_H
