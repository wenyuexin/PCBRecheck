#include "PCBRecheck.h"

using pcb::MessageBoxType;
using pcb::FlawImageInfo;


//复查界面的构造函数
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

	//检修界面初始化
	this->initRecheckMainUI();

	//系统初始化线程
	sysInitThread = new SysInitThread;
	sysInitThread->setOutFolderHierarchy(&OutFolderHierarchy);
	sysInitThread->setRecheckConfig(&userConfig);
	sysInitThread->start(); //开启线程
	connect(sysInitThread, SIGNAL(sysInitFinished_initThread()), this, SLOT(on_sysInitFinished_initThread()));
	connect(sysInitThread, SIGNAL(userConfigError_initThread()), this, SLOT(on_userConfigError_initThread()));
	connect(sysInitThread, SIGNAL(outFolderHierarchyError_initThread()), this, SLOT(on_outFolderHierarchyError_initThread()));

	//编号设置界面
	serialNumberUI = new SerialNumberUI(Q_NULLPTR, screenRect);
	serialNumberUI->setUserConfig(&userConfig);
	serialNumberUI->setRuntimeParams(&runtimeParams);
	serialNumberUI->init();
	connect(serialNumberUI, SIGNAL(showRecheckMainUI_numUI()), this, SLOT(do_showRecheckMainUI_numUI()));
	connect(serialNumberUI, SIGNAL(exitRecheckSystem_numUI()), this, SLOT(do_exitRecheckSystem_numUI()));
	
	//退出询问界面
	exitQueryUI = new ExitQueryUI(Q_NULLPTR, screenRect);
	connect(exitQueryUI, SIGNAL(showSerialNumberUI_exitUI()), this, SLOT(do_showSerialNumberUI_exitUI()));
	connect(exitQueryUI, SIGNAL(showRecheckUI_exitUI()), this, SLOT(do_showRecheckMainUI_exitUI()));

	//定时器
	timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(on_timeOut()));

	//显示复查主界面
	this->showFullScreen();
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
	IconFolder = QDir::currentPath() + "/icons";
	QPixmap image(IconFolder + "/logo.png");
	ui.label_logo->setPixmap(image.scaled(ui.label_logo->size(), Qt::KeepAspectRatio));
	qDebug() << IconFolder;

	//加载其他图标
	QPixmap redIcon(IconFolder + "/red.png"); //red
	lightOnIcon = redIcon.scaled(ui.label_indicator1->size(), Qt::KeepAspectRatio);
	QPixmap greyIcon(IconFolder + "/grey.png"); //grey
	lightOffIcon = greyIcon.scaled(ui.label_indicator1->size(), Qt::KeepAspectRatio);

	//设置当前正在显示的缺陷小图及其缺陷类型指示灯
	ui.label_indicator1->setPixmap(lightOffIcon); //断路
	ui.label_indicator2->setPixmap(lightOffIcon); //缺失
	ui.label_indicator3->setPixmap(lightOffIcon); //短路
	ui.label_indicator4->setPixmap(lightOffIcon); //凸起

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

	//成员变量初始化
	originalFullImageSize = QSize(-1, -1);
	defectNum = -1;
	defectIndex = -1; 

	//graphicsView的设置与图像显示
	ui.graphicsView_full->setFocusPolicy(Qt::NoFocus);
	ui.graphicsView_full->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //禁用水平滚动条
	ui.graphicsView_full->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //禁用垂直滚动条
}


/******************* 初始化线程 *******************/

//当用户参数无效
void PCBRecheck::on_userConfigError_initThread()
{
	userConfig.showMessageBox(this); //弹窗提示
	pcb::delay(10); //延时
	this->exitRecheckSystem(); //退出系统
}


void PCBRecheck::on_outFolderHierarchyError_initThread()
{
	QMessageBox::warning(this, pcb::chinese("提示"),
		pcb::chinese("无待检修样本，请先进行检测!  "),
		pcb::chinese("确定"));
	pcb::delay(10); //延时
	this->exitRecheckSystem(); //退出系统
}

