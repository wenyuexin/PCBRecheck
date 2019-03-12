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
	QString *serialNum; //��Ʒ���
	QString *sampleModelNum; //�ͺ�
	QString *sampleBatchNum; //���κ�
	QString *sampleNum; //������� QString
	FolderHierarchy *folderHierarchy; //���Ŀ¼�µ��ļ��в��
	int sampleTypeNumSlice[2] = { 0,2 }; //�ͺŵ��ַ�����Ƭ��Χ
	int sampleBatchNumSlice[2] = { 2, 2 };  //���κŵ��ַ�����Ƭ��Χ
	int sampleNumSlice[2] = { 4, 4 }; //������ŵ��ַ�����Ƭ��Χ

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
