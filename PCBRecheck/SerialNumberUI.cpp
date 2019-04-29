#include "SerialNumberUI.h"

using pcb::RuntimeParams;


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


/******************** �¼���Ӧ���û��س��� ********************/

void SerialNumberUI::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		QString serialNum = ui.lineEdit_serialNum->text();
		if(serialNum == QString("00000000")){
			emit exitRecheckSystem_numUI();
		}
		else {
			runtimeParams->serialNum = serialNum;
			RuntimeParams::ErrorCode code = RuntimeParams::Uncheck;
			code = runtimeParams->parseSerialNum();
			if (code != RuntimeParams::ValidValue) { //�������
				runtimeParams->showMessageBox(this);
			} 
			else {
				emit showRecheckUI_numUI();
			}
		}
	}
}

/******************** PCB��ŵ��Զ����� ********************/

//���±��
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
	//			ui.lineEdit_serialNum->setText(*serialNum); //����������еĲ�Ʒ���
	//			return true;
	//		}
	//	}
	//}
	//return false;
	return true;
}
