#pragma once

#include <QWidget>
#include "ui_SerialNumberUI.h"
#include "RuntimeLib.h"
#include "Configurator.h"
#include "CameraControler.h"
#include <allheaders.h>
#include <capi.h>
#include "opencv2/opencv.hpp"
#include <QString>
#include <QKeyEvent>
#include <QRegExpValidator>
#include <QLatin1Char>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopWidget>


//产品序号界面
class SerialNumberUI : public QWidget
{
	Q_OBJECT

public:
	//状态码
	enum StatusCode {
		NoError,
		Uncheck,
		OCR_InitFailed,
		OCR_RecognitionFailed,
		OCR_GetUTF8TextFailed,
		Camera_OpenCameraFailed,
		Camera_CaptureFailed,
		Default
	};

private:
	Ui::SerialNumberUI ui;
	QSize cameraLabelSize;

	QIntValidator intValidator;
	pcb::UserConfig *userConfig; //用户参数
	pcb::RuntimeParams *runtimeParams; //运行参数
	pcb::FolderHierarchy *folderHierarchy; //输出目录下的文件夹层次
	StatusCode statusCode; //产品序号界面的状态码

	bool focusOnSerialNumBox;
	CameraControler *cameraControler; //相机控制器
	QImage *frame; //相机的帧
	QString roiImagePath; 

	TessBaseAPI *ocrHandle; //OCR模块
	cv::Mat roiImage; //包含产品序号的字符图

public:
	SerialNumberUI(QWidget *parent = Q_NULLPTR, QRect &screenRect = QRect());
	~SerialNumberUI();

	void init();
	inline void setUserConfig(pcb::UserConfig *ptr) { userConfig = ptr; }
	inline void setRuntimeParams(pcb::RuntimeParams *ptr) { runtimeParams = ptr; }

	inline void setFolderHierarchy(pcb::FolderHierarchy *ptr) { folderHierarchy = ptr; }
	bool getNextSerialNum();
	void setActivated(bool activated);

private:
	void setExecutingMode();
	void getSerialNum();
	void exitOrParseSerialNum();
	void showMessageBox(pcb::MessageBoxType type, StatusCode code);

Q_SIGNALS:
	void exitRecheckSystem_numUI();
	void showRecheckMainUI_numUI();

private Q_SLOTS:
	void keyPressEvent(QKeyEvent *event);
	void do_refreshFrame_camera();
};
