#include "PCBRecheck.h"


//复查界面构造函数
PCBRecheck::PCBRecheck(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//判断并选择在主屏或者是副屏上显示
	QDesktopWidget* desktop = QApplication::desktop();
	QRect screenRect = desktop->screenGeometry(1);//副屏区域
	if (screenRect.width() < 1440 || screenRect.height() < 900) {
		screenRect = desktop->screenGeometry(0);//主屏区域
	}
	this->setGeometry(screenRect);

	//全屏显示
	if (true && !(this->isFullScreen())) {
		this->setWindowFlags(Qt::SubWindow);
		this->showFullScreen();
	}

	//检修界面初始化
	initRecheckMainUI();

	//系统初始化线程
	sysInitThread = new SysInitThread;
	sysInitThread->setOutFolderHierarchy(&OutFolderHierarchy);
	sysInitThread->setRecheckConfig(&userConfig);
	sysInitThread->start(); //开启线程
	connect(sysInitThread, SIGNAL(sysInitFinished_initThread()), this, SLOT(on_sysInitFinished_initThread()));
	connect(sysInitThread, SIGNAL(userConfigError_initThread()), this, SLOT(on_userConfigError_initThread()));
	connect(sysInitThread, SIGNAL(outFolderHierarchyError_initThread()), this, SLOT(on_outFolderHierarchyError_initThread()));

	//编号设置界面
	serialNumberUI = new SerialNumberUI;
	serialNumberUI->setRuntimeParams(&runtimeParams);
	connect(serialNumberUI, SIGNAL(showRecheckMainUI_numUI()), this, SLOT(do_showRecheckMainUI_numUI()));
	connect(serialNumberUI, SIGNAL(exitRecheckSystem_numUI()), this, SLOT(do_exitRecheckSystem_numUI()));
	
	//退出询问界面
	exitQueryUI = new ExitQueryUI;
	connect(exitQueryUI, SIGNAL(showSerialNumberUI_exitUI()), this, SLOT(do_showSerialNumberUI_exitUI()));
	connect(exitQueryUI, SIGNAL(showRecheckUI_exitUI()), this, SLOT(do_showRecheckUI_exitUI()));

	//定时器
	timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(on_timeOut()));
}

//复查界面析构函数
PCBRecheck::~PCBRecheck()
{
	delete sysInitThread;
	delete serialNumberUI;
	delete exitQueryUI;
	delete timer;
}

/********************* 检修界面初始化 *********************/

void PCBRecheck::initRecheckMainUI()
{
	//加载logo
	IconFolder = QDir::currentPath() + "/Icons";
	QImage image(IconFolder + "/logo.png");
	ui.label_logo->setPixmap(QPixmap::fromImage(image.scaled(ui.label_logo->size(), Qt::KeepAspectRatio)));
	qDebug() << IconFolder;

	//加载其他图标
	QImage redIcon = QImage(IconFolder + "/red.png"); //red
	lightOnIcon = QPixmap::fromImage(redIcon.scaled(ui.label_indicator1->size(), Qt::KeepAspectRatio));
	QImage greyIcon = QImage(IconFolder + "/grey.png"); //grey
	lightOffIcon = QPixmap::fromImage(greyIcon.scaled(ui.label_indicator1->size(), Qt::KeepAspectRatio));

	//设置当前正在显示的缺陷小图及其缺陷类型指示灯
	defectIndex = -1; //default
	ui.label_indicator1->setPixmap(lightOffIcon); //A
	ui.label_indicator2->setPixmap(lightOffIcon); //B
	ui.label_indicator3->setPixmap(lightOffIcon); //C
	ui.label_indicator4->setPixmap(lightOffIcon); //D

	//文本框等的背景色
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(250, 250, 250));
	ui.label_flaw->setAutoFillBackground(true); //缺陷图
	ui.label_flaw->setPalette(palette);
	ui.label_xLoc->setAutoFillBackground(true); //缺陷位置x
	ui.label_xLoc->setPalette(palette);
	ui.label_yLoc->setAutoFillBackground(true); //缺陷位置y
	ui.label_yLoc->setPalette(palette);
	ui.label_serialNum->setAutoFillBackground(true); //样本编号
	ui.label_serialNum->setPalette(palette);
}

/******************* 槽函数：初始化处理 *******************/

//当用户参数无效
void PCBRecheck::on_userConfigError_initThread()
{
	userConfig.showMessageBox(this); //弹窗提示
	pcb::delay(10); //延时
	exitRecheckSystem(); //退出系统
}


