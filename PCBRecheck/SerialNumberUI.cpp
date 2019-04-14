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
			if (parseSerialNum()) { //�������
				emit showRecheckUI_numUI();
			}
		}
	}
}

/******************** PCB��ŵĽ�������� ********************/

//��Ž�����
//��Ʒ���=PCB�ͺ�+���κ�+�������
bool SerialNumberUI::parseSerialNum()
{
	if (serialNumSlice[0] != serialNum->size()) {
		QMessageBox::warning(this, QString::fromLocal8Bit("����"),
			QString::fromLocal8Bit("��Ʒ�����Ч�����������룡  "),
			QString::fromLocal8Bit("ȷ��"));
		return false;
	}

	int begin = 0;
	*sampleModelNum = QString::number(serialNum->mid(begin, serialNumSlice[1]).toInt()); //�ͺ�

	begin += serialNumSlice[1];
	*sampleBatchNum = QString::number(serialNum->mid(begin, serialNumSlice[2]).toInt()); //���κ�

	begin += serialNumSlice[2];
	*sampleNum = QString::number(serialNum->mid(begin, serialNumSlice[3]).toInt()); //�������

	return true;
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
				*serialNum = (*serialNum).mid(0, serialNumSlice[1]+ serialNumSlice[2]) +
					QString("%1").arg(sampleNum->toInt(), serialNumSlice[0], 10, QLatin1Char('0'));
				ui.lineEdit_serialNum->setText(*serialNum); //����������еĲ�Ʒ���
				return true;
			}
		}
	}
	return false;
}
