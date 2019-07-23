#include "SerialNumberUI.h"


using pcb::RuntimeParams;
using pcb::MessageBoxType;
using std::string;


SerialNumberUI::SerialNumberUI(QWidget *parent, QRect &screenRect)
	: QWidget(parent)
{
	ui.setupUi(this);

	//判断并选择在主屏或者是副屏上显示
	QPoint widgetPos = screenRect.center();
	widgetPos -= QPoint(this->width() / 2.0, this->height() / 2.0);
	QRect widgetRect = QRect(widgetPos, this->size());
	this->setGeometry(widgetRect);

	//成员变量初始化
	NumberValidator = Q_NULLPTR;
	cameraControler = Q_NULLPTR;
	ocrHandle = Q_NULLPTR;
}

SerialNumberUI::~SerialNumberUI()
{
	qDebug() << "~SerialNumberUI";
	delete NumberValidator;
	NumberValidator = Q_NULLPTR;
	delete cameraControler;
	cameraControler = Q_NULLPTR;

	//删除ROI缓存图像
	QFile::remove(roiImagePath);
}


//产品序号界面的初始化
void SerialNumberUI::init()
{
	//去掉标题栏
	this->setWindowFlags(Qt::FramelessWindowHint);

	//限制文本框只能输入数字
	NumberValidator = new QRegExpValidator(QRegExp("[0-9]+$"));
	ui.lineEdit_serialNum->setValidator(NumberValidator);

	//此界面显示时禁用其他窗口
	//this->setWindowModality(Qt::ApplicationModal);

	//成员变量初始化
	cameraLabelSize = ui.label_cameraFrame->size();
	roiImagePath = runtimeParams->BufferDirPath + "/serialNumRoi.bmp";

	//初始化OCR模块
	ocrHandle = TessBaseAPICreate();
	if (TessBaseAPIInit3(ocrHandle, NULL, "eng") != 0) {
		statusCode = OCR_InitFailed;
		showMessageBox(MessageBoxType::Warning, statusCode); return;
	}

	//初始化相机模块
	cameraControler = new CameraControler(this);
	connect(cameraControler, SIGNAL(openCameraFinished_camera(bool)), this, SLOT(on_openCameraFinished_camera(bool)));
	connect(cameraControler, SIGNAL(refreshFrame_camera()), this, SLOT(do_refreshFrame_camera()));
	connect(cameraControler, SIGNAL(captureFailed_camera()), this, SLOT(on_captureFailed_camera()));

	cameraControler->setRoiImagePath(&roiImagePath); //设置ROI缓存图像的存储路径
	cameraControler->setRoiSize(userConfig->roiSize_W, userConfig->roiSize_H);
	cameraControler->setResolution(800, 600); //设置相机分辨率
	cameraControler->init(); //相机控制类的初始化
	frame = cameraControler->getFrame(); //获取关键帧图像变量的指针
	cameraControler->openCamera(); //打开相机
}


/*************** 敲击键盘按键 ***************/

//键盘事件
void SerialNumberUI::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_NumLock: 
		qDebug() << "Key_NumLock";
		setExecutingMode(); //切换工作模式
		break;
	case Qt::Key_Slash: //斜杠/
		qDebug() << "Key_Slash";
		if (AutomaticMode) {
			cameraControler->takePicture(); //获取图像
			this->getSerialNum(); //调用OCR模块进行识别
			cameraControler->startCapture(); //相机开始获取帧
		}
		break;
	case Qt::Key_Asterisk: //星号*
		qDebug() << "Key_Asterisk"; break;
	case Qt::Key_Minus: //减号-
		qDebug() << "Key_Minus"; break;
	case Qt::Key_Plus: //加号+
		qDebug() << "Key_Plus"; break;
	case Qt::Key_Enter:
	case Qt::Key_Return:
		qDebug() << "Key_Enter";
		exitOrParseSerialNum(); //解析产品序号
		break;
	}
}

//切换工作模式
void SerialNumberUI::setExecutingMode()
{
	focusOnSerialNumBox = GetKeyState(VK_NUMLOCK) & 0x01;
	AutomaticMode = !focusOnSerialNumBox && cameraControler->isCameraOpened();
	if (AutomaticMode) {
		//OCR识别模式
		ui.lineEdit_serialNum->setEnabled(false);
		this->setFocus();
	}
	else { //手动输入模式
		ui.lineEdit_serialNum->setEnabled(true);
		ui.lineEdit_serialNum->setFocus();
	}
}


