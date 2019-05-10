#include "MyMessageBox.h"

MyMessageBox::MyMessageBox()
{
	ui.setupUi(this);

	//�жϲ�ѡ�������������Ǹ�������ʾ
	QDesktopWidget* desktop = QApplication::desktop();
	QRect screenRect = desktop->screenGeometry(1);//��������
	if (screenRect.width() < 1440 || screenRect.height() < 900) {
		screenRect = desktop->screenGeometry(0);//��������
	}

	//�жϲ�ѡ�������������Ǹ�������ʾ
	QPoint widgetPos = screenRect.center();
	widgetPos -= QPoint(this->width() / 2.0, this->height() / 2.0);
	QRect widgetRect = QRect(widgetPos, this->size());
	this->setGeometry(widgetRect);

	//�˽�����ʾʱ������������
	this->setWindowModality(Qt::ApplicationModal);

	//ȥ����󻯡���С����ť�������رհ�ť 
	this->setWindowFlags(Qt::WindowCloseButtonHint);
}

MyMessageBox::~MyMessageBox()
{
	qDebug() << "~MyMessageBox";
}


void MyMessageBox::set(pcb::MessageBoxType type, QString msg, QString title)
{
	//���ڱ���
	if (title == "") {
		switch (type)
		{
		case pcb::Warning:
			boxTitle = pcb::chinese("����");
			break;
		case pcb::Information:
			boxTitle = pcb::chinese("��ʾ");
			break;
		case pcb::Question:
			boxTitle = pcb::chinese("ѯ��");
			break;
		case pcb::Critical:
			boxTitle = pcb::chinese("����");
			break;
		case pcb::About:
			boxTitle = pcb::chinese("����");
			break;
		}
	}
	else {
		boxTitle = title;
	}
	this->setWindowTitle(boxTitle);

	//��Ҫ��ʾ����Ϣ
	message = msg;
}


/************* ������ʾ **************/

void MyMessageBox::doShow()
{
	//����Ҫ��ʾ����Ϣ
	ui.label_message->setText(message);

	this->show();
	qApp->processEvents();

	running = true;
	while (running) pcb::delay(100);
}


/************* �����¼� **************/

void MyMessageBox::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Enter:
	case Qt::Key_Return:
		qDebug() << "Key_Enter";
		on_pushButton_clicked();
		break;
	}
}


/************ ������Ӧ *************/

//ȷ��
void MyMessageBox::on_pushButton_clicked()
{
	running = false;
	this->close();
}