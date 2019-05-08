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
	cv::Size resolution; //相机分辨率，默认640*480
	QImage *qFrame; //相机的帧
	QString *roiImagePath; //ROI图像的路径

	const int roiLineWidth = 2; //绘制ROI区域的矩形框的边界线宽
	int roiWidth; //截图区域的宽
	int roiHeight; //截图区域的高
	cv::Rect roiRect; //ROI区域的位置和大小
	cv::Rect roiBoxRect; //包含矩形边框的ROI区域的位置和大小

public:
	CameraControler(QObject *parent = Q_NULLPTR);
	~CameraControler();

	inline void setRoiImagePath(QString *path) { roiImagePath = path; }
	inline void setRoiSize(int w, int h) { roiRect.width = w; roiRect.height = h; }
	inline void setResolution(int w, int h) { resolution.width = w; resolution.height = h; }
	void init();

	inline QImage* getFrame() { return qFrame; }
	inline bool isCameraOpened() { return camera.isOpened(); }

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
