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
	QImage *qFrame; //�����֡
	QString *roiImagePath; //roiͼ���·��
	int roiWidth = 300; //��ͼ����Ŀ�
	int roiHeight = 120; //��ͼ����ĸ�

public:
	CameraControler(QObject *parent = Q_NULLPTR);
	~CameraControler();

	inline void setRoiImagePath(QString *path) { roiImagePath = path; }
	inline void setRoiSize(int w, int h) { roiWidth = w; roiHeight = h; }
	inline QImage* getFrame() { return qFrame; }

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
