#pragma once

#include "opencv2/opencv.hpp"
#include <QObject>
#include <QImage> 
#include <QPixmap>
#include <QTimer>
#include <QDebug>


//���������
class CameraControler : public QObject
{
	Q_OBJECT

private:
	QTimer *timer;
	cv::VideoCapture camera;
	cv::Size resolution; //����ֱ��ʣ�Ĭ��640*480
	QImage *qFrame; //�����֡
	QString *roiImagePath; //ROIͼ���·��

	const int roiLineWidth = 2; //����ROI����ľ��ο�ı߽��߿�
	int roiWidth; //��ͼ����Ŀ�
	int roiHeight; //��ͼ����ĸ�
	cv::Rect roiRect; //ROI�����λ�úʹ�С
	cv::Rect roiBoxRect; //�������α߿��ROI�����λ�úʹ�С

public:
	CameraControler(QObject *parent = Q_NULLPTR);
	~CameraControler();

	inline void setRoiImagePath(QString *path) { roiImagePath = path; }
	inline void setRoiSize(int w, int h) { roiRect.width = w; roiRect.height = h; }
	inline void setResolution(int w, int h) { resolution.width = w; resolution.height = h; }
	void init();

	inline QImage* getFrame() { return qFrame; }
	inline bool isCameraOpened() { return camera.isOpened(); }

	void openCamera(bool doCapture = true); //������ͷ 
	void takePicture(); //���� 
	void stopCapture(); //ֹͣ��ȡ
	void startCapture(); //���¿�ʼ��ȡ
	void closeCamera(); //�ر�����ͷ 

Q_SIGNALS:
	void openCameraFailed_camera();
	void refreshFrame_camera();
	void captureFailed_camera();

private Q_SLOTS:
	void readFrame(); // ��ȡ��ǰ֡��Ϣ 

};
