#pragma once

#include <QWidget>
#include "ui_MyMessageBox.h"
#include "RuntimeLib.h"
#include <QDesktopWidget>
#include <QKeyEvent>


namespace pcb {
#ifndef RECHECK_ENUM_MESSAGE_BOX_TYPE
#define RECHECK_ENUM_MESSAGE_BOX_TYPE
	//��������
	enum MessageBoxType {
		Warning,
		Information,
		Question,
		Critical,
		About
	};
#endif //RECHECK_ENUM_MESSAGE_BOX_TYPE
}


//������ʾ
class MyMessageBox : public QWidget
{
	Q_OBJECT

private:
	Ui::MyMessageBox ui;
	bool running;
	QString boxTitle;
	QString message;

public:
	MyMessageBox();
	~MyMessageBox();

	void doShow();
	void set(pcb::MessageBoxType type, QString msg, QString title = "");

private Q_SLOTS:
	void keyPressEvent(QKeyEvent *event);
	void on_pushButton_clicked();
};