/****************** 相机 *******************/

//相机打开结束
void SerialNumberUI::on_openCameraFinished_camera(bool success)
{
	//弹窗报错
	if (!success) {
		ui.lineEdit_serialNum->setFocusPolicy(Qt::NoFocus);
		this->setFocusPolicy(Qt::NoFocus);
		statusCode = Camera_OpenCameraFailed;
		showMessageBox(MessageBoxType::Warning, statusCode);
	}

	//设置工作模式
	setExecutingMode();
}

//刷新界面上相机采集的帧
void SerialNumberUI::do_refreshFrame_camera()
{
	//刷新获取的帧
	QPixmap frameForShow = QPixmap::fromImage(frame->scaled(cameraLabelSize, Qt::KeepAspectRatio));
	ui.label_cameraFrame->setPixmap(frameForShow);
}

//相机拍照失败
void SerialNumberUI::on_captureFailed_camera()
{
	//暂时不在输入框中显示光标
	if (focusOnSerialNumBox) {
		//ui.lineEdit_serialNum->setEnabled(false);
		//this->setFocus();
	}

	//弹窗报错
	statusCode = Camera_CaptureFailed;
	showMessageBox(MessageBoxType::Warning, statusCode);

	//设置工作模式，即产品序号的获取模式
	this->setExecutingMode();
}


/**************** 字符识别 *****************/

//调用OCR模块进行识别
void SerialNumberUI::getSerialNum()
{
	//如果光标在序号输入框上则返回
	if (!AutomaticMode) return;

	//更新状态
	ui.lineEdit_serialNum->setText(pcb::chinese("  正在识别产品序号 ..."));
	qApp->processEvents();
	statusCode = Unchecked;

	//clock_t t1 = clock();

	//加载图像
	string roiFilePath = roiImagePath.toStdString();
	const char *filePath = roiFilePath.c_str();
	PIX *img = pixRead(filePath);
	if (img == NULL) {
		statusCode = OCR_LoadRoiImageFailed;
		ui.lineEdit_serialNum->setText(pcb::chinese(""));
		showMessageBox(MessageBoxType::Warning, statusCode); return;
	}

	//字符识别
	TessBaseAPISetImage2(ocrHandle, img);
	if (TessBaseAPIRecognize(ocrHandle, NULL) != 0) {
		statusCode = OCR_RecognitionFailed;
		ui.lineEdit_serialNum->setText(pcb::chinese(""));
		showMessageBox(MessageBoxType::Warning, statusCode); return;
	}

	//识别产品序号字符的转换
	char *text = NULL;
	if ((text = TessBaseAPIGetUTF8Text(ocrHandle)) == NULL) {
		statusCode = OCR_GetUTF8TextFailed;
		ui.lineEdit_serialNum->setText(pcb::chinese(""));
		showMessageBox(MessageBoxType::Warning, statusCode); return;
	}

	//序号的预处理与显示
	QString serialNum = QString(text);
	serialNum = serialNum.remove(QRegExp("\\s")); //删除空白字符
	serialNum = pcb::eraseNonDigitalCharInHeadAndTail(serialNum);//删除首尾的非数字字符
	ui.lineEdit_serialNum->setText(serialNum); //显示识别的产品序号

	//clock_t t2 = clock();
	//qDebug() << "OCR:" << (t2 -t1) << "ms";

	//解析产品序号
	runtimeParams->serialNum = serialNum;
	RuntimeParams::ErrorCode code = RuntimeParams::Unchecked;
	code = runtimeParams->parseSerialNum(); //解析
	if (code != RuntimeParams::ValidValue) {
		statusCode = OCR_InvalidSerialNum;
		runtimeParams->showMessageBox(this);
		ui.lineEdit_serialNum->setText(pcb::chinese(""));
	}

	statusCode = NoError;
}

