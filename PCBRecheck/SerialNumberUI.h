#pragma once

#include <QWidget>
#include "ui_SerialNumberUI.h"
#include "RuntimeLib.h"
#include "Configurator.h"
#include <QString>
#include <QKeyEvent>
#include <QRegExpValidator>
#include <QLatin1Char>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopWidget>


class SerialNumberUI : public QWidget
{
	Q_OBJECT

private:
	Ui::SerialNumberUI ui;
	QIntValidator intValidator;
	pcb::UserConfig *userConfig; //用户参数
	pcb::RuntimeParams *runtimeParams; //运行参数
	pcb::FolderHierarchy *folderHierarchy; //输出目录下的文件夹层次

public:
	SerialNumberUI(QWidget *parent = Q_NULLPTR, QRect &screenRect = QRect());
	~SerialNumberUI();

	inline void setAdminConfig(pcb::UserConfig *ptr) { userConfig = ptr; }
	inline void setRuntimeParams(pcb::RuntimeParams *ptr) { runtimeParams = ptr; }

	inline void setFolderHierarchy(pcb::FolderHierarchy *ptr) { folderHierarchy = ptr; }
	bool getNextSerialNum();

private:
	void recognize();



Q_SIGNALS:
	void exitRecheckSystem_numUI();
	void showRecheckMainUI_numUI();

private Q_SLOTS:
	void keyPressEvent(QKeyEvent *event);
};
