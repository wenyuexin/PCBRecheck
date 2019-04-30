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

public:
	enum RecheckStatus {
		NoError,
		Uncheck,
		CurrentBatchRechecked,
		LoadFullImageFailed,
		LoadFlawImageFailed,
		OpenFlawImageFolderFailed,
		Default
	};

private:
	Ui::PCBRecheckClass ui;
	SysInitThread *sysInitThread; //ϵͳ��ʼ���߳�
	SerialNumberUI *serialNumberUI; //pcb������ý���
	ExitQueryUI *exitQueryUI; //�˳�ѯ�ʽ���
	pcb::UserConfig userConfig; //����ϵͳ������Ϣ
	pcb::RuntimeParams runtimeParams; //���в���
	RecheckStatus recheckStatus; //����״̬

	QPixmap fullImage; //PCB��ͼ
	const QString fullImageNamePrefix = "fullImage"; //�ļ���ǰ׺
	QSize originalFullImageSize; //PCB��ͼ��ԭʼ�ߴ�
	QSize fullImageItemSize; //PCB��ͼ����ʾ�ߴ�
	qreal scaledFactor; //PCB��ͼ�ĳߴ�任����
	QGraphicsScene fullImageScene; //��ʾPCB��ͼ�ĳ���
	FlickeringArrow flickeringArrow; //PCB��ͼ����˸�ļ�ͷ
	QTimer *timer; //��ʱ��

	QString IconFolder; //ͼ���ļ���
	QPixmap lightOnIcon; //����ͼ�� red
	QPixmap lightOffIcon; //���ͼ�� grey

	int defectNum; //ȱ������
	int defectIndex; //��ǰ������ʾ��ȱ��ͼ
	QVector<pcb::FlawImageInfo> flawImageInfoVec; //ȱ��ͼ����Ϣ
	uint16_t flawIndicatorStatus; //ָʾ������״̬

	pcb::FolderHierarchy OutFolderHierarchy; //���Ŀ¼�µ��ļ��в��

public:
	PCBRecheck(QWidget *parent = Q_NULLPTR);
	~PCBRecheck();

private:
	void initRecheckMainUI();
	void showLastFlawImage(); //��ʾ��һ��ȱ��ͼ
	void showNextFlawImage(); //��ʾ��һ��ȱ��ͼ
	
	void refreshRecheckMainUI(); //���¼�����
	bool loadFullImage(); //����PCB��ͼ
	void showFullImage(); //��ʾPCB��ͼ
	void initFlickeringArrow(); //���س�ʼ����˸��ͷ
	void setFlickeringArrowPos(); //���¼�ͷ��λ��

	void getFlawImageInfo(QString dirpath);
	void showFlawImage();
	void switchFlawIndicator();

	void showSerialNumberUI();
	void showExitQueryUI();

	void exitRecheckSystem();
	void showMessageBox(pcb::MessageBoxType type, RecheckStatus status = Default);

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
	void do_showRecheckMainUI_exitUI();
	void on_timeOut();
};
