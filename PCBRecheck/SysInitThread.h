#pragma once

#include <QThread>
#include "SerialNumberUI.h"
#include "Configurator.h"
#include "RuntimeLib.h"
#include <QDir>
#include <QMessageBox>


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
	void setRecheckConfig(pcb::UserConfig *ptr) { userConfig = ptr; }

protected:
	void run();

private:
	bool initUserConfig(); //��ȡ�����ļ�
	int initOutFolderHierarchy(); //��ʼ���ļ��в��
	void getOutputFolderInfo(QString dirpath);
	void getTypeFolderInfo(int typeNum, QString typeFolder);
	void NumList2StrList(const QList<int>& input, QStringList& output);

Q_SIGNALS:
	void sysInitFinished_initThread();
	void userConfigError_initThread();
	void outFolderHierarchyError_initThread();
};
