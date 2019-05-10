#include "RuntimeLib.h"

using pcb::RuntimeParams;


RuntimeParams::RuntimeParams()
{
	errorCode = Unchecked; 
	errorCode_SerialNum = Unchecked;
	serialNum = ""; //��Ʒ���
	sampleModelNum = ""; //�ͺ�
	sampleBatchNum = ""; //���κ�
	sampleNum = ""; //�������
	
	AppDirPath = QDir::currentPath(); //��������Ŀ¼
	BufferDirPath = AppDirPath + "/buffer"; //����Ŀ¼
	QDir bufferDir(BufferDirPath);
	if (!bufferDir.exists()) bufferDir.mkdir(BufferDirPath);
}

RuntimeParams::~RuntimeParams()
{
	qDebug() << "~RuntimeParams";
}

//����Ĭ�ϵ����в���
void RuntimeParams::loadDefaultValue()
{
	errorCode = Unchecked;
	errorCode_SerialNum = Unchecked;
	serialNum = ""; //�������
	sampleModelNum = ""; //�ͺ�
	sampleBatchNum = ""; //���κ�
	sampleNum = ""; //�������

	AppDirPath = QDir::currentPath(); //��������Ŀ¼
	BufferDirPath = AppDirPath + "/buffer"; //����Ŀ¼
	QDir bufferDir(BufferDirPath);
	if (!bufferDir.exists()) bufferDir.mkdir(BufferDirPath);
}

//������Ʒ���
RuntimeParams::ErrorCode RuntimeParams::parseSerialNum()
{
	if (this->checkValidity(Index_serialNum) != ValidValue) {
		return Invalid_serialNum;
	}

	int begin = 0;
	sampleModelNum = serialNum.mid(begin, serialNumSlice[1]); //�ͺ�
	sampleModelNum = QString::number(sampleModelNum.toInt());//ȥ������0

	begin += serialNumSlice[1];
	sampleBatchNum = serialNum.mid(begin, serialNumSlice[2]); //���κ�
	sampleBatchNum = QString::number(sampleBatchNum.toInt());

	begin += serialNumSlice[2];
	sampleNum = serialNum.mid(begin, serialNumSlice[3]); //�������
	sampleNum = QString::number(sampleNum.toInt());

	//��Ч�Լ��
	if (this->checkValidity(Index_All_SerialNum) != ValidValues) {
		return errorCode_SerialNum;
	}
	return ValidValues;
}

//���ݲ�Ʒ����������·��
QString RuntimeParams::getRelativeFolderPath()
{
	return QString("/" +sampleModelNum + "/" + sampleBatchNum + "/" + sampleNum);
}

//��������Ч��
RuntimeParams::ErrorCode RuntimeParams::checkValidity(ParamsIndex index)
{
	ErrorCode code = ErrorCode::Unchecked;
	switch (index)
	{
	case pcb::RuntimeParams::Index_All:
	case pcb::RuntimeParams::Index_All_SerialNum:
	case pcb::RuntimeParams::Index_serialNum: //��Ʒ���
		if (serialNum.size() != serialNumSlice[0] || serialNum.toDouble() == 0) {
			code = Invalid_serialNum;
		}
		if (code != Unchecked || index != Index_All || index != Index_All_SerialNum) break;
	case pcb::RuntimeParams::Index_sampleModelNum: //�ͺ�
		if (sampleModelNum == "" || sampleModelNum.size() > serialNumSlice[1]) {
			code = Invalid_sampleModelNum;
		}
		if (code != Unchecked || index != Index_All || index != Index_All_SerialNum) break;
	case pcb::RuntimeParams::Index_sampleBatchNum: //���κ�
		if (sampleBatchNum == "" || sampleBatchNum.size() > serialNumSlice[2]) {
			code = Invalid_sampleBatchNum;
		}
		if (code != Unchecked || index != Index_All || index != Index_All_SerialNum) break;
	case pcb::RuntimeParams::Index_sampleNum: //�������
		if (sampleNum == "" || sampleNum.size() > serialNumSlice[3]) {
			code = Invalid_sampleNum;
		}
		if (code != Unchecked || index != Index_All) break;
	}

	//����ֵ����Uncheck��ʾ���Ĳ�����Ч
	if (code == Unchecked) code = ValidParams;

	//���´������
	if (code != ValidParams || index == Index_All) errorCode = code;
	if (index == Index_All_SerialNum ||
		(code != ValidParams && index >= Index_serialNum && index <= Index_sampleNum))
	{
		errorCode_SerialNum = code;
	}
	return code;
}

