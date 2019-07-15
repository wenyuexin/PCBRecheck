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
	FileReceiver *fileReceiver;

public:
	FileSyncThread(QObject *parent = Q_NULLPTR);
	~FileSyncThread();

	inline void setRuntimeParams(pcb::RuntimeParams *ptr) { runtimeParams = ptr; }
	inline void setUserConfig(pcb::UserConfig *ptr) { userConfig = ptr; }

protected:
	void run();
};