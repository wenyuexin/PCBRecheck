#include "FileSyncThread.h"

using std::string;

FileSyncThread::FileSyncThread(QObject *parent)
	: QThread(parent)
{
	userConfig = Q_NULLPTR;
	runtimeParams = Q_NULLPTR;
	fileReceiver = Q_NULLPTR;
}

FileSyncThread::~FileSyncThread()
{
	qDebug() << "~FileSyncThread";
	delete fileReceiver;
	fileReceiver = Q_NULLPTR;
}


void FileSyncThread::init()
{
	short port = 8888;
	string workDirectory = userConfig->OutputDirPath.toStdString();
	fileReceiver = new FileReceiver(port, workDirectory);
}


void FileSyncThread::run()
{
	//��ѯ�Ƿ��յ����µļ����
	fileReceiver->startListen();

	//���ܵ����ļ�ת�������������豸
	// ...
}