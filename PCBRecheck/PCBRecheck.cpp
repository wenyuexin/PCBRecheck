#include "PCBRecheck.h"


//������湹�캯��
PCBRecheck::PCBRecheck(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//�жϲ�ѡ�������������Ǹ�������ʾ
	QDesktopWidget* desktop = QApplication::desktop();
	QRect screenRect = desktop->screenGeometry(1);//��������
	if (screenRect.width() < 1440 || screenRect.height() < 900) {
		screenRect = desktop->screenGeometry(0);//��������
	}
	this->setGeometry(screenRect);

	//ȫ����ʾ
	if (true && !(this->isFullScreen())) {
		this->setWindowFlags(Qt::SubWindow);
		this->showFullScreen();
	}

	//���޽����ʼ��
	initRecheckMainUI();

	//ϵͳ��ʼ���߳�
	sysInitThread = new SysInitThread;
	sysInitThread->setOutFolderHierarchy(&OutFolderHierarchy);
	sysInitThread->setRecheckConfig(&userConfig);
	sysInitThread->start(); //�����߳�
	connect(sysInitThread, SIGNAL(sysInitFinished_initThread()), this, SLOT(on_sysInitFinished_initThread()));
	connect(sysInitThread, SIGNAL(userConfigError_initThread()), this, SLOT(on_userConfigError_initThread()));
	connect(sysInitThread, SIGNAL(outFolderHierarchyError_initThread()), this, SLOT(on_outFolderHierarchyError_initThread()));

	//������ý���
	serialNumberUI = new SerialNumberUI;
	serialNumberUI->setRuntimeParams(&runtimeParams);
	connect(serialNumberUI, SIGNAL(showRecheckMainUI_numUI()), this, SLOT(do_showRecheckMainUI_numUI()));
	connect(serialNumberUI, SIGNAL(exitRecheckSystem_numUI()), this, SLOT(do_exitRecheckSystem_numUI()));
	
	//�˳�ѯ�ʽ���
	exitQueryUI = new ExitQueryUI;
	connect(exitQueryUI, SIGNAL(showSerialNumberUI_exitUI()), this, SLOT(do_showSerialNumberUI_exitUI()));
	connect(exitQueryUI, SIGNAL(showRecheckUI_exitUI()), this, SLOT(do_showRecheckUI_exitUI()));

	//��ʱ��
	timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(on_timeOut()));
}

//���������������
PCBRecheck::~PCBRecheck()
{
	delete sysInitThread;
	delete serialNumberUI;
	delete exitQueryUI;
	delete timer;
}

/********************* ���޽����ʼ�� *********************/

void PCBRecheck::initRecheckMainUI()
{
	//����logo
	IconFolder = QDir::currentPath() + "/Icons";
	QImage image(IconFolder + "/logo.png");
	ui.label_logo->setPixmap(QPixmap::fromImage(image.scaled(ui.label_logo->size(), Qt::KeepAspectRatio)));
	qDebug() << IconFolder;

	//��������ͼ��
	QImage redIcon = QImage(IconFolder + "/red.png"); //red
	lightOnIcon = QPixmap::fromImage(redIcon.scaled(ui.label_indicator1->size(), Qt::KeepAspectRatio));
	QImage greyIcon = QImage(IconFolder + "/grey.png"); //grey
	lightOffIcon = QPixmap::fromImage(greyIcon.scaled(ui.label_indicator1->size(), Qt::KeepAspectRatio));

	//���õ�ǰ������ʾ��ȱ��Сͼ����ȱ������ָʾ��
	defectIndex = -1; //default
	ui.label_indicator1->setPixmap(lightOffIcon); //A
	ui.label_indicator2->setPixmap(lightOffIcon); //B
	ui.label_indicator3->setPixmap(lightOffIcon); //C
	ui.label_indicator4->setPixmap(lightOffIcon); //D

	//�ı���ȵı���ɫ
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(250, 250, 250));
	ui.label_flaw->setAutoFillBackground(true); //ȱ��ͼ
	ui.label_flaw->setPalette(palette);
	ui.label_xLoc->setAutoFillBackground(true); //ȱ��λ��x
	ui.label_xLoc->setPalette(palette);
	ui.label_yLoc->setAutoFillBackground(true); //ȱ��λ��y
	ui.label_yLoc->setPalette(palette);
	ui.label_serialNum->setAutoFillBackground(true); //�������
	ui.label_serialNum->setPalette(palette);
}

