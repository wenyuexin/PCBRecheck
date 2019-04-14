/********************************************************************************
** Form generated from reading UI file 'PCBRecheck.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PCBRECHECK_H
#define UI_PCBRECHECK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PCBRecheckClass
{
public:
    QWidget *centralWidget;
    QFrame *line;
    QGraphicsView *graphicsView_full;
    QLabel *label_5;
    QLabel *label_logo;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_minus2;
    QPushButton *pushButton_plus2;
    QPushButton *pushButton_exit2;
    QLabel *label_flaw;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_22;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_23;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_indicator1;
    QLabel *label_indicator1_2;
    QSpacerItem *horizontalSpacer_24;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_indicator2;
    QLabel *label_indicator1_3;
    QSpacerItem *horizontalSpacer_25;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_indicator3;
    QLabel *label_indicator1_4;
    QSpacerItem *horizontalSpacer_26;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_indicator4;
    QLabel *label_indicator1_5;
    QSpacerItem *horizontalSpacer_27;
    QSpacerItem *verticalSpacer_8;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_28;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_29;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QLabel *label_xLoc;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_yLoc;
    QSpacerItem *horizontalSpacer_30;
    QSpacerItem *verticalSpacer_9;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_31;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_32;
    QLabel *label_serialNum;
    QSpacerItem *horizontalSpacer_33;
    QLabel *label_titleBarBackground;
    QLabel *label_3;
    QLabel *label_defectNum;

    void setupUi(QMainWindow *PCBRecheckClass)
    {
        if (PCBRecheckClass->objectName().isEmpty())
            PCBRecheckClass->setObjectName(QStringLiteral("PCBRecheckClass"));
        PCBRecheckClass->resize(1366, 768);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PCBRecheckClass->sizePolicy().hasHeightForWidth());
        PCBRecheckClass->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(PCBRecheckClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 20, 1371, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        graphicsView_full = new QGraphicsView(centralWidget);
        graphicsView_full->setObjectName(QStringLiteral("graphicsView_full"));
        graphicsView_full->setEnabled(true);
        graphicsView_full->setGeometry(QRect(-1, 30, 731, 731));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(graphicsView_full->sizePolicy().hasHeightForWidth());
        graphicsView_full->setSizePolicy(sizePolicy1);
        graphicsView_full->setMinimumSize(QSize(580, 696));
        graphicsView_full->setMaximumSize(QSize(1000, 1200));
        graphicsView_full->setBaseSize(QSize(0, 0));
        graphicsView_full->setStyleSheet(QStringLiteral("background-color: rgb(250, 250, 250);"));
        graphicsView_full->setFrameShape(QFrame::NoFrame);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(650, 0, 150, 30));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setMinimumSize(QSize(50, 30));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(18);
        font.setBold(false);
        font.setWeight(50);
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignCenter);
        label_logo = new QLabel(centralWidget);
        label_logo->setObjectName(QStringLiteral("label_logo"));
        label_logo->setGeometry(QRect(620, 0, 30, 30));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_logo->sizePolicy().hasHeightForWidth());
        label_logo->setSizePolicy(sizePolicy2);
        label_logo->setMaximumSize(QSize(16777215, 35));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label_logo->setFont(font1);
        label_logo->setStyleSheet(QStringLiteral("border-image: url(:/Icons/logo.png);"));
        layoutWidget_2 = new QWidget(centralWidget);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(760, 530, 77, 191));
        verticalLayout = new QVBoxLayout(layoutWidget_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_minus2 = new QPushButton(layoutWidget_2);
        pushButton_minus2->setObjectName(QStringLiteral("pushButton_minus2"));
        sizePolicy.setHeightForWidth(pushButton_minus2->sizePolicy().hasHeightForWidth());
        pushButton_minus2->setSizePolicy(sizePolicy);
        pushButton_minus2->setMinimumSize(QSize(0, 35));
        QFont font2;
        font2.setPointSize(16);
        font2.setBold(true);
        font2.setWeight(75);
        pushButton_minus2->setFont(font2);

        verticalLayout->addWidget(pushButton_minus2);

        pushButton_plus2 = new QPushButton(layoutWidget_2);
        pushButton_plus2->setObjectName(QStringLiteral("pushButton_plus2"));
        sizePolicy.setHeightForWidth(pushButton_plus2->sizePolicy().hasHeightForWidth());
        pushButton_plus2->setSizePolicy(sizePolicy);
        pushButton_plus2->setMinimumSize(QSize(0, 35));
        pushButton_plus2->setSizeIncrement(QSize(0, 0));
        QFont font3;
        font3.setPointSize(16);
        font3.setBold(false);
        font3.setWeight(50);
        pushButton_plus2->setFont(font3);

        verticalLayout->addWidget(pushButton_plus2);

        pushButton_exit2 = new QPushButton(layoutWidget_2);
        pushButton_exit2->setObjectName(QStringLiteral("pushButton_exit2"));
        sizePolicy.setHeightForWidth(pushButton_exit2->sizePolicy().hasHeightForWidth());
        pushButton_exit2->setSizePolicy(sizePolicy);
        pushButton_exit2->setMinimumSize(QSize(0, 35));
        QFont font4;
        font4.setPointSize(14);
        pushButton_exit2->setFont(font4);

        verticalLayout->addWidget(pushButton_exit2);

        label_flaw = new QLabel(centralWidget);
        label_flaw->setObjectName(QStringLiteral("label_flaw"));
        label_flaw->setGeometry(QRect(880, 50, 450, 450));
        sizePolicy.setHeightForWidth(label_flaw->sizePolicy().hasHeightForWidth());
        label_flaw->setSizePolicy(sizePolicy);
        label_flaw->setStyleSheet(QStringLiteral("background-color: rgb(250, 250, 250);"));
        label_flaw->setFrameShape(QFrame::NoFrame);
        label_flaw->setFrameShadow(QFrame::Sunken);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(870, 510, 461, 221));
        verticalLayout_6 = new QVBoxLayout(layoutWidget);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_22 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_22);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        QFont font5;
        font5.setPointSize(16);
        label_4->setFont(font5);

        horizontalLayout->addWidget(label_4);

        horizontalSpacer_23 = new QSpacerItem(25, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_23);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_indicator1 = new QLabel(layoutWidget);
        label_indicator1->setObjectName(QStringLiteral("label_indicator1"));
        sizePolicy.setHeightForWidth(label_indicator1->sizePolicy().hasHeightForWidth());
        label_indicator1->setSizePolicy(sizePolicy);
        label_indicator1->setMinimumSize(QSize(40, 40));
        label_indicator1->setMaximumSize(QSize(40, 40));
        QFont font6;
        font6.setPointSize(18);
        label_indicator1->setFont(font6);
        label_indicator1->setFrameShape(QFrame::NoFrame);
        label_indicator1->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_indicator1);

        label_indicator1_2 = new QLabel(layoutWidget);
        label_indicator1_2->setObjectName(QStringLiteral("label_indicator1_2"));
        sizePolicy.setHeightForWidth(label_indicator1_2->sizePolicy().hasHeightForWidth());
        label_indicator1_2->setSizePolicy(sizePolicy);
        label_indicator1_2->setMinimumSize(QSize(40, 35));
        label_indicator1_2->setMaximumSize(QSize(40, 35));
        QFont font7;
        font7.setPointSize(18);
        font7.setBold(false);
        font7.setWeight(50);
        label_indicator1_2->setFont(font7);
        label_indicator1_2->setFrameShape(QFrame::NoFrame);
        label_indicator1_2->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        verticalLayout_2->addWidget(label_indicator1_2);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer_24 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_24);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_indicator2 = new QLabel(layoutWidget);
        label_indicator2->setObjectName(QStringLiteral("label_indicator2"));
        sizePolicy.setHeightForWidth(label_indicator2->sizePolicy().hasHeightForWidth());
        label_indicator2->setSizePolicy(sizePolicy);
        label_indicator2->setMinimumSize(QSize(40, 40));
        label_indicator2->setMaximumSize(QSize(40, 40));
        label_indicator2->setFont(font6);
        label_indicator2->setFrameShape(QFrame::NoFrame);
        label_indicator2->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_indicator2);

        label_indicator1_3 = new QLabel(layoutWidget);
        label_indicator1_3->setObjectName(QStringLiteral("label_indicator1_3"));
        sizePolicy.setHeightForWidth(label_indicator1_3->sizePolicy().hasHeightForWidth());
        label_indicator1_3->setSizePolicy(sizePolicy);
        label_indicator1_3->setMinimumSize(QSize(40, 35));
        label_indicator1_3->setMaximumSize(QSize(40, 35));
        label_indicator1_3->setFont(font7);
        label_indicator1_3->setFrameShape(QFrame::NoFrame);
        label_indicator1_3->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        verticalLayout_3->addWidget(label_indicator1_3);


        horizontalLayout->addLayout(verticalLayout_3);

        horizontalSpacer_25 = new QSpacerItem(15, 47, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_25);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_indicator3 = new QLabel(layoutWidget);
        label_indicator3->setObjectName(QStringLiteral("label_indicator3"));
        sizePolicy.setHeightForWidth(label_indicator3->sizePolicy().hasHeightForWidth());
        label_indicator3->setSizePolicy(sizePolicy);
        label_indicator3->setMinimumSize(QSize(40, 40));
        label_indicator3->setMaximumSize(QSize(50, 50));
        label_indicator3->setFont(font6);
        label_indicator3->setFrameShape(QFrame::NoFrame);
        label_indicator3->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_indicator3);

        label_indicator1_4 = new QLabel(layoutWidget);
        label_indicator1_4->setObjectName(QStringLiteral("label_indicator1_4"));
        sizePolicy.setHeightForWidth(label_indicator1_4->sizePolicy().hasHeightForWidth());
        label_indicator1_4->setSizePolicy(sizePolicy);
        label_indicator1_4->setMinimumSize(QSize(40, 35));
        label_indicator1_4->setMaximumSize(QSize(40, 35));
        label_indicator1_4->setFont(font7);
        label_indicator1_4->setFrameShape(QFrame::NoFrame);
        label_indicator1_4->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        verticalLayout_4->addWidget(label_indicator1_4);


        horizontalLayout->addLayout(verticalLayout_4);

        horizontalSpacer_26 = new QSpacerItem(15, 47, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_26);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_indicator4 = new QLabel(layoutWidget);
        label_indicator4->setObjectName(QStringLiteral("label_indicator4"));
        sizePolicy.setHeightForWidth(label_indicator4->sizePolicy().hasHeightForWidth());
        label_indicator4->setSizePolicy(sizePolicy);
        label_indicator4->setMinimumSize(QSize(40, 40));
        label_indicator4->setMaximumSize(QSize(40, 40));
        label_indicator4->setFont(font6);
        label_indicator4->setFrameShape(QFrame::NoFrame);
        label_indicator4->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_indicator4);

        label_indicator1_5 = new QLabel(layoutWidget);
        label_indicator1_5->setObjectName(QStringLiteral("label_indicator1_5"));
        sizePolicy.setHeightForWidth(label_indicator1_5->sizePolicy().hasHeightForWidth());
        label_indicator1_5->setSizePolicy(sizePolicy);
        label_indicator1_5->setMinimumSize(QSize(40, 35));
        label_indicator1_5->setMaximumSize(QSize(40, 35));
        label_indicator1_5->setFont(font7);
        label_indicator1_5->setFrameShape(QFrame::NoFrame);
        label_indicator1_5->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        verticalLayout_5->addWidget(label_indicator1_5);


        horizontalLayout->addLayout(verticalLayout_5);

        horizontalSpacer_27 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_27);


        verticalLayout_6->addLayout(horizontalLayout);

        verticalSpacer_8 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_6->addItem(verticalSpacer_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalSpacer_28 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_28);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);
        label_7->setFont(font5);

        horizontalLayout_9->addWidget(label_7);

        horizontalSpacer_29 = new QSpacerItem(25, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_29);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(10, 0));
        QFont font8;
        font8.setPointSize(17);
        font8.setBold(false);
        font8.setWeight(50);
        label->setFont(font8);

        horizontalLayout_9->addWidget(label);

        horizontalSpacer = new QSpacerItem(3, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer);

        label_xLoc = new QLabel(layoutWidget);
        label_xLoc->setObjectName(QStringLiteral("label_xLoc"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_xLoc->sizePolicy().hasHeightForWidth());
        label_xLoc->setSizePolicy(sizePolicy3);
        label_xLoc->setMinimumSize(QSize(85, 35));
        label_xLoc->setMaximumSize(QSize(85, 35));
        label_xLoc->setFont(font5);
        label_xLoc->setStyleSheet(QStringLiteral("background-color: rgb(250, 250, 250);"));
        label_xLoc->setFrameShape(QFrame::Box);
        label_xLoc->setFrameShadow(QFrame::Plain);
        label_xLoc->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(label_xLoc);

        horizontalSpacer_2 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_2);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(10, 0));
        label_2->setFont(font8);

        horizontalLayout_9->addWidget(label_2);

        horizontalSpacer_3 = new QSpacerItem(3, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_3);

        label_yLoc = new QLabel(layoutWidget);
        label_yLoc->setObjectName(QStringLiteral("label_yLoc"));
        sizePolicy.setHeightForWidth(label_yLoc->sizePolicy().hasHeightForWidth());
        label_yLoc->setSizePolicy(sizePolicy);
        label_yLoc->setMinimumSize(QSize(85, 35));
        label_yLoc->setMaximumSize(QSize(85, 35));
        label_yLoc->setFont(font5);
        label_yLoc->setStyleSheet(QStringLiteral("background-color: rgb(250, 250, 250);"));
        label_yLoc->setFrameShape(QFrame::Box);
        label_yLoc->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(label_yLoc);

        horizontalSpacer_30 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_30);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(2, 2);
        horizontalLayout_9->setStretch(10, 3);

        verticalLayout_6->addLayout(horizontalLayout_9);

        verticalSpacer_9 = new QSpacerItem(15, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_6->addItem(verticalSpacer_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalSpacer_31 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_31);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setFont(font5);

        horizontalLayout_10->addWidget(label_6);

        horizontalSpacer_32 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_32);

        label_serialNum = new QLabel(layoutWidget);
        label_serialNum->setObjectName(QStringLiteral("label_serialNum"));
        sizePolicy.setHeightForWidth(label_serialNum->sizePolicy().hasHeightForWidth());
        label_serialNum->setSizePolicy(sizePolicy);
        label_serialNum->setMinimumSize(QSize(210, 35));
        label_serialNum->setMaximumSize(QSize(210, 35));
        label_serialNum->setFont(font5);
        label_serialNum->setStyleSheet(QStringLiteral("background-color: rgb(250, 250, 250);"));
        label_serialNum->setFrameShape(QFrame::Box);
        label_serialNum->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(label_serialNum);

        horizontalSpacer_33 = new QSpacerItem(55, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_33);


        verticalLayout_6->addLayout(horizontalLayout_10);

        label_titleBarBackground = new QLabel(centralWidget);
        label_titleBarBackground->setObjectName(QStringLiteral("label_titleBarBackground"));
        label_titleBarBackground->setGeometry(QRect(3, 0, 1371, 31));
        label_titleBarBackground->setStyleSheet(QStringLiteral("background-color: rgb(240, 240, 248);"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(750, 200, 101, 31));
        label_3->setFont(font6);
        label_3->setAlignment(Qt::AlignCenter);
        label_defectNum = new QLabel(centralWidget);
        label_defectNum->setObjectName(QStringLiteral("label_defectNum"));
        label_defectNum->setGeometry(QRect(750, 250, 100, 35));
        sizePolicy3.setHeightForWidth(label_defectNum->sizePolicy().hasHeightForWidth());
        label_defectNum->setSizePolicy(sizePolicy3);
        label_defectNum->setMinimumSize(QSize(100, 35));
        label_defectNum->setMaximumSize(QSize(100, 35));
        label_defectNum->setFont(font5);
        label_defectNum->setStyleSheet(QStringLiteral("background-color: rgb(250, 250, 250);"));
        label_defectNum->setFrameShape(QFrame::Box);
        label_defectNum->setFrameShadow(QFrame::Plain);
        label_defectNum->setAlignment(Qt::AlignCenter);
        PCBRecheckClass->setCentralWidget(centralWidget);
        label_titleBarBackground->raise();
        line->raise();
        graphicsView_full->raise();
        label_5->raise();
        label_logo->raise();
        layoutWidget_2->raise();
        label_flaw->raise();
        layoutWidget->raise();
        label_3->raise();
        label_defectNum->raise();

        retranslateUi(PCBRecheckClass);

        QMetaObject::connectSlotsByName(PCBRecheckClass);
    } // setupUi

    void retranslateUi(QMainWindow *PCBRecheckClass)
    {
        PCBRecheckClass->setWindowTitle(QApplication::translate("PCBRecheckClass", "PCBRecheck", nullptr));
        label_5->setText(QApplication::translate("PCBRecheckClass", "PCB\346\243\200\344\277\256\347\263\273\347\273\237", nullptr));
        label_logo->setText(QApplication::translate("PCBRecheckClass", "logo", nullptr));
        pushButton_minus2->setText(QApplication::translate("PCBRecheckClass", "-", nullptr));
        pushButton_plus2->setText(QApplication::translate("PCBRecheckClass", "+", nullptr));
        pushButton_exit2->setText(QApplication::translate("PCBRecheckClass", "\351\200\200\345\207\272", nullptr));
        label_flaw->setText(QString());
        label_4->setText(QApplication::translate("PCBRecheckClass", "\347\274\272\351\231\267\347\261\273\345\236\213", nullptr));
        label_indicator1->setText(QApplication::translate("PCBRecheckClass", "L1", nullptr));
        label_indicator1_2->setText(QApplication::translate("PCBRecheckClass", "A", nullptr));
        label_indicator2->setText(QApplication::translate("PCBRecheckClass", "L2", nullptr));
        label_indicator1_3->setText(QApplication::translate("PCBRecheckClass", "B", nullptr));
        label_indicator3->setText(QApplication::translate("PCBRecheckClass", "L3", nullptr));
        label_indicator1_4->setText(QApplication::translate("PCBRecheckClass", "C", nullptr));
        label_indicator4->setText(QApplication::translate("PCBRecheckClass", "L4", nullptr));
        label_indicator1_5->setText(QApplication::translate("PCBRecheckClass", "D", nullptr));
        label_7->setText(QApplication::translate("PCBRecheckClass", "\347\274\272\351\231\267\344\275\215\347\275\256", nullptr));
        label->setText(QApplication::translate("PCBRecheckClass", "X", nullptr));
        label_xLoc->setText(QString());
        label_2->setText(QApplication::translate("PCBRecheckClass", "Y", nullptr));
        label_yLoc->setText(QString());
        label_6->setText(QApplication::translate("PCBRecheckClass", "\344\272\247\345\223\201\345\272\217\345\217\267", nullptr));
        label_serialNum->setText(QString());
        label_titleBarBackground->setText(QString());
        label_3->setText(QApplication::translate("PCBRecheckClass", "\347\274\272\351\231\267\346\200\273\346\225\260", nullptr));
        label_defectNum->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PCBRecheckClass: public Ui_PCBRecheckClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PCBRECHECK_H
