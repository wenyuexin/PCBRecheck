#include "FileSyncThread.h"

FileSyncThread::FileSyncThread(QObject *parent)
	: QThread(parent)
{
	runtimeParams = Q_NULLPTR;

	short t_port = 8888;
	std::string t_workDirectory = "D:\\PCBData\\output";
	fileReceiver = new FileReceiver(t_port, t_workDirectory);
}

FileSyncThread::~FileSyncThread()
{
	qDebug() << "~FileSyncThread";
	delete fileReceiver;
	fileReceiver = Q_NULLPTR;
}


void FileSyncThread::run()
{
	//��ѯ�Ƿ��յ����µļ����
	fileReceiver->startListen();

	//��������͵����������豸
	// ...
}