/******************* �ۺ�������ʼ������ *******************/

//���û�������Ч
void PCBRecheck::on_userConfigError_initThread()
{
	userConfig.showMessageBox(this); //������ʾ
	pcb::delay(10); //��ʱ
	exitRecheckSystem(); //�˳�ϵͳ
}


void PCBRecheck::on_outFolderHierarchyError_initThread()
{
	QMessageBox::warning(Q_NULLPTR, QString::fromLocal8Bit("����"),
		QString::fromLocal8Bit("�޴��������������Ƚ��м��!  "), 
		QString::fromLocal8Bit("ȷ��"));
	exitRecheckSystem(); //�˳�ϵͳ
}

//ϵͳ��ʼ������
void PCBRecheck::on_sysInitFinished_initThread()
{
	//�����ʼ���̷߳��صĲ���
	//OutputDirPath = userConfig.OutputDirPath;
	//TemplDirPath = userConfig.TemplDirPath;
	//ImageFormat = userConfig.ImageFormat;

	//�����Ŀ¼�µ��ļ��в�δ��ݸ�������ý���
	serialNumberUI->setFolderHierarchy(&OutFolderHierarchy);

	//��ʾPCB���ѯ�ʽ���
	showSerialNumberUI();
}

/************************ ������ý��� ***********************/

//��������ý��淵�أ�����ʾ����������
void PCBRecheck::do_showRecheckMainUI_numUI()
{
	serialNumberUI->hide(); //����PCB������ý���
	ui.pushButton_plus2->setEnabled(true);
	ui.pushButton_minus2->setEnabled(true);
	ui.pushButton_exit2->setEnabled(true);
	refreshRecheckUI(); //���½�������ʾ����Ϣ
}

//�ɲ������ý��淵�أ����˳�����ϵͳ
void PCBRecheck::do_exitRecheckSystem_numUI()
{
	exitRecheckSystem();
}


/************************ �˳�ѯ�ʽ��� ***********************/

//�����˳�ѯ�ʽ��棬����ʾ������ý���
void PCBRecheck::do_showSerialNumberUI_exitUI()
{
	exitQueryUI->hide();  //�����˳�ѯ�ʽ���
	Sleep(10); //˯��10ms
	
	if (!serialNumberUI->getNextSerialNum()) { //��ȡͬ���ε���һ����Ʒ���
		//��ʾ�������Ѿ�������
		QMessageBox::warning(this, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("�����ε����������Ѿ��������!  "), 
			QString::fromLocal8Bit("ȷ��"));
	}
	//��ʾ������ý���
	showSerialNumberUI(); 
}

//�˳�ϵͳ
void PCBRecheck::do_showRecheckUI_exitUI()
{
	exitQueryUI->hide();
	ui.pushButton_plus2->setEnabled(true);
	ui.pushButton_minus2->setEnabled(true);
	ui.pushButton_exit2->setEnabled(true);
}


/********* ��ʼ��ʾ������PCB��ͼ����1��ȱ��Сͼ�� ********/

//ˢ�¼�ʱ����С��ͷ
void PCBRecheck::on_timeOut()
{
	flickeringArrow.update(-100, -100, 200, 200);
	timer->start(500);
}