//系统初始化结束
void PCBRecheck::on_sysInitFinished_initThread()
{
	//将输出目录下的文件夹层次传递给编号设置界面
	serialNumberUI->setFolderHierarchy(&OutFolderHierarchy);

	//显示PCB序号询问界面
	this->showSerialNumberUI();
}


/***************** 编号设置界面 ******************/

//显示编号设置界面
void PCBRecheck::showSerialNumberUI()
{
	ui.pushButton_plus2->setEnabled(false);
	ui.pushButton_minus2->setEnabled(false);
	ui.pushButton_exit->setEnabled(false);
	serialNumberUI->setActivated(true);
	serialNumberUI->show(); //弹出退出询问框
}

//由序号设置界面返回，并显示检修主界面
void PCBRecheck::do_showRecheckMainUI_numUI()
{
	serialNumberUI->hide(); //隐藏PCB编号设置界面
	serialNumberUI->setActivated(false);
	ui.pushButton_plus2->setEnabled(true);
	ui.pushButton_minus2->setEnabled(true);
	ui.pushButton_exit->setEnabled(true);
	this->refreshRecheckMainUI(); //更新界面上显示的信息
}

//由参数设置界面返回，并退出检修系统
void PCBRecheck::do_exitRecheckSystem_numUI()
{
	this->exitRecheckSystem();
}


/****************** 退出询问界面 *****************/

//显示退出询问界面
void PCBRecheck::showExitQueryUI()
{
	ui.pushButton_plus2->setEnabled(false);
	ui.pushButton_minus2->setEnabled(false);
	ui.pushButton_exit->setEnabled(false);
	exitQueryUI->show(); //弹出退出询问框
}

//隐藏退出询问界面，并显示编号设置界面
void PCBRecheck::do_showSerialNumberUI_exitUI()
{
	exitQueryUI->hide();  //隐藏退出询问界面
	pcb::delay(10); //睡眠10ms
	
	if (!serialNumberUI->getNextSerialNum()) { //获取同批次的下一个产品序号
		recheckStatus = CurrentBatchRechecked; //提示该批次已经检修完
		this->showMessageBox(MessageBoxType::Information, recheckStatus);
	}
	//显示编号设置界面
	showSerialNumberUI(); 
}

//退出系统
void PCBRecheck::do_showRecheckMainUI_exitUI()
{
	exitQueryUI->hide();
	ui.pushButton_plus2->setEnabled(true);
	ui.pushButton_minus2->setEnabled(true);
	ui.pushButton_exit->setEnabled(true);
}


/********* 初始显示：加载PCB大图、第1个缺陷小图等 ********/

//刷新计时器与小箭头
void PCBRecheck::on_timeOut()
{
	flickeringArrow.update(-100, -100, 200, 200);
	timer->start(500);
}

//更新界面上显示的信息
void PCBRecheck::refreshRecheckMainUI()
{
	logging(runtimeParams.serialNum);

	//更新界面中的PCB编号
	ui.label_serialNum->setText(runtimeParams.serialNum);

	//检测结果所在的文件夹
	QString flawImageFolderPath = userConfig.OutputDirPath + "/"
		+ runtimeParams.getRelativeFolderPath();
	//判断产品序号对应文件夹是否存在
	if (!QFileInfo(flawImageFolderPath).isDir()) {
		recheckStatus = OpenFlawImageFolderFailed;
		this->showMessageBox(MessageBoxType::Warning, recheckStatus);
		this->showSerialNumberUI(); //显示PCB序号询问界面
		return;
	}

	//加载并显示PCB大图
	if (!loadFullImage()) return;
	this->showFullImage();

	//获取文件夹内的图片路径
	getFlawImageInfo(flawImageFolderPath);

	//加载闪烁的箭头
	defectIndex = 0;
	this->initFlickeringArrow();
	
	//设置场景和显示视图
	ui.graphicsView_full->setScene(&fullImageScene); //设置场景
	ui.graphicsView_full->show(); //显示图像

	//加载并显示第1个缺陷小图
	this->showFlawImage(); //显示缺陷图
}

