#include "RuntimeLib.h"

using pcb::RuntimeParams;


RuntimeParams::RuntimeParams()
{
	errorCode = ErrorCode::Uncheck; //�������
	serialNum = ""; //��Ʒ���
	sampleModelNum = ""; //�ͺ�
	sampleBatchNum = ""; //���κ�
	sampleNum = ""; //�������
	AppDirPath = ""; //��������Ŀ¼
}

RuntimeParams::~RuntimeParams()
{
}

//����Ĭ�ϵ����в���
void RuntimeParams::loadDefaultValue()
{
	errorCode = Uncheck;
	serialNum = ""; //�������
	sampleModelNum = ""; //�ͺ�
	sampleBatchNum = ""; //���κ�
	sampleNum = ""; //�������
	AppDirPath = QDir::currentPath(); //��������Ŀ¼
}

//������Ʒ���
RuntimeParams::ErrorCode RuntimeParams::parseSerialNum()
{
	if (this->checkValidity() != ValidValue) {
		return ErrorCode::Invalid_serialNum;
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

	return ErrorCode::ValidParams;
}

//��������Ч��
RuntimeParams::ErrorCode RuntimeParams::checkValidity(ParamsIndex index)
{
	ErrorCode code = ErrorCode::Uncheck;
	switch (index)
	{
	case pcb::RuntimeParams::Index_All:
	case pcb::RuntimeParams::Index_serialNum: //��Ʒ���
		if (serialNum.size() != serialNumSlice[0] || serialNum.toDouble() == 0) {
			code = Invalid_serialNum;
		}
		if (code != Uncheck || index != Index_All) break;
	case pcb::RuntimeParams::Index_sampleModelNum: //�ͺ�
		if (sampleModelNum == "" || sampleModelNum.size() > serialNumSlice[1]) {
			code = Invalid_sampleModelNum;
		}
		if (code != Uncheck || index != Index_All) break;
	case pcb::RuntimeParams::Index_sampleBatchNum: //���κ�
		if (sampleBatchNum == "" || sampleBatchNum.size() > serialNumSlice[2]) {
			code = Invalid_sampleBatchNum;
		}
		if (code != Uncheck || index != Index_All) break;
	case pcb::RuntimeParams::Index_sampleNum: //�������
		if (sampleNum == "" || sampleNum.size() > serialNumSlice[3]) {
			code = Invalid_sampleNum;
		}
		if (code != Uncheck || index != Index_All) break;
	}

	//����ֵ����Uncheck��ʾ���Ĳ�����Ч
	if (code == Uncheck) code = ValidParams;
	//���´������
	if (code != ValidParams || index == Index_All) errorCode = code;
	return code;
}

//�ж����в������Ƿ���Ч
bool RuntimeParams::isValid(bool doCheck)
{
	if (errorCode == ValidParams) return true;

	if (doCheck && errorCode == RuntimeParams::Uncheck)
		this->checkValidity(Index_All);
	return (errorCode == ValidParams);
}

//������ʾ
void RuntimeParams::showMessageBox(QWidget *parent, ErrorCode code)
{
	ErrorCode tempCode = (code == Default) ? errorCode : code;
	if (tempCode == RuntimeParams::ValidParams) return;

	QString valueName = "";
	switch (tempCode)
	{
	case pcb::RuntimeParams::Uncheck:
		valueName = pcb::chinese("\"����δ��֤\""); break;
	case pcb::RuntimeParams::Invalid_serialNum:
	case pcb::RuntimeParams::Invalid_sampleModelNum:
	case pcb::RuntimeParams::Invalid_sampleBatchNum:
	case pcb::RuntimeParams::Invalid_sampleNum:
		valueName = pcb::chinese("\"��Ʒ���\""); break;
	case pcb::RuntimeParams::Default:
		valueName = "\"-\""; break;
	}

	QMessageBox::warning(parent, pcb::chinese("����"),
		pcb::chinese("���в�����Ч�������Ʒ��Ż�ͼ�������Ƿ���Ч��  \n") +
		pcb::chinese("������Ϣ��") + valueName + "   \n" +
		"Config: User: ErrorCode: " + QString::number(tempCode),
		pcb::chinese("ȷ��"));
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
