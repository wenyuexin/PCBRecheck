#pragma once

#include <QWidget>
#include "ui_SerialNumberUI.h"
#include <QString>
#include <QKeyEvent>
#include <QRegExpValidator>
#include <QLatin1Char>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
#ifndef FOLDER_HIERARCHY_TPYE
#define FOLDER_HIERARCHY_TPYE
	typedef QMap<int, QMap<int, QList<int>>> FolderHierarchy;
#endif	
}


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
	Ui::FolderHierarchy *folderHierarchy; //���Ŀ¼�µ��ļ��в��
	const int serialNumSlice[4] = { 8, 2, 2, 4 }; //������

public:
	SerialNumberUI(QWidget *parent = Q_NULLPTR);
	~SerialNumberUI();

	void setSerialNum(QString **ptrArray);
	inline void setFolderHierarchy(Ui::FolderHierarchy *ptr) { folderHierarchy = ptr; }
	bool getNextSerialNum();

private:
	bool parseSerialNum();

Q_SIGNALS:
	void exitRecheckSystem_numUI();
	void showRecheckUI_numUI();

private Q_SLOTS:
	void keyPressEvent(QKeyEvent *event);
};
