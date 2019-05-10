#pragma once

#include <QThread>
#include "SerialNumberUI.h"
#include "Configurator.h"
#include "RuntimeLib.h"
#include "MyMessageBox.h"
#include <QDir>


class SysInitThread :
	public QThread
{
	Q_OBJECT

private:
	QWidget *RecheckMainWidget;
	pcb::FolderHierarchy *OutputFolderHierarchy; //���Ŀ¼�µ��ļ��в��
	pcb::UserConfig *userConfig; //����ϵͳ��������Ϣ
	
public:
	SysInitThread();
	~SysInitThread();

	void setOutFolderHierarchy(pcb::FolderHierarchy *ptr) { OutputFolderHierarchy = ptr; }
	void setUserConfig(pcb::UserConfig *ptr) { userConfig = ptr; }

protected:
	void run();

private:
	bool initUserConfig(); //��ȡ�����ļ�
	bool initOutputFolderHierarchy(); //��ʼ���ļ��в��
	bool getOutputFolderInfo(QString dirpath);
	void getModelFolderInfo(int modelIndex, QString &modelFolderPath);
	void getBatchFolderInfo(int modelIndex, int batchIndex, QString &batchFolderPath);
	void NumList2StrList(const QList<int>& input, QStringList& output);

Q_SIGNALS:
	void sysInitFinished_initThread();
	void userConfigError_initThread();
	void outFolderHierarchyError_initThread();
};