//���½�������ʾ����Ϣ
void PCBRecheck::refreshRecheckUI()
{
	/*���½����е�PCB���*/
	QFont font("SimSun-ExtB", 16, 0); //���壬�����С���Ӵ�Ȩ�� 
	ui.label_serialNum->setFont(font); //��������
	ui.label_serialNum->setText(runtimeParams.serialNum); //����pcb���

	QString flawImageFolder = userConfig.OutputDirPath + "/"
		+ runtimeParams.sampleModelNum + "/" + runtimeParams.sampleBatchNum + "/" 
		+ runtimeParams.sampleNum;//��������ڵ��ļ���
	if (!QFileInfo(flawImageFolder).isDir()) {
		QMessageBox::warning(this, QString::fromLocal8Bit("����"),
			QString::fromLocal8Bit("·����λʧ�ܣ��޷���ȡ��Ӧ�ļ����!\n") 
			+ QString("path: ./output/" + runtimeParams.sampleModelNum + "/" 
				+ runtimeParams.sampleBatchNum + "/" + runtimeParams.sampleNum),
			QString::fromLocal8Bit("ȷ��"));
		Sleep(10);
		showSerialNumberUI(); //��ʾPCB���ѯ�ʽ���
		return;
	}
	else {
		getFlawImgInfo(flawImageFolder); //��ȡ�ļ����ڵ�ͼƬ·��
	}

	//���ز���ʾPCB��ͼ
	defectIndex = 0;
	showFullImage();
	ui.label_defectNum->setText(QString::number(defectNum));

	/*���ز���ʾ��1��ȱ��Сͼ*/
	QFileInfo flawImgInfo(flawImagePathVec[0]);
	if (!flawImgInfo.isFile()) {
		QMessageBox::warning(this, QString::fromLocal8Bit("����"),
			QString::fromLocal8Bit("�޷��򿪶�Ӧ��ȱ��ͼ!  "));
	}
	else {
		showFlawImage(); //��ʾȱ��ͼ
	}
}

// ��ȡ��ǰ��Ŷ�Ӧ������ȱ��ͼƬ�����ݻ��浽�ڴ��У�
// �����л�ȱ��ͼʱ��ֻ��������ж�ȡ���ݼ���
void PCBRecheck::getFlawImgInfo(QString dirpath)
{
	QDir dir(dirpath);
	dir.setSorting(QDir::Name | QDir::Time | QDir::Reversed);
	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

	QStringList filters("*" + userConfig.ImageFormat); // ����ͼƬ�ĸ�ʽΪ bmp  ��bmp��ʽ��ͼƬ���˳��� 
	dir.setNameFilters(filters);

	QFileInfoList folder_list = dir.entryInfoList();
	flawImageInfoVec.clear();
	flawImagePathVec.clear();
	for (int i = 0; i < folder_list.size(); i++) {
		flawImagePathVec.push_back(folder_list.at(i).absoluteFilePath()); //ȱ��ͼ·��
		flawImageInfoVec.push_back(folder_list.at(i).baseName().split("_")); //ȱ��ͼ��Ϣ
	}
}


