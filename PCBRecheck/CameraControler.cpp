#include "CameraControler.h"

using cv::VideoCapture;
using cv::Mat;
using cv::Rect;
using cv::Size;
using cv::Point;


CameraControler::CameraControler(QObject *parent)
	: QObject(parent)
{
}

CameraControler::~CameraControler()
{
	qDebug() << "~CameraControler";
	closeCamera();
	delete qFrame;
	qFrame = Q_NULLPTR;
}

//初始化
void CameraControler::init()
{
	//计算ROI区域的位置和大小
	int x_tl = (resolution.width - roiRect.width) / 2;
	int y_tl = (resolution.height - roiRect.height) / 2;
	roiRect.x = x_tl;
	roiRect.y = y_tl;
	roiRect &= Rect(Point(0, 0), resolution); //防止超出边界

	//计算包含矩形边框的ROI区域的位置和大小
	roiBoxRect = cv::Rect(Point(x_tl - roiLineWidth, y_tl - roiLineWidth),
		roiRect.size() + Size(2 * roiLineWidth, 2 * roiLineWidth));
	roiBoxRect &= Rect(Point(0, 0), resolution); //防止超出边界

	qFrame = new QImage(); //用于在界面上显示的帧
	timer = new QTimer(this); //用于控制拍照频率的计时器
	connect(timer, SIGNAL(timeout()), this, SLOT(readFrame()));
}


//打开并设置相机
void CameraControler::openCamera(bool doCapture)
{
	camera = VideoCapture(0);//打开
	if (!camera.isOpened()) { //打开失败
		emit openCameraFailed_camera(); return;
	}

	//设置相机分辨率
	camera.set(cv::CAP_PROP_FRAME_WIDTH, resolution.width);
	camera.set(cv::CAP_PROP_FRAME_HEIGHT, resolution.height);

	//测试相机能不能采集到图像
	//如果连续两次都不能采集到图像则认为初始化失败
	if (!camera.grab()) {
		if (!camera.grab()) {
			emit openCameraFailed_camera(); return;
		}
	}

	//启动计时器，每次计时结束后，调用相机获取新的帧
	if (doCapture) timer->start(120); 
}


//读取帧
void CameraControler::readFrame()
{
	Mat cvFrame;
	camera >> cvFrame; //从相机中抓取一帧
	if (cvFrame.empty()) camera >> cvFrame; //如果为空则再抓一帧
	if (cvFrame.empty()) return; //如果仍为空则返回

	cv::rectangle(cvFrame, roiBoxRect, cv::Scalar(0, 0, 255), roiLineWidth, cv::LINE_8, 0);//在帧图像中间位置绘制矩形框
	QImage image = QImage((const uchar*)cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);//转换
	*qFrame = image.rgbSwapped();
	emit refreshFrame_camera();//通知产品序号界面更新帧
}


//截取关键帧
void CameraControler::takePicture()
{
	stopCapture(); //暂停获取新的帧

	Mat cvFrame;
	camera >> cvFrame; //从相机中抓取一帧
	int cnt = 0; //获取次数
	while (cvFrame.empty()) { //如果仍然为空则继续
		cnt += 1; //更新计数器
		camera >> cvFrame; //继续截取
		if (cvFrame.empty() && cnt > 5) { //失败次数到达阈值后，直接返回
			emit captureFailed_camera(); return; 
		}
	}

	cv::rectangle(cvFrame, roiBoxRect, cv::Scalar(0, 0, 255), roiLineWidth, cv::LINE_8, 0);//在帧图像中间位置绘制矩形框
	QImage image = QImage((const uchar*)cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);//转换
	*qFrame = image.rgbSwapped();
	emit refreshFrame_camera();//通知产品序号界面更新帧

	Mat roiImage = cvFrame(roiRect); //截取关键帧的ROI区域
	cv::imwrite(roiImagePath->toStdString(), roiImage); //保存ROI区域的图像
}


//停止获取新的帧
void CameraControler::startCapture()
{
	if (camera.isOpened())
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