//�ж����в������Ƿ���Ч
bool RuntimeParams::isValid(ParamsIndex index, bool doCheck)
{
	if (errorCode == ValidParams) return true;

	//���в���
	if (index == Index_All) {
		if (doCheck && errorCode == Unchecked)
			checkValidity(index);
		return (errorCode == ValidParams);
	}

	//��Ʒ�����ز���
	if (index == Index_All_SerialNum) {
		if (doCheck && errorCode_SerialNum == Unchecked)
			checkValidity(index);
		return (errorCode_SerialNum == ValidValues);
	}

	return false;
}

//������ʾ
void RuntimeParams::showMessageBox(QWidget *parent, ErrorCode code)
{
	ErrorCode tempCode = (code == Default) ? errorCode : code;
	if (tempCode == RuntimeParams::ValidParams) return;

	QString valueName = "";
	switch (tempCode)
	{
	case pcb::RuntimeParams::Unchecked:
		valueName = pcb::chinese("����δ��֤"); break;
	case pcb::RuntimeParams::Invalid_serialNum:
	case pcb::RuntimeParams::Invalid_sampleModelNum:
	case pcb::RuntimeParams::Invalid_sampleBatchNum:
	case pcb::RuntimeParams::Invalid_sampleNum:
		valueName = pcb::chinese("��Ʒ���"); break;
	case pcb::RuntimeParams::Default:
		valueName = "-"; break;
	}

	QString message = pcb::chinese("���в�����Ч�������Ʒ��Ż�ͼ�����ݣ� \n") +
		pcb::chinese("������Ϣ��") + valueName + "   \n" +
		"Config: User: ErrorCode: " + QString::number(tempCode);

	//��ʾ����
	MyMessageBox messageBox;
	messageBox.set(pcb::MessageBoxType::Warning, message);
	messageBox.doShow();
	return;
}



/****************************************************/
/*                   namespace pcb                  */
/****************************************************/

//�������ӳ�
void pcb::delay(unsigned long msec)
{
	QTime dieTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//����ʽ�ļ���·��ѡ��
QString pcb::selectDirPath(QWidget *parent, QString windowTitle)
{
	if (windowTitle == "") windowTitle = chinese("��ѡ��·��");

	QFileDialog *fileDialog = new QFileDialog(parent);
	fileDialog->setWindowTitle(windowTitle); //�����ļ�����Ի���ı���
	fileDialog->setFileMode(QFileDialog::Directory); //�����ļ��Ի��򵯳���ʱ����ʾ�ļ���
	fileDialog->setViewMode(QFileDialog::Detail); //�ļ�����ϸ����ʽ��ʾ����ʾ�ļ�������С���������ڵ���Ϣ

	QString path = "";
	if (fileDialog->exec() == QDialog::DialogCode::Accepted) //ѡ��·��
		path = fileDialog->selectedFiles()[0];
	delete fileDialog;
	return path;
}

//ɾ���ַ�����β�ķ������ַ�
QString pcb::eraseNonDigitalCharInHeadAndTail(QString s)
{
	if (s == "") return "";
	int begin = 0;
	for (; begin < s.size() && !s.at(begin).isDigit(); begin++) {}
	int end = s.size() - 1;
	for (; end > begin && !s.at(end).isDigit(); end--) {}
	return s.mid(begin, end - begin + 1);
}