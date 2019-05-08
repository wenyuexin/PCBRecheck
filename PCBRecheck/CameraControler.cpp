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

//��ʼ��
void CameraControler::init()
{
	//����ROI�����λ�úʹ�С
	int x_tl = (resolution.width - roiRect.width) / 2;
	int y_tl = (resolution.height - roiRect.height) / 2;
	roiRect.x = x_tl;
	roiRect.y = y_tl;
	roiRect &= Rect(Point(0, 0), resolution); //��ֹ�����߽�

	//����������α߿��ROI�����λ�úʹ�С
	roiBoxRect = cv::Rect(Point(x_tl - roiLineWidth, y_tl - roiLineWidth),
		roiRect.size() + Size(2 * roiLineWidth, 2 * roiLineWidth));
	roiBoxRect &= Rect(Point(0, 0), resolution); //��ֹ�����߽�

	qFrame = new QImage(); //�����ڽ�������ʾ��֡
	timer = new QTimer(this); //���ڿ�������Ƶ�ʵļ�ʱ��
	connect(timer, SIGNAL(timeout()), this, SLOT(readFrame()));
}


//�򿪲��������
void CameraControler::openCamera(bool doCapture)
{
	camera = VideoCapture(0);//��
	if (!camera.isOpened()) { //��ʧ��
		emit openCameraFailed_camera(); return;
	}

	//��������ֱ���
	camera.set(cv::CAP_PROP_FRAME_WIDTH, resolution.width);
	camera.set(cv::CAP_PROP_FRAME_HEIGHT, resolution.height);

	//��������ܲ��ܲɼ���ͼ��
	//����������ζ����ܲɼ���ͼ������Ϊ��ʼ��ʧ��
	if (!camera.grab()) {
		if (!camera.grab()) {
			emit openCameraFailed_camera(); return;
		}
	}

	//������ʱ����ÿ�μ�ʱ�����󣬵��������ȡ�µ�֡
	if (doCapture) timer->start(120); 
}


//��ȡ֡
void CameraControler::readFrame()
{
	Mat cvFrame;
	camera >> cvFrame; //�������ץȡһ֡
	if (cvFrame.empty()) camera >> cvFrame; //���Ϊ������ץһ֡
	if (cvFrame.empty()) return; //�����Ϊ���򷵻�

	cv::rectangle(cvFrame, roiBoxRect, cv::Scalar(0, 0, 255), roiLineWidth, cv::LINE_8, 0);//��֡ͼ���м�λ�û��ƾ��ο�
	QImage image = QImage((const uchar*)cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);//ת��
	*qFrame = image.rgbSwapped();
	emit refreshFrame_camera();//֪ͨ��Ʒ��Ž������֡
}


//��ȡ�ؼ�֡
void CameraControler::takePicture()
{
	stopCapture(); //��ͣ��ȡ�µ�֡

	Mat cvFrame;
	camera >> cvFrame; //�������ץȡһ֡
	int cnt = 0; //��ȡ����
	while (cvFrame.empty()) { //�����ȻΪ�������
		cnt += 1; //���¼�����
		camera >> cvFrame; //������ȡ
		if (cvFrame.empty() && cnt > 5) { //ʧ�ܴ���������ֵ��ֱ�ӷ���
			emit captureFailed_camera(); return; 
		}
	}

	cv::rectangle(cvFrame, roiBoxRect, cv::Scalar(0, 0, 255), roiLineWidth, cv::LINE_8, 0);//��֡ͼ���м�λ�û��ƾ��ο�
	QImage image = QImage((const uchar*)cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);//ת��
	*qFrame = image.rgbSwapped();
	emit refreshFrame_camera();//֪ͨ��Ʒ��Ž������֡

	Mat roiImage = cvFrame(roiRect); //��ȡ�ؼ�֡��ROI����
	cv::imwrite(roiImagePath->toStdString(), roiImage); //����ROI�����ͼ��
}


//ֹͣ��ȡ�µ�֡
void CameraControler::startCapture()
{
	if (camera.isOpened())
		timer->start();
}

//ֹͣ��ȡ�µ�֡
void CameraControler::stopCapture()
{
	timer->stop();
}


//�ر�����ͷ���ͷ���Դ
void CameraControler::closeCamera()
{
	timer->stop();
	camera.release();
}
