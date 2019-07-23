#include "CameraControler.h"

using cv::VideoCapture;
using cv::Mat;
using cv::Rect;
using cv::Size;
using cv::Point;


CameraControler::CameraControler(QObject *parent)
	: QObject(parent)
{
	qFrame = Q_NULLPTR;
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
	qFrame = new QImage(); //�����ڽ�������ʾ��֡
	timer = new QTimer(this); //���ڿ�������Ƶ�ʵļ�ʱ��
	connect(timer, SIGNAL(timeout()), this, SLOT(readFrame()));
}


/****************** ����ر� *******************/

//�򿪲��������
void CameraControler::openCamera(bool doCapture)
{
	camera = VideoCapture(0);//��
	if (!camera.isOpened()) { //��ʧ��
		emit openCameraFinished_camera(false); return;
	}

	//��������ֱ���
	//�ͷֱ����£�Ŀ�������ͼ����ܲ��������Ұ����
	//����ͨ��������߷ֱ��ʣ�Ȼ�������ô���ü��ķ�ʽ����
	bool success = true;
	success &= camera.set(cv::CAP_PROP_FRAME_WIDTH, resolution.width);
	success &= camera.set(cv::CAP_PROP_FRAME_HEIGHT, resolution.height);
	resolution.width = camera.get(cv::CAP_PROP_FRAME_WIDTH);
	resolution.height = camera.get(cv::CAP_PROP_FRAME_HEIGHT);
	calcRoiRect(resolution); //����roi����Ĵ�С

	//��������ܲ��ܲɼ���ͼ��
	//����������ζ����ܲɼ���ͼ������Ϊ��ʼ��ʧ��
	if (!camera.grab()) {
		if (!camera.grab()) {
			emit openCameraFinished_camera(false); return;
		}
	}

	//������ʱ����ÿ�μ�ʱ�����󣬵��������ȡ�µ�֡
	if (doCapture) timer->start(120); 
	//֪ͨ���ʶ����棬����Ѿ��ɹ���ʼ��
	emit openCameraFinished_camera(true);
}

//����roi����Ĵ�С
void CameraControler::calcRoiRect(Size sz)
{
	//����ROI�����λ�úʹ�С
	int x_tl = (sz.width - roiRect.width) / 2;
	int y_tl = (sz.height - roiRect.height) / 2;
	roiRect.x = x_tl;
	roiRect.y = y_tl;
	roiRect &= Rect(Point(0, 0), sz); //��ֹ�����߽�

	//����������α߿��ROI�����λ�úʹ�С
	roiBoxRect = cv::Rect(Point(x_tl - roiLineWidth, y_tl - roiLineWidth),
		roiRect.size() + Size(2 * roiLineWidth, 2 * roiLineWidth));
	roiBoxRect &= Rect(Point(0, 0), sz); //��ֹ�����߽�
}

//�ر�����ͷ���ͷ���Դ
void CameraControler::closeCamera()
{
	timer->stop();
	if (camera.isOpened())
		camera.release();
}


/****************** ��ͼ *******************/

//��ȡ֡
void CameraControler::readFrame()
{
	Mat cvFrame;
	camera >> cvFrame; //�������ץȡһ֡
	if (cvFrame.empty()) camera >> cvFrame; //���Ϊ������ץһ֡
	if (cvFrame.empty()) return; //�����Ϊ���򷵻�

	cv::rectangle(cvFrame, roiBoxRect, cv::Scalar(0, 0, 255), roiLineWidth, cv::LINE_8, 0);//��֡ͼ���м�λ�û��ƾ��ο�
	//cv::imwrite("./frame.jpg" ,cvFrame);

	*qFrame = QImage((const uchar*)cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);//ת��
	*qFrame = qFrame->rgbSwapped();
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
	*qFrame = QImage((const uchar*)cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);//ת��
	*qFrame = qFrame->rgbSwapped();
	emit refreshFrame_camera();//֪ͨ��Ʒ��Ž������֡

	Mat roiImage = cvFrame(roiRect); //��ȡ�ؼ�֡��ROI����
	cv::imwrite(roiImagePath->toStdString(), roiImage); //����ROI�����ͼ��
}


/**************** ��ͣ��ͼ���� ******************/

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
