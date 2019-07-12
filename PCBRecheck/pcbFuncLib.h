#pragma once

#include <QString>
#include <QDir>
#include <QFile>
#include <QTime>
#include <vector>


namespace pcb
{
#ifndef PCB_FUNCTIONS
#define PCB_FUNCTIONS
	void delay(unsigned long msec);//非阻塞延迟

	QString selectDirPath(QWidget *parent, QString windowTitle = "");//交互式文件夹路径选择
	void clearFiles(const QString &folderFullPath);
	void clearFolder(const QString &folderFullPath, bool included = false);
	void getFilePathList(const QString &folderFullPath, std::vector<std::string> &list);

	QString eraseNonDigitalCharInHeadAndTail(QString s); //删除字符串首尾的非数字字符
	QString boolVectorToString(const std::vector<bool> &vec);
	std::vector<bool> stringToBoolVector(const QString &str, int n = -1);

	bool isInetAddress(QString ip);
#endif //PCB_FUNCTIONS
}