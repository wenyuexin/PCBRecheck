#include "ExitQueryUI.h"

ExitQueryUI::ExitQueryUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QPalette palette;
	palette.setColor(QPalette::Background, QColor(246, 246, 246));
	ui.label_background->setAutoFillBackground(true); //缺陷图
	ui.label_background->setPalette(palette);

	//去掉标题栏
	this->setWindowFlags(Qt::FramelessWindowHint);

	//此界面显示时禁用其他窗口
	this->setWindowModality(Qt::ApplicationModal);
}

ExitQueryUI::~ExitQueryUI()
{
}

/***************** 事件触发：敲击键盘 ****************/

//敲击小键盘的 + -
void ExitQueryUI::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Plus:
		emit showSerialNumberUI_exitUI();
		break;

	case Qt::Key_Minus:
		emit showRecheckUI_exitUI();
		break;

	default:
		break;
	}
}