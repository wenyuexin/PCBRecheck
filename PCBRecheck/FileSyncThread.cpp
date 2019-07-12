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
	//查询是否收到了新的检测结果
	fileReceiver.startListen();

	//如果有则发送到其他复查设备
	// ...
}