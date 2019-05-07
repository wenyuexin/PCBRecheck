#pragma once

#include "opencv2/opencv.hpp"
#include <QObject>
#include <QImage> 
#include <QPixmap>
#include <QTimer>
#include <QDebug>


//相机控制器
class CameraControler : public QObject
{
	Q_OBJECT

private:
	QTimer *timer;
	cv::VideoCapture camera;
	QImage *qFrame; //相机的帧
	QString *roiImagePath; //roi图像的路径
	int roiWidth = 300; //截图区域的宽
	int roiHeight = 120; //截图区域的高

public:
	CameraControler(QObject *parent = Q_NULLPTR);
	~CameraControler();

	inline void setRoiImagePath(QString *path) { roiImagePath = path; }
	inline void setRoiSize(int w, int h) { roiWidth = w; roiHeight = h; }
	inline QImage* getFrame() { return qFrame; }

	void openCamera(bool doCapture = true); //打开摄像头 
	void takePicture(); //拍照 
	void stopCapture(); //停止获取
	void startCapture(); //重新开始获取
	void closeCamera(); //关闭摄像头 

Q_SIGNALS:
	void openCameraFailed_camera();
	void refreshFrame_camera();
	void captureFailed_camera();

private Q_SLOTS:
	void readFrame(); // 读取当前帧信息 

};
