#include "MyMessageBox.h"

MyMessageBox::MyMessageBox()
{
	ui.setupUi(this);

	//判断并选择在主屏或者是副屏上显示
	QDesktopWidget* desktop = QApplication::desktop();
	QRect screenRect = desktop->screenGeometry(1);//副屏区域
	if (screenRect.width() < 1440 || screenRect.height() < 900) {
		screenRect = desktop->screenGeometry(0);//主屏区域
	}

	//判断并选择在主屏或者是副屏上显示
	QPoint widgetPos = screenRect.center();
	widgetPos -= QPoint(this->width() / 2.0, this->height() / 2.0);
	QRect widgetRect = QRect(widgetPos, this->size());
	this->setGeometry(widgetRect);

	//此界面显示时禁用其他窗口
	this->setWindowModality(Qt::ApplicationModal);

	//去掉最大化、最小化按钮，保留关闭按钮 
	this->setWindowFlags(Qt::WindowCloseButtonHint);
}

MyMessageBox::~MyMessageBox()
{
	qDebug() << "~MyMessageBox";
}


void MyMessageBox::set(pcb::MessageBoxType type, QString msg, QString title)
{
	//窗口标题
	if (title == "") {
		switch (type)
		{
		case pcb::Warning:
			boxTitle = pcb::chinese("警告");
			break;
		case pcb::Information:
			boxTitle = pcb::chinese("提示");
			break;
		case pcb::Question:
			boxTitle = pcb::chinese("询问");
			break;
		case pcb::Critical:
			boxTitle = pcb::chinese("错误");
			break;
		case pcb::About:
			boxTitle = pcb::chinese("关于");
			break;
		}
	}
	else {
		boxTitle = title;
	}
	this->setWindowTitle(boxTitle);

	//需要显示的信息
	message = msg;
}


/************* 窗口显示 **************/

void MyMessageBox::doShow()
{
	//设置要显示的信息
	ui.label_message->setText(message);

	this->show();
	qApp->processEvents();

	running = true;
	while (running) pcb::delay(100);
}


/************* 键盘事件 **************/

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


/************ 按键响应 *************/

//确定
void MyMessageBox::on_pushButton_clicked()
{
	running = false;
	this->close();
}