//加载PCB整图
bool PCBRecheck::loadFullImage()
{
	QString flawImageDirPath = userConfig.OutputDirPath + "/"
		+ runtimeParams.sampleModelNum + "/"
		+ runtimeParams.sampleBatchNum + "/"
		+ runtimeParams.sampleNum; //检测结果所在的文件夹

	QDir dir(flawImageDirPath + "/fullImage/");
	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
	QStringList filters("*" + userConfig.ImageFormat); //将特定格式的图片过滤出来 
	dir.setNameFilters(filters);
	QFileInfoList folder_list = dir.entryInfoList(); //获取文件列表

	QString fullImagePath = "";
	QStringList pcbInfoList;//整图的原图大小+缺陷总数
	for (int i = 0; i < folder_list.size(); i++) {
		if (folder_list.at(i).baseName().startsWith(fullImageNamePrefix)) {
			fullImagePath = folder_list.at(i).absoluteFilePath();

			//根据文件名获取PCB整图的原始尺寸和缺陷总数
			pcbInfoList = folder_list.at(i).baseName().split("_");
			if (pcbInfoList.size() == 4) {
				originalFullImageSize.setWidth(pcbInfoList[1].toInt());
				originalFullImageSize.setHeight(pcbInfoList[2].toInt());
				defectNum = pcbInfoList[3].toInt();
				break;
			}
			else {
				logging("InvalidFullImageName: pcbInfoList size(): " 
					+ QString::number(pcbInfoList.size()));
				recheckStatus = InvalidFullImageName;
				this->showMessageBox(MessageBoxType::Warning, recheckStatus);
				serialNumberUI->show(); //显示PCB序号询问界面
				return false;
			}
		}
	}

	//判断是否找到fullImage为前缀的ImageFormat格式的图
	if (fullImagePath == "") {
		logging("FullImageNotFound");
		recheckStatus = FullImageNotFound;
		this->showMessageBox(MessageBoxType::Warning, recheckStatus);
		serialNumberUI->show(); //显示PCB序号询问界面
		return false;
	}

	//加载大图
	QImage fullImg; //读图
	if (!fullImg.load(fullImagePath)) {
		logging("LoadFullImageFailed: fullImagePath: " + fullImagePath);
		recheckStatus = LoadFullImageFailed;
		this->showMessageBox(MessageBoxType::Warning, recheckStatus);
		serialNumberUI->show(); //显示PCB序号询问界面
		return false;
	}

	scaledFactor = qMin(qreal(ui.graphicsView_full->height() - 2) / fullImg.size().height(),
		qreal(ui.graphicsView_full->width() - 2) / fullImg.size().width());//整图的尺寸变换因子
	fullImg = fullImg.scaled(fullImg.size()*scaledFactor, Qt::KeepAspectRatio); //缩放
	fullImage = QPixmap::fromImage(fullImg); //转换
	fullImageItemSize = fullImage.size(); //PCB大图的实际显示尺寸
	return true;
}

//加载并显示PCB大图
//从对应的output文件夹的fullImage子文件夹中读取整图
void PCBRecheck::showFullImage()
{
	//显示缺陷总数
	if (defectNum >= 0) {
		ui.label_defectNum->setText(QString::number(defectNum));
	}

	//删除场景中之前加载的元素
	QList<QGraphicsItem *> itemList = fullImageScene.items();
	for (int i = 0; i < itemList.size(); i++) {
		fullImageScene.removeItem(itemList[i]);  //从scene中移除
	}

	//将PCB大图加载到场景中
	fullImageScene.addPixmap(fullImage); //将图像加载进场景中
	QRect sceneRect = QRect(QPoint(0, 0), fullImageItemSize); //场景范围
	fullImageScene.setSceneRect(sceneRect); //设置场景范围
}


