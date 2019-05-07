#include "CameraControler.h"

using cv::VideoCapture;
using cv::Mat;


CameraControler::CameraControler(QObject *parent)
	: QObject(parent)
{
	qFrame = new QImage();
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(readFrame()));
}

CameraControler::~CameraControler()
{
	qDebug() << "~CameraControler";
	closeCamera();
	delete qFrame;
	qFrame = Q_NULLPTR;
}

//相机初始化
void CameraControler::openCamera(bool doCapture)
{
	camera = VideoCapture(0);//打开摄像头
	if (!camera.isOpened()) { //打开失败
		emit openCameraFailed_camera(); return;
	}
	if (doCapture) timer->start(120); //开始计时，结束后获取新的帧
}


//读取帧
void CameraControler::readFrame()
{
	cv::Mat cvFrame;
	camera >> cvFrame; //从相机中抓取一帧
	if (cvFrame.empty()) camera >> cvFrame; //如果为空则在抓一帧
	if (cvFrame.empty()) return; //如果仍为空则返回

	int x_tl = (cvFrame.cols - roiWidth - 4) / 2;
	int y_tl = (cvFrame.rows - roiHeight - 4) / 2;
	cv::Rect rect(x_tl, y_tl, roiWidth, roiHeight);//ROI区域
	cv::rectangle(cvFrame, rect, cv::Scalar(0, 0, 255), 2, cv::LINE_8, 0);//在帧图像中间位置绘制矩形框
	QImage image = QImage((const uchar*)cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);//转换

	*qFrame = image.rgbSwapped();
	emit refreshFrame_camera();//通知产品序号界面更新帧
}


//拍照
void CameraControler::takePicture()
{
	stopCapture(); //暂停获取新的帧

	cv::Mat cvFrame;
	camera >> cvFrame;//从摄像头中抓取并返回每一帧 
	camera >> cvFrame; //从相机中抓取一帧
	int cnt = 0; //获取次数
	while (cvFrame.empty()) { ////如果仍然为空
		cnt += 1; //更新计数器
		camera >> cvFrame; //继续获取帧
		if (cvFrame.empty() && cnt > 5) { //失败次数到达阈值后，直接返回
			emit captureFailed_camera(); return; 
		}
	}

	int x_tl = (cvFrame.cols - roiWidth - 4) / 2;
	int y_tl = (cvFrame.rows - roiHeight - 4) / 2;
	cv::Rect rect(x_tl, y_tl, roiWidth, roiHeight);//ROI区域
	cv::rectangle(cvFrame, rect, cv::Scalar(0, 0, 255), 2, cv::LINE_8, 0);//在帧图像中间位置绘制矩形框
	QImage image = QImage((const uchar*)cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);//转换
	*qFrame = image.rgbSwapped();
	emit refreshFrame_camera();//通知产品序号界面更新帧

	Mat roiImage = cvFrame(rect); //截取关键帧
	cv::imwrite(roiImagePath->toStdString(), roiImage); //保存ROI区域的图像
}


//停止获取新的帧
void CameraControler::startCapture()
{
	timer->start();
}

//停止获取新的帧
void CameraControler::stopCapture()
{
	timer->stop();
}


//关闭摄像头，释放资源
void CameraControler::closeCamera()
{
	timer->stop();
	camera.release();
}