//���ز���ʾPCB��ͼ
//�Ӷ�Ӧ��output�ļ��е�fullImage���ļ����ж�ȡ��ͼ
void PCBRecheck::showFullImage()
{
	QString flawImageDirPath = userConfig.OutputDirPath + "/" 
		+ runtimeParams.sampleModelNum + "/"
		+ runtimeParams.sampleBatchNum + "/" 
		+ runtimeParams.sampleNum; //��������ڵ��ļ���

	QString fullImageDirPath = flawImageDirPath + "/fullImage/";
	QDir dir(fullImageDirPath);
	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
	QStringList filters("*" + userConfig.ImageFormat); //��ImgFormat��ʽ��ͼƬ���˳��� 
	dir.setNameFilters(filters);
	QFileInfoList folder_list = dir.entryInfoList(); //��ȡ�ļ��б�

	QString fullImagePath = "";
	QStringList pcbInfoList;//��ͼ��ԭͼ��С+ȱ������
	for (int i = 0; i < folder_list.size(); i++) {
		if (folder_list.at(i).baseName().startsWith(fullImageNamePrefix)) {
			fullImagePath = folder_list.at(i).absoluteFilePath();
			pcbInfoList = folder_list.at(i).baseName().split("_");
			break;
		}
	}

	if (fullImage.load(fullImagePath)) {
		//�����ļ�����ȡPCB��ͼ��ԭʼ�ߴ��ȱ������
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

		//ɾ��������֮ǰ���ص�Ԫ��
		QList<QGraphicsItem *> itemList = fullImageScene.items();
		for (int i = 0; i < itemList.size(); i++) {
			fullImageScene.removeItem(itemList[i]);  //��scene���Ƴ�
		}

		//����PCB��ͼ
		scaledFactor = min(qreal(ui.graphicsView_full->height() - 2) / fullImage.size().height(),
			qreal(ui.graphicsView_full->width() - 2) / fullImage.size().width()); //PCB��ͼ�ĳߴ�任����
		fullImage = fullImage.scaled(fullImage.size()*scaledFactor, Qt::KeepAspectRatio); //ͼ������
		fullImageSize = fullImage.size(); //PCB��ͼ��ʵ����ʾ�ߴ�
		fullImageScene.addPixmap(QPixmap::fromImage(fullImage)); //��ͼ����ؽ�������
		fullImageScene.setSceneRect(0, 0, fullImageSize.width(), fullImageSize.height()); //���ó�����Χ

		//������˸�ļ�ͷ
		timer->start(500); //������ʱ��
		setFlickeringArrowPos(); //������˸��ͷ��λ��
		flickeringArrow.setFullImageSize(&fullImageSize);
		fullImageScene.addItem(&flickeringArrow); //����ͷ���ؽ�������

		//�ؼ�������ͼ����ʾ
		ui.graphicsView_full->setFocusPolicy(Qt::NoFocus);
		ui.graphicsView_full->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //����ˮƽ������
		ui.graphicsView_full->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //���ô�ֱ������
		ui.graphicsView_full->setScene(&fullImageScene); //���ó���
		ui.graphicsView_full->show(); //��ʾͼ��
	}
	else {
		QMessageBox::warning(this, QString::fromLocal8Bit("����"),
			QString::fromLocal8Bit("�޷�������Ӧ��PCB��ͼ!  "),
			QString::fromLocal8Bit("ȷ��"));
		pcb::delay(10); //�ӳ�
		serialNumberUI->show(); //��ʾPCB���ѯ�ʽ���
	}
}

//������˸��ͷ��λ��
void PCBRecheck::setFlickeringArrowPos()
{
	qreal xLoc = flawImageInfoVec[defectIndex][1].toDouble();
	xLoc *= (1.0*fullImageSize.width()/originalFullImageSize.width());
	qreal yLoc = flawImageInfoVec[defectIndex][2].toDouble();
	yLoc *= (1.0*fullImageSize.height()/originalFullImageSize.height());
	flickeringArrow.setPos(xLoc, yLoc); //���ü�ͷ��λ��
}

/***************** �¼���Ӧ���л�ȱ��Сͼ ****************/

//�û�С���̵� + -
void PCBRecheck::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) 
	{	
	case Qt::Key_Plus: //�л�����ʾ��һ��ȱ��
		qDebug() << "plus";
		showNextFlawImage();
		break;

	case Qt::Key_Minus: //�л�����ʾ��һ��ȱ��
		qDebug() << "minus";
		showLastFlawImage();
		break;

	default:
		break;
	}
}

//����ӺŰ��� - �л�����ʾ��һ��ȱ��
void PCBRecheck::on_pushButton_plus2_clicked()
{
	qDebug() << "plus2";
	showNextFlawImage();
}