// 获取当前编号对应的所有缺陷图片的数据缓存到内存中，
// 上下切换缺陷图时，只需从数组中读取数据即可
void PCBRecheck::getFlawImageInfo(QString dirpath)
{
	QDir dir(dirpath);
	dir.setSorting(QDir::Name | QDir::Time | QDir::Reversed);
	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

	QStringList filters("*" + userConfig.ImageFormat); //将特定格式的图片过滤出来 
	dir.setNameFilters(filters);

	QFileInfoList folder_list = dir.entryInfoList();
	flawImageInfoVec.clear();
	flawImageInfoVec.resize(defectNum);
	for (int i = 0; i < folder_list.size(); i++) {
		FlawImageInfo flawImageInfo;
		flawImageInfo.filePath = folder_list.at(i).absoluteFilePath();

		QStringList list = folder_list.at(i).baseName().split("_");
		int index = list.at(0).toInt() - 1;
		if (index >= defectNum) continue;
		flawImageInfo.flawIndex = QString::number(index); //缺陷编号
		flawImageInfo.xPos = list.at(1); //x坐标
		flawImageInfo.yPos = list.at(2); //y坐标
		flawImageInfo.flawType = list.at(3); //缺陷类型
		flawImageInfoVec[index] = flawImageInfo;
	}
}

//加载初始的闪烁箭头
void PCBRecheck::initFlickeringArrow()
{
	if (defectNum <= 0) return;
	timer->start(500); //启动定时器
	setFlickeringArrowPos(); //更新闪烁箭头的位置
	flickeringArrow.setFullImageSize(&fullImageItemSize);
	fullImageScene.addItem(&flickeringArrow); //将箭头加载进场景中
}

//更新闪烁箭头的位置
void PCBRecheck::setFlickeringArrowPos()
{
	qreal xLoc = flawImageInfoVec[defectIndex].xPos.toDouble();
	xLoc *= (1.0*fullImageItemSize.width()/originalFullImageSize.width());
	qreal yLoc = flawImageInfoVec[defectIndex].yPos.toDouble();
	yLoc *= (1.0*fullImageItemSize.height()/originalFullImageSize.height());
	flickeringArrow.setPos(xLoc, yLoc); //设置箭头的位置
}



/***************** 切换缺陷小图 ****************/

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
	case Qt::Key_Asterisk: //直接显示退出询问界面
		qDebug() << "star";
		showExitQueryUI();
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
	if (defectIndex > flawImageInfoVec.size() - 1) {  //边界
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
	if (defectNum <= 0) return;
	QFileInfo flawImgInfo(flawImageInfoVec[defectIndex].filePath);
	if (!flawImgInfo.isFile()) {
		recheckStatus = FlawImageNotFound;
		this->showMessageBox(MessageBoxType::Warning, recheckStatus);
		return;
	}

	QImage flawImg(flawImageInfoVec[defectIndex].filePath); //读图
	flawImg = flawImg.scaled(ui.label_flaw->size(), Qt::KeepAspectRatio); //缩放
	QPixmap flawImage(QPixmap::fromImage(flawImg)); //转换
	ui.label_flaw->clear(); //清空
	ui.label_flaw->setPixmap(flawImage); //显示图像

	//更新其他缺陷信息
	ui.label_xLoc->setText(flawImageInfoVec[defectIndex].xPos); //更新缺陷的x坐标
	ui.label_yLoc->setText(flawImageInfoVec[defectIndex].yPos); //更新缺陷的y坐标
	ui.label_defectIndex->setText(QString::number(defectIndex + 1)); //显示缺陷编号
	this->switchFlawIndicator(); //更新缺陷类型图标(修改指示灯亮灭状态)

	//更新PCB大图上的小箭头的位置
	this->setFlickeringArrowPos();
}

