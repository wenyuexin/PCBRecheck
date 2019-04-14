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

//检修界面
namespace Ui {
	class PCBRecheck;
}

class PCBRecheck : public QMainWindow
{
	Q_OBJECT

private:
	Ui::PCBRecheckClass ui;
	SysInitThread *sysInitThread; //系统初始化线程
	SerialNumberUI *serialNumberUI; //pcb编号设置界面
	ExitQueryUI *exitQueryUI; //退出询问界面

	QImage fullImage; //PCB大图
	QString fullImageNamePrefix = "fullImage"; //文件名前缀
	QSize originalFullImageSize; //PCB大图的原始尺寸
	QSize fullImageSize; //PCB大图的显示尺寸
	qreal scaledFactor; //PCB大图的尺寸变换因子
	QGraphicsScene fullPCBScene; //显示PCB大图的场景
	FlickeringArrow flickeringArrow; //PCB大图上闪烁的箭头
	QTimer *timer; //定时器
	QPixmap lightOnIcon; //亮灯图标 red
	QPixmap lightOffIcon; //灭灯图标 grey

	QString IconFolder; //图标文件夹
	Ui::RecheckConfig config; //检修系统配置信息
	QString OutputDirPath;//检测结果存储路径
	QString TemplDirPath;//模板文件的存储路径
	QString ImageFormat; //图像后缀

	QString serialNum; //序列号
	QString sampleTypeNum; //型号
	QString sampleBatchNum; //批次号
	QString sampleNum; //样本编号
	QString *numPtrArray[4] = { &serialNum, &sampleTypeNum, &sampleBatchNum, &sampleNum };
	int currentFlawIndex; //当前正在显示的缺陷图
	QVector<QString> flawImgPathVec; //当前样本对应的所有陷图的路径
	QVector<QStringList> flawImgInfoVec; //缺陷图的基本信息（坐标）
	uint16_t flawIndicatorStatus; //指示灯亮灭状态

	Ui::FolderHierarchy OutFolderHierarchy; //输出目录下的文件夹层次
	int defectNum = 0;

public:
	PCBRecheck(QWidget *parent = Q_NULLPTR);
	~PCBRecheck();

private:
	void initRecheckMainUI();
	void showLastFlawImage(); //显示上一张缺陷图
	void showNextFlawImage(); //显示下一张缺陷图
	
	void refreshRecheckUI(); //更新检测界面
	void showFullPcbImage(); //显示PCB整图
	void showFullPcbImage2(); //显示PCB整图
	void setFlickeringArrowPos(); //更新箭头的位置
	void showSerialNumberUI();
	void showExitQueryUI();
	void getFlawImgInfo(QString dirpath);
	void exitRecheckSystem();
	void showFlawImage();
	void switchFlawIndicator();

private Q_SLOTS:
	void on_pushButton_plus2_clicked(); //点击加号按键
	void on_pushButton_minus2_clicked(); //点击减号按键
	void on_pushButton_exit2_clicked(); //点击退出按键
	void keyPressEvent(QKeyEvent *event); //敲击键盘事件
	void do_showRecheckUI_initThread(); //初始化线程返回主界面
	void on_configError1_initThread(); //初始化错误
	void on_configError2_initThread(); //初始化错误
	void on_outFolderHierarchyError_initThread(); //初始化错误
	void do_showRecheckUI_numUI(); //pcb编号设置界面返回主界面
	void do_exitRecheckSystem_numUI(); //退出系统
	void do_showSerialNumberUI_exitUI();
	void do_showRecheckUI_exitUI();
	void on_timeOut();
};
