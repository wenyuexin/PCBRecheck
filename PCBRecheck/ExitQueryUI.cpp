#include "ExitQueryUI.h"



ExitQueryUI::ExitQueryUI(QWidget *parent, QRect &screenRect)
	: QWidget(parent)
{
	ui.setupUi(this);

	//判断并选择在主屏或者是副屏上显示
	QPoint widgetPos = screenRect.center();
	widgetPos -= QPoint(this->width() / 2.0, this->height() / 2.0);
	QRect widgetRect = QRect(widgetPos, this->size());
	this->setGeometry(widgetRect);

	//填充颜色
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
	qDebug() << "~ExitQueryUI";
}


/***************** 事件触发：敲击键盘 ****************/

//敲击小键盘的 + -
void ExitQueryUI::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Plus:
		qDebug() << "Key_Plus";
		emit showSerialNumberUI_exitUI();
		break;
	case Qt::Key_Minus:
		qDebug() << "Key_Minus";
		emit showRecheckUI_exitUI();
		break;
	default:
		break;
	}
}