#pragma once

#include <QThread>
#include <QDebug>
#include "Configurator.h"
#include "RuntimeParams.h"
#include "FileReceiver.h"


class FileSyncThread : public QThread
{
	Q_OBJECT

private:
	pcb::UserConfig *userConfig;
	pcb::RuntimeParams *runtimeParams;
	FileReceiver *fileReceiver;//文件接收器

public:
	FileSyncThread(QObject *parent = Q_NULLPTR);
	~FileSyncThread();

	inline void setUserConfig(pcb::UserConfig *ptr) { userConfig = ptr; }
	inline void setRuntimeParams(pcb::RuntimeParams *ptr) { runtimeParams = ptr; }

	void init();

protected:
	void run();
};