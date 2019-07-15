#pragma once

#include <QThread>
#include <QDir>
#include "Configurator.h"


class SysInitThread :
	public QThread
{
	Q_OBJECT

private:
	//QWidget *RecheckMainWidget;
	pcb::FolderHierarchy *OutputFolderHierarchy; //输出目录下的文件夹层次
	pcb::UserConfig *userConfig; //检修系统的配置信息
	
public:
	SysInitThread();
	~SysInitThread();

	void setOutFolderHierarchy(pcb::FolderHierarchy *ptr) { OutputFolderHierarchy = ptr; }
	void setUserConfig(pcb::UserConfig *ptr) { userConfig = ptr; }

protected:
	void run();

private:
	bool initUserConfig(); //读取配置文件
	bool initOutputFolderHierarchy(); //初始化文件夹层次
	bool getOutputFolderInfo(QString dirpath);
	void getModelFolderInfo(int modelIndex, QString &modelFolderPath);
	void getBatchFolderInfo(int modelIndex, int batchIndex, QString &batchFolderPath);
	void NumList2StrList(const QList<int>& input, QStringList& output);

Q_SIGNALS:
	void sysInitFinished_initThread();
	void userConfigError_initThread();
	void outFolderHierarchyError_initThread();
};
