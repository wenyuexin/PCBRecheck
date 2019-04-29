#include "RuntimeLib.h"

using pcb::RuntimeParams;


RuntimeParams::RuntimeParams()
{
	errorCode = ErrorCode::Uncheck; //错误代码
	serialNum = ""; //产品序号
	sampleModelNum = ""; //型号
	sampleBatchNum = ""; //批次号
	sampleNum = ""; //样本编号
	AppDirPath = ""; //程序所在目录
}

RuntimeParams::~RuntimeParams()
{
}

//加载默认的运行参数
void RuntimeParams::loadDefaultValue()
{
	errorCode = Uncheck;
	serialNum = ""; //样本编号
	sampleModelNum = ""; //型号
	sampleBatchNum = ""; //批次号
	sampleNum = ""; //样本编号
	AppDirPath = QDir::currentPath(); //程序所在目录
}

//解析产品序号
RuntimeParams::ErrorCode RuntimeParams::parseSerialNum()
{
	if (this->checkValidity() != ValidValue) {
		return ErrorCode::Invalid_serialNum;
	}

	int begin = 0;
	sampleModelNum = serialNum.mid(begin, serialNumSlice[1]); //型号
	sampleModelNum = QString::number(sampleModelNum.toInt());//去除数字0

	begin += serialNumSlice[1];
	sampleBatchNum = serialNum.mid(begin, serialNumSlice[2]); //批次号
	sampleBatchNum = QString::number(sampleBatchNum.toInt());

	begin += serialNumSlice[2];
	sampleNum = serialNum.mid(begin, serialNumSlice[3]); //样本编号
	sampleNum = QString::number(sampleNum.toInt());

	return ErrorCode::ValidParams;
}

//检查参数有效性
RuntimeParams::ErrorCode RuntimeParams::checkValidity(ParamsIndex index)
{
	ErrorCode code = ErrorCode::Uncheck;
	switch (index)
	{
	case pcb::RuntimeParams::Index_All:
	case pcb::RuntimeParams::Index_serialNum: //产品序号
		if (serialNum.size() != serialNumSlice[0] || serialNum.toDouble() == 0) {
			code = Invalid_serialNum;
		}
		if (code != Uncheck || index != Index_All) break;
	case pcb::RuntimeParams::Index_sampleModelNum: //型号
		if (sampleModelNum == "" || sampleModelNum.size() > serialNumSlice[1]) {
			code = Invalid_sampleModelNum;
		}
		if (code != Uncheck || index != Index_All) break;
	case pcb::RuntimeParams::Index_sampleBatchNum: //批次号
		if (sampleBatchNum == "" || sampleBatchNum.size() > serialNumSlice[2]) {
			code = Invalid_sampleBatchNum;
		}
		if (code != Uncheck || index != Index_All) break;
	case pcb::RuntimeParams::Index_sampleNum: //样本编号
		if (sampleNum == "" || sampleNum.size() > serialNumSlice[3]) {
			code = Invalid_sampleNum;
		}
		if (code != Uncheck || index != Index_All) break;
	}

	//代码值等于Uncheck表示检测的参数有效
	if (code == Uncheck) code = ValidParams;
	//更新错误代码
	if (code != ValidParams || index == Index_All) errorCode = code;
	return code;
}

//判断运行参数类是否有效
bool RuntimeParams::isValid(bool doCheck)
{
	if (errorCode == ValidParams) return true;

	if (doCheck && errorCode == RuntimeParams::Uncheck)
		this->checkValidity(Index_All);
	return (errorCode == ValidParams);
}

//弹窗提示
void RuntimeParams::showMessageBox(QWidget *parent, ErrorCode code)
{
	ErrorCode tempCode = (code == Default) ? errorCode : code;
	if (tempCode == RuntimeParams::ValidParams) return;

	QString valueName = "";
	switch (tempCode)
	{
	case pcb::RuntimeParams::Uncheck:
		valueName = pcb::chinese("\"参数未验证\""); break;
	case pcb::RuntimeParams::Invalid_serialNum:
	case pcb::RuntimeParams::Invalid_sampleModelNum:
	case pcb::RuntimeParams::Invalid_sampleBatchNum:
	case pcb::RuntimeParams::Invalid_sampleNum:
		valueName = pcb::chinese("\"产品序号\""); break;
	case pcb::RuntimeParams::Default:
		valueName = "\"-\""; break;
	}

	QMessageBox::warning(parent, pcb::chinese("警告"),
		pcb::chinese("运行参数无效，请检查产品序号或图像数据是否有效！  \n") +
		pcb::chinese("错误信息：") + valueName + "   \n" +
		"Config: User: ErrorCode: " + QString::number(tempCode),
		pcb::chinese("确定"));
	return;
}



/****************************************************/
/*                   namespace pcb                  */
/****************************************************/

//非阻塞延迟
void pcb::delay(unsigned long msec)
{
	QTime dieTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//交互式文件夹路径选择
QString pcb::selectDirPath(QWidget *parent, QString windowTitle)
{
	if (windowTitle == "") windowTitle = chinese("请选择路径");

	QFileDialog *fileDialog = new QFileDialog(parent);
	fileDialog->setWindowTitle(windowTitle); //设置文件保存对话框的标题
	fileDialog->setFileMode(QFileDialog::Directory); //设置文件对话框弹出的时候显示文件夹
	fileDialog->setViewMode(QFileDialog::Detail); //文件以详细的形式显示，显示文件名，大小，创建日期等信息

	QString path = "";
	if (fileDialog->exec() == QDialog::DialogCode::Accepted) //选择路径
		path = fileDialog->selectedFiles()[0];
	delete fileDialog;
	return path;
}
