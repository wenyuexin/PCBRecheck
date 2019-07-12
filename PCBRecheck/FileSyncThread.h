#pragma once

#include <QThread>

class FileSyncThread : public QThread
{
	Q_OBJECT

public:
	FileSyncThread(QObject *parent);
	~FileSyncThread();

protected:
	void run();
};