void PCBRecheck::on_outFolderHierarchyError_initThread()
{
	QMessageBox::warning(Q_NULLPTR, QString::fromLocal8Bit("警告"),
		QString::fromLocal8Bit("无待检修样本，请先进行检测!  "), 
		QString::fromLocal8Bit("确定"));
	exitRecheckSystem(); //退出系统
}

//系统初始化结束
void PCBRecheck::on_sysInitFinished_initThread()
{
	//处理初始化线程返回的参数
	//OutputDirPath = userConfig.OutputDirPath;
	//TemplDirPath = userConfig.TemplDirPath;
	//ImageFormat = userConfig.ImageFormat;

	//将输出目录下的文件夹层次传递给编号设置界面
	serialNumberUI->setFolderHierarchy(&OutFolderHierarchy);

	//显示PCB序号询问界面
	showSerialNumberUI();
}

/************************ 编号设置界面 ***********************/

//由序号设置界面返回，并显示检修主界面
void PCBRecheck::do_showRecheckMainUI_numUI()
{
	serialNumberUI->hide(); //隐藏PCB编号设置界面
	ui.pushButton_plus2->setEnabled(true);
	ui.pushButton_minus2->setEnabled(true);
	ui.pushButton_exit2->setEnabled(true);
	refreshRecheckUI(); //更新界面上显示的信息
}

//由参数设置界面返回，并退出检修系统
void PCBRecheck::do_exitRecheckSystem_numUI()
{
	exitRecheckSystem();
}


/************************ 退出询问界面 ***********************/

//隐藏退出询问界面，并显示编号设置界面
void PCBRecheck::do_showSerialNumberUI_exitUI()
{
	exitQueryUI->hide();  //隐藏退出询问界面
	Sleep(10); //睡眠10ms
	
	if (!serialNumberUI->getNextSerialNum()) { //获取同批次的下一个产品序号
		//提示该批次已经检修完
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("该批次的所有样本已经检修完成!  "), 
			QString::fromLocal8Bit("确定"));
	}
	//显示编号设置界面
	showSerialNumberUI(); 
}

//退出系统
void PCBRecheck::do_showRecheckUI_exitUI()
{
	exitQueryUI->hide();
	ui.pushButton_plus2->setEnabled(true);
	ui.pushButton_minus2->setEnabled(true);
	ui.pushButton_exit2->setEnabled(true);
}


/********* 初始显示：加载PCB大图、第1个缺陷小图等 ********/

//刷新计时器与小箭头
void PCBRecheck::on_timeOut()
{
	flickeringArrow.update(-100, -100, 200, 200);
	timer->start(500);
}

//更新界面上显示的信息
void PCBRecheck::refreshRecheckUI()
{
	/*更新界面中的PCB编号*/
	QFont font("SimSun-ExtB", 16, 0); //字体，字体大小，加粗权重 
	ui.label_serialNum->setFont(font); //设置字体
	ui.label_serialNum->setText(runtimeParams.serialNum); //更新pcb编号

	QString flawImageFolder = userConfig.OutputDirPath + "/"
		+ runtimeParams.sampleModelNum + "/" + runtimeParams.sampleBatchNum + "/" 
		+ runtimeParams.sampleNum;//检测结果所在的文件夹
	if (!QFileInfo(flawImageFolder).isDir()) {
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"),
			QString::fromLocal8Bit("路径定位失败，无法获取相应的检测结果!\n") 
			+ QString("path: ./output/" + runtimeParams.sampleModelNum + "/" 
				+ runtimeParams.sampleBatchNum + "/" + runtimeParams.sampleNum),
			QString::fromLocal8Bit("确定"));
		Sleep(10);
		showSerialNumberUI(); //显示PCB序号询问界面
		return;
	}
	else {
		getFlawImgInfo(flawImageFolder); //获取文件夹内的图片路径
	}

	//加载并显示PCB大图
	defectIndex = 0;
	showFullImage();
	ui.label_defectNum->setText(QString::number(defectNum));

	/*加载并显示第1个缺陷小图*/
	QFileInfo flawImgInfo(flawImagePathVec[0]);
	if (!flawImgInfo.isFile()) {
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"),
			QString::fromLocal8Bit("无法打开对应的缺陷图!  "));
	}
	else {
		showFlawImage(); //显示缺陷图
	}
}

