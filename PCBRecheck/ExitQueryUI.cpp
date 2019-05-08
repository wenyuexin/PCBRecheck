#include "ExitQueryUI.h"



ExitQueryUI::ExitQueryUI(QWidget *parent, QRect &screenRect)
	: QWidget(parent)
{
	ui.setupUi(this);

	//�жϲ�ѡ�������������Ǹ�������ʾ
	QPoint widgetPos = screenRect.center();
	widgetPos -= QPoint(this->width() / 2.0, this->height() / 2.0);
	QRect widgetRect = QRect(widgetPos, this->size());
	this->setGeometry(widgetRect);

	//�����ɫ
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(246, 246, 246));
	ui.label_background->setAutoFillBackground(true); //ȱ��ͼ
	ui.label_background->setPalette(palette);

	//ȥ��������
	this->setWindowFlags(Qt::FramelessWindowHint);

	//�˽�����ʾʱ������������
	this->setWindowModality(Qt::ApplicationModal);
}

ExitQueryUI::~ExitQueryUI()
{
	qDebug() << "~ExitQueryUI";
}


/***************** �¼��������û����� ****************/

//�û�С���̵� + -
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