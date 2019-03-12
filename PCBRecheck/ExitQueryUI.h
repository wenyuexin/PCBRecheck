#pragma once

#include <QWidget>
#include "ui_ExitQueryUI.h"
#include "ExitQueryUI.h"
#include <QKeyEvent>

//�˳�ѯ�ʽ���
//
class ExitQueryUI : public QWidget
{
	Q_OBJECT

private:
	Ui::ExitQueryUI ui;

public:
	ExitQueryUI(QWidget *parent = Q_NULLPTR);
	~ExitQueryUI();

Q_SIGNALS:
	void showSerialNumberUI_exitUI();
	void showRecheckUI_exitUI();

private Q_SLOTS:
	void keyPressEvent(QKeyEvent *event);
};