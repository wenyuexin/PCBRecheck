#include "SerialNumberUI.h"

SerialNumberUI::SerialNumberUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QPalette palette;
	palette.setColor(QPalette::Background, QColor(246, 246, 246));
	ui.label_background->setAutoFillBackground(true); //缺陷图
	ui.label_background->setPalette(palette);

	//去掉标题栏
	this->setWindowFlags(Qt::FramelessWindowHint);

	//限制输入
	ui.lineEdit_serialNum->setValidator(&intValidator);

	//此界面显示时禁用其他窗口
	this->setWindowModality(Qt::ApplicationModal);
}

SerialNumberUI::~SerialNumberUI()
{
}

/******************** 获取输入参数的指针 **********************/

//传递PCB编号的指针
void SerialNumberUI::setSerialNum(QString **ptrArray)
{
	serialNum = ptrArray[0]; //PCB编号
	sampleModelNum = ptrArray[1]; //样本类型编号
	sampleBatchNum = ptrArray[2]; //样本批次号
	sampleNum = ptrArray[3]; //样本编号
}


/******************** 事件响应：敲击回车键 ********************/

void SerialNumberUI::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		*serialNum = ui.lineEdit_serialNum->text();
		if(*serialNum == QString("00000000")){
			emit exitRecheckSystem_numUI();
		}
		else {
			if (parseSerialNum()) { //解析编号
				emit showRecheckUI_numUI();
			}
		}
	}
}

/******************** PCB编号的解析与更新 ********************/

//编号解析：
//产品序号=PCB型号+批次号+样本编号
bool SerialNumberUI::parseSerialNum()
{
	if (serialNumSlice[0] != serialNum->size()) {
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"),
			QString::fromLocal8Bit("产品序号无效，请重新输入！  "),
			QString::fromLocal8Bit("确定"));
		return false;
	}

	int begin = 0;
	*sampleModelNum = QString::number(serialNum->mid(begin, serialNumSlice[1]).toInt()); //型号

	begin += serialNumSlice[1];
	*sampleBatchNum = QString::number(serialNum->mid(begin, serialNumSlice[2]).toInt()); //批次号

	begin += serialNumSlice[2];
	*sampleNum = QString::number(serialNum->mid(begin, serialNumSlice[3]).toInt()); //样本编号

	return true;
}

//更新编号
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
				ui.lineEdit_serialNum->setText(*serialNum); //更新输入框中的产品序号
				return true;
			}
		}
	}
	return false;
}
