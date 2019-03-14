#include "SerialNumberUI.h"

SerialNumberUI::SerialNumberUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QPalette palette;
	palette.setColor(QPalette::Background, QColor(246, 246, 246));
	ui.label_background->setAutoFillBackground(true); //ȱ��ͼ
	ui.label_background->setPalette(palette);

	//ȥ��������
	this->setWindowFlags(Qt::FramelessWindowHint);

	//��������
	ui.lineEdit_serialNum->setValidator(&intValidator);

	//�˽�����ʾʱ������������
	this->setWindowModality(Qt::ApplicationModal);
}

SerialNumberUI::~SerialNumberUI()
{
}

/******************** ��ȡ���������ָ�� **********************/

//����PCB��ŵ�ָ��
void SerialNumberUI::setSerialNum(QString **ptrArray)
{
	serialNum = ptrArray[0]; //PCB���
	sampleModelNum = ptrArray[1]; //�������ͱ��
	sampleBatchNum = ptrArray[2]; //�������κ�
	sampleNum = ptrArray[3]; //�������
}


/******************** �¼���Ӧ���û��س��� ********************/

void SerialNumberUI::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		*serialNum = ui.lineEdit_serialNum->text();
		if(*serialNum == QString("00000000")){
			emit exitRecheckSystem_numUI();
		}
		else {
			parseSerialNum(); //�������
			emit showRecheckUI_numUI();
		}
	}
}

/******************** PCB��ŵĽ�������� ********************/

//��Ž�����
//��Ʒ���=PCB�ͺ�+���κ�+�������
void SerialNumberUI::parseSerialNum()
{
	/*
	*sampleTypeNumPtr = "1"; //�ͺ�
	*sampleBatchNumPtr = "1"; //���κ�
	*sampleNumPtr = "3"; //�������
	*/

	*sampleModelNum = QString::number(serialNum->mid(
		sampleTypeNumSlice[0], sampleTypeNumSlice[1]).toInt()); //�ͺ�
	*sampleBatchNum = QString::number(serialNum->mid(
		sampleBatchNumSlice[0], sampleBatchNumSlice[1]).toInt()); //���κ�
	*sampleNum = QString::number(serialNum->mid(
		sampleNumSlice[0], sampleNumSlice[1]).toInt()); //�������
}

//���±��
bool SerialNumberUI::getNextSerialNum()
{
	QList<int> sampleImageList = (*folderHierarchy)[sampleModelNum->toInt()][sampleBatchNum->toInt()];
	if (sampleImageList.size() < 2) return false;

	for (int i = 0; i < sampleImageList.size(); i++) {
		if (sampleImageList[i] == sampleNum->toInt()) {
			if (i+1 < sampleImageList.size()) {
				*sampleNum = QString::number(sampleImageList[i+1]);
				*serialNum = (*serialNum).mid(0, sampleTypeNumSlice[1]+ sampleBatchNumSlice[1]) +
					QString("%1").arg(sampleNum->toInt(), sampleNumSlice[1], 10, QLatin1Char('0'));
				ui.lineEdit_serialNum->setText(*serialNum); //����������еĲ�Ʒ���
				return true;
			}
		}
	}
	return false;
}
