#include "FileSyncThread.h"

FileSyncThread::FileSyncThread(QObject *parent)
	: QThread(parent)
{
}

FileSyncThread::~FileSyncThread()
{
}


void FileSyncThread::run()
{
	while (true) {
		//pcb::delay(1000);

		//查询是否收到了新的检测结果

		//如果有则发送到其他复查设备

	}
}