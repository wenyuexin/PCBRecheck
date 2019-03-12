#pragma once

#include <QWidget>
#include "ui_SerialNumberUI.h"
#include <QString>
#include <QKeyEvent>
#include <QRegExpValidator>
#include <QLatin1Char>

#ifndef FOLDER_HIERARCHY_TPYE
#define FOLDER_HIERARCHY_TPYE
typedef QMap<int, QMap<int, QList<int>>> FolderHierarchy;
#endif

class SerialNumberUI : public QWidget
{
	Q_OBJECT

private:
	Ui::SerialNumberUI ui;
	QIntValidator intValidator;
	QString *serialNum; //产品序号
	QString *sampleModelNum; //型号
	QString *sampleBatchNum; //批次号
	QString *sampleNum; //样本编号 QString
	FolderHierarchy *folderHierarchy; //输出目录下的文件夹层次
	int sampleTypeNumSlice[2] = { 0,2 }; //型号的字符串切片范围
	int sampleBatchNumSlice[2] = { 2, 2 };  //批次号的字符串切片范围
	int sampleNumSlice[2] = { 4, 4 }; //样本编号的字符串切片范围

public:
	SerialNumberUI(QWidget *parent = Q_NULLPTR);
	~SerialNumberUI();

	void setSerialNumPtr(QString **ptrArray);
	void setFolderHierarchyPtr(FolderHierarchy *ptr);
	bool getNextSerialNum();

private:
	void parseSerialNum();

Q_SIGNALS:
	void exitRecheckSystem_numUI();
	void showRecheckUI_numUI();

private Q_SLOTS:
	void keyPressEvent(QKeyEvent *event);
};
