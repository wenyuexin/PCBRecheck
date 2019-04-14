#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PCBRecheck.h"
#include "Configurator.h"
#include "SerialNumberUI.h"
#include "SysInitThread.h"
#include "ExitQueryUI.h"
#include "FlickeringArrow.h"
#include <QKeyEvent>
#include <QDebug>
#include <QDir>
#include <QMap>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QPixmap>
#include <QRect>
#include <QDesktopWidget>
#include <QTimer>
#include <time.h> 
#include <windows.h>

//���޽���
namespace Ui {
	class PCBRecheck;
}

class PCBRecheck : public QMainWindow
{
	Q_OBJECT

private:
	Ui::PCBRecheckClass ui;
	SysInitThread *sysInitThread; //ϵͳ��ʼ���߳�
	SerialNumberUI *serialNumberUI; //pcb������ý���
	ExitQueryUI *exitQueryUI; //�˳�ѯ�ʽ���

	QImage fullImage; //PCB��ͼ
	QString fullImageNamePrefix = "fullImage"; //�ļ���ǰ׺
	QSize originalFullImageSize; //PCB��ͼ��ԭʼ�ߴ�
	QSize fullImageSize; //PCB��ͼ����ʾ�ߴ�
	qreal scaledFactor; //PCB��ͼ�ĳߴ�任����
	QGraphicsScene fullPCBScene; //��ʾPCB��ͼ�ĳ���
	FlickeringArrow flickeringArrow; //PCB��ͼ����˸�ļ�ͷ
	QTimer *timer; //��ʱ��
	QPixmap lightOnIcon; //����ͼ�� red
	QPixmap lightOffIcon; //���ͼ�� grey

	QString IconFolder; //ͼ���ļ���
	Ui::RecheckConfig config; //����ϵͳ������Ϣ
	QString OutputDirPath;//������洢·��
	QString TemplDirPath;//ģ���ļ��Ĵ洢·��
	QString ImageFormat; //ͼ���׺

	QString serialNum; //���к�
	QString sampleTypeNum; //�ͺ�
	QString sampleBatchNum; //���κ�
	QString sampleNum; //�������
	QString *numPtrArray[4] = { &serialNum, &sampleTypeNum, &sampleBatchNum, &sampleNum };
	int currentFlawIndex; //��ǰ������ʾ��ȱ��ͼ
	QVector<QString> flawImgPathVec; //��ǰ������Ӧ��������ͼ��·��
	QVector<QStringList> flawImgInfoVec; //ȱ��ͼ�Ļ�����Ϣ�����꣩
	uint16_t flawIndicatorStatus; //ָʾ������״̬

	Ui::FolderHierarchy OutFolderHierarchy; //���Ŀ¼�µ��ļ��в��
	int defectNum = 0;

public:
	PCBRecheck(QWidget *parent = Q_NULLPTR);
	~PCBRecheck();

private:
	void initRecheckMainUI();
	void showLastFlawImage(); //��ʾ��һ��ȱ��ͼ
	void showNextFlawImage(); //��ʾ��һ��ȱ��ͼ
	
	void refreshRecheckUI(); //���¼�����
	void showFullPcbImage(); //��ʾPCB��ͼ
	void showFullPcbImage2(); //��ʾPCB��ͼ
	void setFlickeringArrowPos(); //���¼�ͷ��λ��
	void showSerialNumberUI();
	void showExitQueryUI();
	void getFlawImgInfo(QString dirpath);
	void exitRecheckSystem();
	void showFlawImage();
	void switchFlawIndicator();

private Q_SLOTS:
	void on_pushButton_plus2_clicked(); //����ӺŰ���
	void on_pushButton_minus2_clicked(); //������Ű���
	void on_pushButton_exit2_clicked(); //����˳�����
	void keyPressEvent(QKeyEvent *event); //�û������¼�
	void do_showRecheckUI_initThread(); //��ʼ���̷߳���������
	void on_configError1_initThread(); //��ʼ������
	void on_configError2_initThread(); //��ʼ������
	void on_outFolderHierarchyError_initThread(); //��ʼ������
	void do_showRecheckUI_numUI(); //pcb������ý��淵��������
	void do_exitRecheckSystem_numUI(); //�˳�ϵͳ
	void do_showSerialNumberUI_exitUI();
	void do_showRecheckUI_exitUI();
	void on_timeOut();
};