//更新缺陷类型的指示图标
void PCBRecheck::switchFlawIndicator()
{
	flawIndicatorStatus = pow(2, flawImageInfoVec[defectIndex].flawType.toInt() - 1);
	ui.label_indicator1->setPixmap(bool((flawIndicatorStatus & 0x1) >> 0) ? lightOnIcon : lightOffIcon); //A
	ui.label_indicator2->setPixmap(bool((flawIndicatorStatus & 0x2) >> 1) ? lightOnIcon : lightOffIcon); //B
	ui.label_indicator3->setPixmap(bool((flawIndicatorStatus & 0x4) >> 2) ? lightOnIcon : lightOffIcon); //C
	ui.label_indicator4->setPixmap(bool((flawIndicatorStatus & 0x8) >> 3) ? lightOnIcon : lightOffIcon); //D
}


/**************** 退出程序 *****************/

//点击退出按键
void PCBRecheck::on_pushButton_exit_clicked()
{
	exitRecheckSystem();
}

//执行退出
void PCBRecheck::exitRecheckSystem()
{
	this->close();
	qApp->exit(0);
}


/******************** 其他 *********************/

//弹窗报错
void PCBRecheck::showMessageBox(MessageBoxType type, RecheckStatus status)
{
	RecheckStatus tempStatus = (status == Default) ? recheckStatus : status;
	if (tempStatus == NoError) return;

	QString message = "";
	switch (status)
	{
	case PCBRecheck::Uncheck:
		message = pcb::chinese("系统状态未知!  \n"); 
		message += "Recheck: ErrorCode: " + QString::number(tempStatus); break;
	case PCBRecheck::CurrentBatchRechecked:
		message = pcb::chinese("该批次的所有样本已经复查完成!  \n"); 
		message += "Recheck: ErrorCode: " + QString::number(tempStatus); break;
	case PCBRecheck::InvalidFullImageName:
		message = pcb::chinese("PCB整图文件的文件名无效!  \n"); 
		message += "Recheck: ErrorCode: " + QString::number(tempStatus); break;
	case PCBRecheck::FullImageNotFound:
		message = pcb::chinese("没有找到PCB整图!  \n");
		message += "Recheck: ErrorCode: " + QString::number(tempStatus); break;
	case PCBRecheck::LoadFullImageFailed:
		message = pcb::chinese("无法打开PCB整图!  \n"); 
		message += "Recheck: ErrorCode: " + QString::number(tempStatus); break;
	case PCBRecheck::FlawImageNotFound:
		message = pcb::chinese("没有找到PCB缺陷图!  \n");
		message += "Recheck: ErrorCode: " + QString::number(tempStatus); break;
	case PCBRecheck::LoadFlawImageFailed:
		message = pcb::chinese("无法打开相应的缺陷图!  \n"); 
		message += "Recheck: ErrorCode: " + QString::number(tempStatus); break;
	case PCBRecheck::OpenFlawImageFolderFailed:
		message = pcb::chinese("路径定位失败，无法获取相应的检测结果! \n"); 
		message += QString("path: ./output" + runtimeParams.getRelativeFolderPath()) + "\n"; 
		message += "Recheck: ErrorCode: " + QString::number(tempStatus); break;
	case PCBRecheck::Default:
		break;
	}

	QString messageBoxTitle = "";
	QString buttonName = pcb::chinese("确定");
	switch (type)
	{
	case pcb::Warning:
		messageBoxTitle = pcb::chinese("警告");
		QMessageBox::warning(this, messageBoxTitle, message, buttonName);
		break;
	case pcb::Information:
		messageBoxTitle = pcb::chinese("提示");
		QMessageBox::information(this, messageBoxTitle, message, buttonName);
		break;
	case pcb::Question:
		messageBoxTitle = pcb::chinese("询问");
		QMessageBox::question(this, messageBoxTitle, message, buttonName);
		break;
	case pcb::Critical:
		messageBoxTitle = "";
		break;
	case pcb::About:
		messageBoxTitle = "";
		break;
	}
	pcb::delay(10);//延时
}

//添加日志
void PCBRecheck::logging(QString msg)
{
	QString fileName = "./log.txt";
	QFile file(fileName);
	file.open(QIODevice::Append);
	file.close();
	if (file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream.seek(file.size());

		QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
		stream << time << " " << msg << "\n";
		file.close();
	}
}