// 获取当前编号对应的所有缺陷图片的数据缓存到内存中，
// 上下切换缺陷图时，只需从数组中读取数据即可
void PCBRecheck::getFlawImgInfo(QString dirpath)
{
	QDir dir(dirpath);
	dir.setSorting(QDir::Name | QDir::Time | QDir::Reversed);
	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

	QStringList filters("*" + userConfig.ImageFormat); // 所有图片的格式为 bmp  将bmp格式的图片过滤出来 
	dir.setNameFilters(filters);

	QFileInfoList folder_list = dir.entryInfoList();
	flawImageInfoVec.clear();
	flawImagePathVec.clear();
	for (int i = 0; i < folder_list.size(); i++) {
		flawImagePathVec.push_back(folder_list.at(i).absoluteFilePath()); //缺陷图路径
		flawImageInfoVec.push_back(folder_list.at(i).baseName().split("_")); //缺陷图信息
	}
}


//加载并显示PCB大图
//从对应的output文件夹的fullImage子文件夹中读取整图
void PCBRecheck::showFullImage()
{
	QString flawImageDirPath = userConfig.OutputDirPath + "/" 
		+ runtimeParams.sampleModelNum + "/"
		+ runtimeParams.sampleBatchNum + "/" 
		+ runtimeParams.sampleNum; //检测结果所在的文件夹

	QString fullImageDirPath = flawImageDirPath + "/fullImage/";
	QDir dir(fullImageDirPath);
	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
	QStringList filters("*" + userConfig.ImageFormat); //将ImgFormat格式的图片过滤出来 
	dir.setNameFilters(filters);
	QFileInfoList folder_list = dir.entryInfoList(); //获取文件列表

	QString fullImagePath = "";
	QStringList pcbInfoList;//整图的原图大小+缺陷总数
	for (int i = 0; i < folder_list.size(); i++) {
		if (folder_list.at(i).baseName().startsWith(fullImageNamePrefix)) {
			fullImagePath = folder_list.at(i).absoluteFilePath();
			pcbInfoList = folder_list.at(i).baseName().split("_");
			break;
		}
	}

	if (fullImage.load(fullImagePath)) {
		//根据文件名获取PCB整图的原始尺寸和缺陷总数
		if (pcbInfoList.size() == 4) {
			originalFullImageSize.setWidth(pcbInfoList[1].toInt());
			originalFullImageSize.setHeight(pcbInfoList[2].toInt());
			defectNum = pcbInfoList[3].toInt();
			qDebug() << "defectNum =" << defectNum;
		}
		else {
			originalFullImageSize = QSize(-1, -1);
			defectNum = -1;
		}

		//删除场景中之前加载的元素
		QList<QGraphicsItem *> itemList = fullImageScene.items();
		for (int i = 0; i < itemList.size(); i++) {
			fullImageScene.removeItem(itemList[i]);  //从scene中移除
		}

		//加载PCB大图
		scaledFactor = min(qreal(ui.graphicsView_full->height() - 2) / fullImage.size().height(),
			qreal(ui.graphicsView_full->width() - 2) / fullImage.size().width()); //PCB大图的尺寸变换因子
		fullImage = fullImage.scaled(fullImage.size()*scaledFactor, Qt::KeepAspectRatio); //图像缩放
		fullImageSize = fullImage.size(); //PCB大图的实际显示尺寸
		fullImageScene.addPixmap(QPixmap::fromImage(fullImage)); //将图像加载进场景中
		fullImageScene.setSceneRect(0, 0, fullImageSize.width(), fullImageSize.height()); //设置场景范围

		//加载闪烁的箭头
		timer->start(500); //启动定时器
		setFlickeringArrowPos(); //更新闪烁箭头的位置
		flickeringArrow.setFullImageSize(&fullImageSize);
		fullImageScene.addItem(&flickeringArrow); //将箭头加载进场景中

		//控件设置与图像显示
		ui.graphicsView_full->setFocusPolicy(Qt::NoFocus);
		ui.graphicsView_full->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //禁用水平滚动条
		ui.graphicsView_full->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //禁用垂直滚动条
		ui.graphicsView_full->setScene(&fullImageScene); //设置场景
		ui.graphicsView_full->show(); //显示图像
	}
	else {
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"),
			QString::fromLocal8Bit("无法加载相应的PCB整图!  "),
			QString::fromLocal8Bit("确定"));
		pcb::delay(10); //延迟
		serialNumberUI->show(); //显示PCB序号询问界面
	}
}

