#pragma once

#include <QThread>
#include "SerialNumberUI.h"
#include "Configurator.h"
#include <QDir>
#include <QMessageBox>

#ifndef FOLDER_HIERARCHY_TPYE
#define FOLDER_HIERARCHY_TPYE
typedef QMap<int, QMap<int, QList<int>>> FolderHierarchy;
#endif

class SysInitThread :
	public QThread
{
	Q_OBJECT

private:
	QWidget *RecheckMainWidget;
	FolderHierarchy *OutputFolderHierarchy; //���Ŀ¼�µ��ļ��в��
	Ui::RecheckConfig *recheckConfig; //����ϵͳ��������Ϣ
	
public:
	SysInitThread();
	~SysInitThread();

	void setOutFolderHierarchyPtr(FolderHierarchy *ptr);
	void setRecheckConfigPtr(Ui::RecheckConfig * ptr);

protected:
	void run();

private:
	int initRecheckConfig(); //��ȡ�����ļ�
	int initOutFolderHierarchy(); //��ʼ���ļ��в��
	void getOutputFolderInfo(QString dirpath);
	void getTypeFolderInfo(int typeNum, QString typeFolder);
	void NumList2StrList(const QList<int>& input, QStringList& output);

Q_SIGNALS:
	void initializeFinished_initThread();
	void configError1_initThread();
	void configError2_initThread();
	void outFolderHierarchyError_initThread();
};
