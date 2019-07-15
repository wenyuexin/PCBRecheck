#pragma once

#include <QString>
#include <QDir>
#include <QFile>
#include <QTime>
#include <vector>
#include <QFileDialog>
#include <QEvent>
#include <QCoreApplication>
//#include "MyMessageBox.h"


namespace pcb
{
#ifndef RECHECK_TPYE_FOLDER_HIERARCHY
#define RECHECK_TPYE_FOLDER_HIERARCHY
	typedef QMap<int, QMap<int, QList<int>>> FolderHierarchy;
#endif	


#ifndef RECHECK_STRUCT
#define RECHECK_STRUCT
	struct FlawImageInfo {
		QString filePath;
		QString flawIndex;
		QString xPos;
		QString yPos;
		QString flawType;
	};
#endif	


#ifndef PCB_FUNCTIONS_CHINESE
#define PCB_FUNCTIONS_CHINESE
	inline QString chinese(const QByteArray &src) { return QString::fromLocal8Bit(src); }
#endif //PCB_FUNCTIONS_CHINESE

#ifndef PCB_FUNCTIONS
#define PCB_FUNCTIONS
	void delay(unsigned long msec);//�������ӳ�

	QString selectDirPath(QWidget *parent, QString windowTitle = "");//����ʽ�ļ���·��ѡ��
	void clearFiles(const QString &folderFullPath);
	void clearFolder(const QString &folderFullPath, bool included = false);
	void getFilePathList(const QString &folderFullPath, std::vector<std::string> &list);

	QString eraseNonDigitalCharInHeadAndTail(QString s); //ɾ���ַ�����β�ķ������ַ�
	QString boolVectorToString(const std::vector<bool> &vec);
	std::vector<bool> stringToBoolVector(const QString &str, int n = -1);

	bool isInetAddress(QString ip);
#endif //PCB_FUNCTIONS
}