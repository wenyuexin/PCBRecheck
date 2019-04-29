#include "SerialNumberUI.h"

using pcb::RuntimeParams;


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


/******************** 事件响应：敲击回车键 ********************/

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
			if (code != RuntimeParams::ValidValue) { //解析编号
				runtimeParams->showMessageBox(this);
			} 
			else {
				emit showRecheckUI_numUI();
			}
		}
	}
}

/******************** PCB编号的自动更新 ********************/

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
