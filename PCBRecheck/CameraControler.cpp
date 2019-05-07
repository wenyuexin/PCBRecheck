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

//�����ʼ��
void CameraControler::openCamera(bool doCapture)
{
	camera = VideoCapture(0);//������ͷ
	if (!camera.isOpened()) { //��ʧ��
		emit openCameraFailed_camera(); return;
	}
	if (doCapture) timer->start(120); //��ʼ��ʱ���������ȡ�µ�֡
}


//��ȡ֡
void CameraControler::readFrame()
{
	cv::Mat cvFrame;
	camera >> cvFrame; //�������ץȡһ֡
	if (cvFrame.empty()) camera >> cvFrame; //���Ϊ������ץһ֡
	if (cvFrame.empty()) return; //�����Ϊ���򷵻�

	int x_tl = (cvFrame.cols - roiWidth - 4) / 2;
	int y_tl = (cvFrame.rows - roiHeight - 4) / 2;
	cv::Rect rect(x_tl, y_tl, roiWidth, roiHeight);//ROI����
	cv::rectangle(cvFrame, rect, cv::Scalar(0, 0, 255), 2, cv::LINE_8, 0);//��֡ͼ���м�λ�û��ƾ��ο�
	QImage image = QImage((const uchar*)cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);//ת��

	*qFrame = image.rgbSwapped();
	emit refreshFrame_camera();//֪ͨ��Ʒ��Ž������֡
}


//����
void CameraControler::takePicture()
{
	stopCapture(); //��ͣ��ȡ�µ�֡

	cv::Mat cvFrame;
	camera >> cvFrame;//������ͷ��ץȡ������ÿһ֡ 
	camera >> cvFrame; //�������ץȡһ֡
	int cnt = 0; //��ȡ����
	while (cvFrame.empty()) { ////�����ȻΪ��
		cnt += 1; //���¼�����
		camera >> cvFrame; //������ȡ֡
		if (cvFrame.empty() && cnt > 5) { //ʧ�ܴ���������ֵ��ֱ�ӷ���
			emit captureFailed_camera(); return; 
		}
	}

	int x_tl = (cvFrame.cols - roiWidth - 4) / 2;
	int y_tl = (cvFrame.rows - roiHeight - 4) / 2;
	cv::Rect rect(x_tl, y_tl, roiWidth, roiHeight);//ROI����
	cv::rectangle(cvFrame, rect, cv::Scalar(0, 0, 255), 2, cv::LINE_8, 0);//��֡ͼ���м�λ�û��ƾ��ο�
	QImage image = QImage((const uchar*)cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);//ת��
	*qFrame = image.rgbSwapped();
	emit refreshFrame_camera();//֪ͨ��Ʒ��Ž������֡

	Mat roiImage = cvFrame(rect); //��ȡ�ؼ�֡
	cv::imwrite(roiImagePath->toStdString(), roiImage); //����ROI�����ͼ��
}


//ֹͣ��ȡ�µ�֡
void CameraControler::startCapture()
{
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
