#include "ExitQueryUI.h"

ExitQueryUI::ExitQueryUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

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
}

/***************** �¼��������û����� ****************/

//�û�С���̵� + -
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