//������Ű��� - �л�����ʾ��һ��ȱ��
void PCBRecheck::on_pushButton_minus2_clicked()
{
	qDebug() << "minus2";
	showLastFlawImage();
}


/**************** ����ʵ�֣��л�ȱ��Сͼ *****************/

//�л�����ʾ��һ��ȱ��ͼ
void PCBRecheck::showLastFlawImage()
{
	//��ȡ ��ǰ�� index �ж��Ƿ� ����߽�
	defectIndex -= 1;
	if (defectIndex < 0) {  //�߽�
		defectIndex += 1;
		qDebug() << "this is the first one";
		showExitQueryUI(); //��ʾ�˳�ѯ�ʿ�
	}
	else {
		showFlawImage();
	}
}

//�л�����ʾ��һ��ȱ��ͼ
void PCBRecheck::showNextFlawImage()
{
	//��ȡ ��ǰ�� index �ж��Ƿ� ����߽�
	defectIndex += 1;
	if (defectIndex > flawImagePathVec.size() - 1) {  //�߽�
		defectIndex -= 1;
		qDebug() << "this is the last one";
		showExitQueryUI(); //��ʾ�˳�ѯ�ʿ�
	}
	else {
		showFlawImage();
	}
}

//��ȱ��ͼ���ز���ʾ����Ӧ��lebal�ؼ���
void PCBRecheck::showFlawImage()
{
	QImage flawImage(flawImagePathVec[defectIndex]); //ȱ��ͼ
	ui.label_flaw->setPixmap(QPixmap::fromImage(flawImage.scaled(ui.label_flaw->size(), Qt::KeepAspectRatio))); //��ʾͼ��
	ui.label_xLoc->setText(flawImageInfoVec[defectIndex][1]); //����ȱ�ݵ�x����
	ui.label_yLoc->setText(flawImageInfoVec[defectIndex][2]); //����ȱ�ݵ�y����
	switchFlawIndicator(); //����ȱ������ͼ��(�޸�ָʾ������״̬)

	//����PCB��ͼ�ϵ�С��ͷ��λ��
	setFlickeringArrowPos();
}

//����ȱ�����͵�ָʾͼ��
void PCBRecheck::switchFlawIndicator()
{
	flawIndicatorStatus = pow(2, flawImageInfoVec[defectIndex][3].toInt() - 1);
	ui.label_indicator1->setPixmap(bool((flawIndicatorStatus & 0x1) >> 0) ? lightOnIcon : lightOffIcon); //A
	ui.label_indicator2->setPixmap(bool((flawIndicatorStatus & 0x2) >> 1) ? lightOnIcon : lightOffIcon); //B
	ui.label_indicator3->setPixmap(bool((flawIndicatorStatus & 0x4) >> 2) ? lightOnIcon : lightOffIcon); //C
	ui.label_indicator4->setPixmap(bool((flawIndicatorStatus & 0x8) >> 3) ? lightOnIcon : lightOffIcon); //D
}


/**************** �¼���Ӧ������˳����� *****************/

void PCBRecheck::on_pushButton_exit2_clicked()
{
	exitRecheckSystem();
}


/******************** ������� *********************/

void PCBRecheck::showSerialNumberUI()
{
	ui.pushButton_plus2->setEnabled(false);
	ui.pushButton_minus2->setEnabled(false);
	ui.pushButton_exit2->setEnabled(false);
	this->serialNumberUI->show(); //�����˳�ѯ�ʿ�
}

void PCBRecheck::showExitQueryUI()
{
	ui.pushButton_plus2->setEnabled(false);
	ui.pushButton_minus2->setEnabled(false);
	ui.pushButton_exit2->setEnabled(false);
	this->exitQueryUI->show(); //�����˳�ѯ�ʿ�
}

void PCBRecheck::exitRecheckSystem()
{
	this->close();
	qApp->exit(0);
}