//退出系统或者解析产品序号
//注：敲击回车键时会调用此函数
void SerialNumberUI::exitOrParseSerialNum()
{
	//判断是否要退出程序
	QString serialNum = ui.lineEdit_serialNum->text();
	if (serialNum == QString("00000000")) {
		emit exitRecheckSystem_numUI(); return;
	}

	//解析产品序号
	runtimeParams->serialNum = serialNum;
	RuntimeParams::ErrorCode code = RuntimeParams::Unchecked;
	code = runtimeParams->parseSerialNum(); //解析
	if (code != RuntimeParams::ValidValue) { 
		statusCode = OCR_InvalidSerialNum;
		runtimeParams->showMessageBox(this);
	}
	else {
		statusCode = NoError;
		cameraControler->stopCapture(); //相机停止获取帧
		emit showRecheckMainUI_numUI();
	}
}


/************* PCB编号的自动更新 *************/

//更新编号
bool SerialNumberUI::getNextSerialNum()
{
	//QList<int> sampleImageList = (*folderHierarchy)[sampleModelNum->toInt()][sampleBatchNum->toInt()];
	//if (sampleImageList.size() < 2) return false;

	//for (int i = 0; i < sampleImageList.size(); i++) {
	//	if (sampleImageList[i] == sampleNum->toInt()) {
	//		if (i+1 < sampleImageList.size()) {
	//			*sampleNum = QString::number(sampleImageList[i+1]);
	//			*serialNum = (*serialNum).mid(0, serialNumSlice[1]+ serialNumSlice[2]) +
	//				QString("%1").arg(sampleNum->toInt(), serialNumSlice[0], 10, QLatin1Char('0'));
	//			ui.lineEdit_serialNum->setText(*serialNum); //更新输入框中的产品序号
	//			return true;
	//		}
	//	}
	//}
	//return false;
	return true;
}


/************* 其他 *************/

//设置界面是否处于激活状态
void SerialNumberUI::setActivated(bool activated)
{
	if (activated)
		cameraControler->startCapture(); //开始抓图
	else
		cameraControler->stopCapture(); //停止抓图
}

//弹窗报错
void SerialNumberUI::showMessageBox(MessageBoxType boxType, StatusCode code)
{
	StatusCode tempCode = (code == Default) ? statusCode : code;
	if (tempCode == NoError) return;

	//根据状态码生成需要显示的信息字符串
	QString message = "";
	switch (tempCode)
	{
	case SerialNumberUI::Unchecked:
		message = pcb::chinese("系统状态未知! \n");
		message += "SerialNum: ErrorCode: " + QString::number(tempCode); break;
	case SerialNumberUI::OCR_InitFailed:
		message = pcb::chinese("OCR模块初始化失败! \n");
		message += "SerialNum: ErrorCode: " + QString::number(tempCode); break;
	case SerialNumberUI::OCR_LoadRoiImageFailed:
		message = pcb::chinese("OCR缓存图像读取失败，请重新识别! \n");
		message += "SerialNum: OCR: ErrorCode: " + QString::number(tempCode); break;
	case SerialNumberUI::OCR_RecognitionFailed:
		message = pcb::chinese("产品序号识别失败! \n");
		message += "SerialNum: OCR: ErrorCode: " + QString::number(tempCode); break;
	case SerialNumberUI::OCR_GetUTF8TextFailed:
		message = pcb::chinese("产品序号转换失败! \n");
		message += "SerialNum: OCR: ErrorCode: " + QString::number(tempCode); break;
	case SerialNumberUI::OCR_InvalidSerialNum:
		message = pcb::chinese("产品序号无效，请重新识别或手动输入! \n");
		message += "SerialNum: OCR: ErrorCode: " + QString::number(tempCode); break;
	case SerialNumberUI::Camera_OpenCameraFailed:
		message = pcb::chinese("相机打开失败，请重启程序或使用手动输入模式!\n");
		message += "SerialNum: Camera: ErrorCode: " + QString::number(tempCode); break;
	case SerialNumberUI::Camera_CaptureFailed:
		message = pcb::chinese("图像截取失败，请重截取关键帧! \n");
		message += "SerialNum: Camera: ErrorCode: " + QString::number(tempCode); break;
	case SerialNumberUI::Default:
		message = pcb::chinese("未知错误!  \n");
		message += "SerialNum: ErrorCode: " + QString::number(tempCode); break; 
	}

	//显示窗口
	MyMessageBox messageBox;
	messageBox.set(boxType, message);
	messageBox.doShow();
}
