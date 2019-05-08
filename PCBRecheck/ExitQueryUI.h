#pragma once

#include <QWidget>
#include "ui_ExitQueryUI.h"
#include "ExitQueryUI.h"
#include <QKeyEvent>
#include <QDebug>


//退出询问界面
class ExitQueryUI : public QWidget
{
	Q_OBJECT

private:
	Ui::ExitQueryUI ui;

public:
	ExitQueryUI(QWidget *parent = Q_NULLPTR, QRect &screenRect = QRect());
	~ExitQueryUI();

Q_SIGNALS:
	void showSerialNumberUI_exitUI();
	void showRecheckUI_exitUI();

private Q_SLOTS:
	void keyPressEvent(QKeyEvent *event);
};
