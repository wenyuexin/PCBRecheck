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


//��Ʒ��Ž���
class SerialNumberUI : public QWidget
{
	Q_OBJECT

public:
	//״̬��
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
	pcb::UserConfig *userConfig; //�û�����
	pcb::RuntimeParams *runtimeParams; //���в���
	pcb::FolderHierarchy *folderHierarchy; //���Ŀ¼�µ��ļ��в��
	StatusCode statusCode; //��Ʒ��Ž����״̬��

	bool focusOnSerialNumBox;
	CameraControler *cameraControler; //���������
	QImage *frame; //�����֡
	QString roiImagePath; 

	TessBaseAPI *ocrHandle; //OCRģ��
	cv::Mat roiImage; //������Ʒ��ŵ��ַ�ͼ

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
