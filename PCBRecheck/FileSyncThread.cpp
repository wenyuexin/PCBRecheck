#include "FileSyncThread.h"

FileSyncThread::FileSyncThread(QObject *parent)
	: QThread(parent)
{
	runtimeParams = Q_NULLPTR;
}

FileSyncThread::~FileSyncThread()
{
	qDebug() << "~FileSyncThread";
}


void FileSyncThread::run()
{
	//��ѯ�Ƿ��յ����µļ����
	fileReceiver.startListen();

	//��������͵����������豸
	// ...
}