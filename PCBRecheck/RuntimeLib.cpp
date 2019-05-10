#include "RuntimeLib.h"

using pcb::RuntimeParams;


RuntimeParams::RuntimeParams()
{
	errorCode = Unchecked; 
	errorCode_SerialNum = Unchecked;
	serialNum = ""; //产品序号
	sampleModelNum = ""; //型号
	sampleBatchNum = ""; //批次号
	sampleNum = ""; //样本编号
	
	AppDirPath = QDir::currentPath(); //程序所在目录
	BufferDirPath = AppDirPath + "/buffer"; //缓存目录
	QDir bufferDir(BufferDirPath);
	if (!bufferDir.exists()) bufferDir.mkdir(BufferDirPath);
}

RuntimeParams::~RuntimeParams()
{
	qDebug() << "~RuntimeParams";
}

//加载默认的运行参数
void RuntimeParams::loadDefaultValue()
{
	errorCode = Unchecked;
	errorCode_SerialNum = Unchecked;
	serialNum = ""; //样本编号
	sampleModelNum = ""; //型号
	sampleBatchNum = ""; //批次号
	sampleNum = ""; //样本编号

	AppDirPath = QDir::currentPath(); //程序所在目录
	BufferDirPath = AppDirPath + "/buffer"; //缓存目录
	QDir bufferDir(BufferDirPath);
	if (!bufferDir.exists()) bufferDir.mkdir(BufferDirPath);
}

//解析产品序号
RuntimeParams::ErrorCode RuntimeParams::parseSerialNum()
{
	if (this->checkValidity(Index_serialNum) != ValidValue) {
		return Invalid_serialNum;
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

	//有效性检查
	if (this->checkValidity(Index_All_SerialNum) != ValidValues) {
		return errorCode_SerialNum;
	}
	return ValidValues;
}

//根据产品序号生成相对路径
QString RuntimeParams::getRelativeFolderPath()
{
	return QString("/" +sampleModelNum + "/" + sampleBatchNum + "/" + sampleNum);
}

//检查参数有效性
RuntimeParams::ErrorCode RuntimeParams::checkValidity(ParamsIndex index)
{
	ErrorCode code = ErrorCode::Unchecked;
	switch (index)
	{
	case pcb::RuntimeParams::Index_All:
	case pcb::RuntimeParams::Index_All_SerialNum:
	case pcb::RuntimeParams::Index_serialNum: //产品序号
		if (serialNum.size() != serialNumSlice[0] || serialNum.toDouble() == 0) {
			code = Invalid_serialNum;
		}
		if (code != Unchecked || index != Index_All || index != Index_All_SerialNum) break;
	case pcb::RuntimeParams::Index_sampleModelNum: //型号
		if (sampleModelNum == "" || sampleModelNum.size() > serialNumSlice[1]) {
			code = Invalid_sampleModelNum;
		}
		if (code != Unchecked || index != Index_All || index != Index_All_SerialNum) break;
	case pcb::RuntimeParams::Index_sampleBatchNum: //批次号
		if (sampleBatchNum == "" || sampleBatchNum.size() > serialNumSlice[2]) {
			code = Invalid_sampleBatchNum;
		}
		if (code != Unchecked || index != Index_All || index != Index_All_SerialNum) break;
	case pcb::RuntimeParams::Index_sampleNum: //样本编号
		if (sampleNum == "" || sampleNum.size() > serialNumSlice[3]) {
			code = Invalid_sampleNum;
		}
		if (code != Unchecked || index != Index_All) break;
	}

	//代码值等于Uncheck表示检测的参数有效
	if (code == Unchecked) code = ValidParams;

	//更新错误代码
	if (code != ValidParams || index == Index_All) errorCode = code;
	if (index == Index_All_SerialNum ||
		(code != ValidParams && index >= Index_serialNum && index <= Index_sampleNum))
	{
		errorCode_SerialNum = code;
	}
	return code;
}

//判断运行参数类是否有效
bool RuntimeParams::isValid(ParamsIndex index, bool doCheck)
{
	if (errorCode == ValidParams) return true;

	//所有参数
	if (index == Index_All) {
		if (doCheck && errorCode == Unchecked)
			checkValidity(index);
		return (errorCode == ValidParams);
	}

	//产品序号相关参数
	if (index == Index_All_SerialNum) {
		if (doCheck && errorCode_SerialNum == Unchecked)
			checkValidity(index);
		return (errorCode_SerialNum == ValidValues);
	}

	return false;
}

//弹窗提示
void RuntimeParams::showMessageBox(QWidget *parent, ErrorCode code)
{
	ErrorCode tempCode = (code == Default) ? errorCode : code;
	if (tempCode == RuntimeParams::ValidParams) return;

	QString valueName = "";
	switch (tempCode)
	{
	case pcb::RuntimeParams::Unchecked:
		valueName = pcb::chinese("参数未验证"); break;
	case pcb::RuntimeParams::Invalid_serialNum:
	case pcb::RuntimeParams::Invalid_sampleModelNum:
	case pcb::RuntimeParams::Invalid_sampleBatchNum:
	case pcb::RuntimeParams::Invalid_sampleNum:
		valueName = pcb::chinese("产品序号"); break;
	case pcb::RuntimeParams::Default:
		valueName = "-"; break;
	}

	QString message = pcb::chinese("运行参数无效，请检查产品序号或图像数据！ \n") +
		pcb::chinese("错误信息：") + valueName + "   \n" +
		"Config: User: ErrorCode: " + QString::number(tempCode);

	//显示窗口
	MyMessageBox messageBox;
	messageBox.set(pcb::MessageBoxType::Warning, message);
	messageBox.doShow();
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

//删除字符串首尾的非数字字符
QString pcb::eraseNonDigitalCharInHeadAndTail(QString s)
{
	if (s == "") return "";
	int begin = 0;
	for (; begin < s.size() && !s.at(begin).isDigit(); begin++) {}
	int end = s.size() - 1;
	for (; end > begin && !s.at(end).isDigit(); end--) {}
	return s.mid(begin, end - begin + 1);
}