//更新闪烁箭头的位置
void PCBRecheck::setFlickeringArrowPos()
{
	qreal xLoc = flawImageInfoVec[defectIndex][1].toDouble();
	xLoc *= (1.0*fullImageSize.width()/originalFullImageSize.width());
	qreal yLoc = flawImageInfoVec[defectIndex][2].toDouble();
	yLoc *= (1.0*fullImageSize.height()/originalFullImageSize.height());
	flickeringArrow.setPos(xLoc, yLoc); //设置箭头的位置
}

/***************** 事件响应：切换缺陷小图 ****************/

//敲击小键盘的 + -
void PCBRecheck::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) 
	{	
	case Qt::Key_Plus: //切换并显示下一个缺陷
		qDebug() << "plus";
		showNextFlawImage();
		break;

	case Qt::Key_Minus: //切换并显示上一个缺陷
		qDebug() << "minus";
		showLastFlawImage();
		break;

	default:
		break;
	}
}

//点击加号按键 - 切换并显示下一个缺陷
void PCBRecheck::on_pushButton_plus2_clicked()
{
	qDebug() << "plus2";
	showNextFlawImage();
}

//点击减号按键 - 切换并显示上一个缺陷
void PCBRecheck::on_pushButton_minus2_clicked()
{
	qDebug() << "minus2";
	showLastFlawImage();
}


/**************** 功能实现：切换缺陷小图 *****************/

//切换并显示上一个缺陷图
void PCBRecheck::showLastFlawImage()
{
	//获取 当前的 index 判断是否 到达边界
	defectIndex -= 1;
	if (defectIndex < 0) {  //边界
		defectIndex += 1;
		qDebug() << "this is the first one";
		showExitQueryUI(); //显示退出询问框
	}
	else {
		showFlawImage();
	}
}

//切换并显示下一个缺陷图
void PCBRecheck::showNextFlawImage()
{
	//获取 当前的 index 判断是否 到达边界
	defectIndex += 1;
	if (defectIndex > flawImagePathVec.size() - 1) {  //边界
		defectIndex -= 1;
		qDebug() << "this is the last one";
		showExitQueryUI(); //显示退出询问框
	}
	else {
		showFlawImage();
	}
}

//将缺陷图加载并显示到对应的lebal控件中
void PCBRecheck::showFlawImage()
{
	QImage flawImage(flawImagePathVec[defectIndex]); //缺陷图
	ui.label_flaw->setPixmap(QPixmap::fromImage(flawImage.scaled(ui.label_flaw->size(), Qt::KeepAspectRatio))); //显示图像
	ui.label_xLoc->setText(flawImageInfoVec[defectIndex][1]); //更新缺陷的x坐标
	ui.label_yLoc->setText(flawImageInfoVec[defectIndex][2]); //更新缺陷的y坐标
	switchFlawIndicator(); //更新缺陷类型图标(修改指示灯亮灭状态)

	//更新PCB大图上的小箭头的位置
	setFlickeringArrowPos();
}

//更新缺陷类型的指示图标
void PCBRecheck::switchFlawIndicator()
{
	flawIndicatorStatus = pow(2, flawImageInfoVec[defectIndex][3].toInt() - 1);
	ui.label_indicator1->setPixmap(bool((flawIndicatorStatus & 0x1) >> 0) ? lightOnIcon : lightOffIcon); //A
	ui.label_indicator2->setPixmap(bool((flawIndicatorStatus & 0x2) >> 1) ? lightOnIcon : lightOffIcon); //B
	ui.label_indicator3->setPixmap(bool((flawIndicatorStatus & 0x4) >> 2) ? lightOnIcon : lightOffIcon); //C
	ui.label_indicator4->setPixmap(bool((flawIndicatorStatus & 0x8) >> 3) ? lightOnIcon : lightOffIcon); //D
}


/**************** 事件响应：点击退出按键 *****************/

void PCBRecheck::on_pushButton_exit2_clicked()
{
	exitRecheckSystem();
}


/******************** 程序控制 *********************/

void PCBRecheck::showSerialNumberUI()
{
	ui.pushButton_plus2->setEnabled(false);
	ui.pushButton_minus2->setEnabled(false);
	ui.pushButton_exit2->setEnabled(false);
	this->serialNumberUI->show(); //弹出退出询问框
}

void PCBRecheck::showExitQueryUI()
{
	ui.pushButton_plus2->setEnabled(false);
	ui.pushButton_minus2->setEnabled(false);
	ui.pushButton_exit2->setEnabled(false);
	this->exitQueryUI->show(); //弹出退出询问框
}

void PCBRecheck::exitRecheckSystem()
{
	this->close();
	qApp->exit(0);
}
