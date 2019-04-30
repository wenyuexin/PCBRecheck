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

//����������
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
	QGraphicsScene fullImageScene; //��ʾPCB��ͼ�ĳ���
	FlickeringArrow flickeringArrow; //PCB��ͼ����˸�ļ�ͷ
	QTimer *timer; //��ʱ��
	QPixmap lightOnIcon; //����ͼ�� red
	QPixmap lightOffIcon; //���ͼ�� grey

	QString IconFolder; //ͼ���ļ���
	pcb::UserConfig userConfig; //����ϵͳ������Ϣ
	pcb::RuntimeParams runtimeParams; //���в���

	int defectIndex; //��ǰ������ʾ��ȱ��ͼ
	QVector<QString> flawImagePathVec; //��ǰ������Ӧ��������ͼ��·��
	QVector<QStringList> flawImageInfoVec; //ȱ��ͼ�Ļ�����Ϣ�����꣩
	uint16_t flawIndicatorStatus; //ָʾ������״̬

	pcb::FolderHierarchy OutFolderHierarchy; //���Ŀ¼�µ��ļ��в��
	int defectNum = 0;

public:
	PCBRecheck(QWidget *parent = Q_NULLPTR);
	~PCBRecheck();

private:
	void initRecheckMainUI();
	void showLastFlawImage(); //��ʾ��һ��ȱ��ͼ
	void showNextFlawImage(); //��ʾ��һ��ȱ��ͼ
	
	void refreshRecheckUI(); //���¼�����
	void showFullImage(); //��ʾPCB��ͼ
	void setFlickeringArrowPos(); //���¼�ͷ��λ��
	void showSerialNumberUI();
	void showExitQueryUI();
	void getFlawImgInfo(QString dirpath);
	void exitRecheckSystem();
	void showFlawImage();
	void switchFlawIndicator();

private Q_SLOTS:
	void on_sysInitFinished_initThread(); //��ʼ���̷߳���������
	void on_userConfigError_initThread(); //��ʼ������
	void on_outFolderHierarchyError_initThread(); //��ʼ������

	void on_pushButton_plus2_clicked(); //����ӺŰ���
	void on_pushButton_minus2_clicked(); //������Ű���
	void on_pushButton_exit2_clicked(); //����˳�����
	void keyPressEvent(QKeyEvent *event); //�û������¼�

	void do_showRecheckMainUI_numUI(); //pcb������ý��淵��������
	void do_exitRecheckSystem_numUI(); //�˳�ϵͳ

	void do_showSerialNumberUI_exitUI();
	void do_showRecheckUI_exitUI();
	void on_timeOut();
};
