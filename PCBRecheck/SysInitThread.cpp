#include "SysInitThread.h"

using pcb::UserConfig;
using pcb::Configurator;


SysInitThread::SysInitThread()
{
}

SysInitThread::~SysInitThread()
{
	qDebug() << "~SysInitThread";
}

//ִ���߳�
void SysInitThread::run()
{
	//ϵͳ��ʼ����������ȡ�����ļ�
	if (!initUserConfig()) return;

	//��ȡoutput�ļ����µ�Ŀ¼���
	if (!initOutputFolderHierarchy()) return;

	emit sysInitFinished_initThread();
}


/****************** ����ϵͳ��ʼ�� *******************/

//��ʼ���û�����
bool SysInitThread::initUserConfig()
{
	//emit sysInitStatus_initThread(pcb::chinese("���ڻ�ȡϵͳ����   "));

	if (!Configurator::loadConfigFile(".user.config", userConfig)) {
		emit userConfigError_initThread(); return false;
	}
	else {
		//������Ч���ж�
		UserConfig::ErrorCode code;//�������
		code = userConfig->checkValidity(UserConfig::Index_All);
		if (code != UserConfig::ValidConfig) {
			emit userConfigError_initThread(); return false;
		}
	}

	//emit sysInitStatus_initThread(pcb::chinese("ϵͳ������ȡ����   "));
	pcb::delay(800);
	return true;
}


/************************* ·����Ϣ��ʼ�� **************************/

bool SysInitThread::initOutputFolderHierarchy()
{
	//��ȡoutput�ļ����µ��ļ��в��
	if (!getOutputFolderInfo(userConfig->OutputDirPath)) {
		emit outFolderHierarchyError_initThread(); return false;
	}
	return true;
}


//��ȡoutput�ļ����µ��ļ���Ϣ
//�ڲ�����������Σ��ͺš����κš��������
bool SysInitThread::getOutputFolderInfo(QString dirpath)
{
	QDir outputDir(dirpath);
	outputDir.setSorting(QDir::Name | QDir::Time | QDir::Reversed);
	outputDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QFileInfoList modelFolderList = outputDir.entryInfoList();
	if (modelFolderList.isEmpty()) return true;

	OutputFolderHierarchy->clear();
	for (int i = 0; i < modelFolderList.size(); i++) {
		QString modelFolderPath = modelFolderList.at(i).absoluteFilePath();
		int modelIndex = modelFolderList.at(i).baseName().toInt();
		getModelFolderInfo(modelIndex, modelFolderPath);
	}
	return true;
}


//��ȡĳ�ͺ��ļ����µ��ļ���Ϣ
void SysInitThread::getModelFolderInfo(int modelIndex, QString &modelFolderPath)
{
	QDir typeDir(modelFolderPath);
	typeDir.setSorting(QDir::Name | QDir::Time | QDir::Reversed);
	typeDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QFileInfoList batchFolderList = typeDir.entryInfoList();
	if (batchFolderList.isEmpty()) return;

	(*OutputFolderHierarchy)[modelIndex].clear(); //���
	for (int i = 0; i < batchFolderList.size(); i++) {
		QString batchFolderPath = batchFolderList.at(i).absoluteFilePath();
		int batchIndex = batchFolderList.at(i).baseName().toInt(); //���κ�
		getBatchFolderInfo(modelIndex, batchIndex, batchFolderPath);
	}
}

//��ȡĳ�����κ��ļ����²�ͬ������ŵļ������Ϣ
void SysInitThread::getBatchFolderInfo(int modelIndex, int batchIndex, QString &batchFolderPath)
{
	QDir batchDir(batchFolderPath);
	batchDir.setSorting(QDir::Name);
	batchDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QFileInfoList sampleFolderList = batchDir.entryInfoList();
	//���κ��ļ������������ļ��У���ɾ�����ļ���
	if (sampleFolderList.size() == 0) {
		batchDir.removeRecursively(); return;
	}

	QList<int> tmpList;//�м���������ڽ��ļ��������ִ�С����
	for (int j = 0; j < sampleFolderList.size(); j++) {
		int sampleIndex = (QFileInfo(sampleFolderList.at(j)).fileName()).toInt();//�����ڵ��������
		QDir sampleDir(batchFolderPath + "/" + QString::number(sampleIndex));

		//ɾ���յ������ļ���
		if (sampleDir.entryInfoList().size() <= 2) { //�ļ���Ϊ�գ�size��СΪ2������0
			sampleDir.removeRecursively(); continue;
		}
		//���ǿյ������ļ��м���DirData��list��
		tmpList.push_back(sampleIndex);
	}

	//����ɾ���������ļ��к󣬿��������ļ��п��ˣ���Ҫɾ���յ������ļ���
	if (batchDir.entryInfoList().size() == 0) {
		batchDir.removeRecursively(); 
		(*OutputFolderHierarchy)[modelIndex].remove(batchIndex);
		return;
	}

	//���򲢽���Ϣ��ӵ�DirData��
	if (tmpList.size() != 0) {
		qSort(tmpList);
		(*OutputFolderHierarchy)[modelIndex].insert(batchIndex, tmpList);
	}
}

//�����б�ת�ַ����б�
void SysInitThread::NumList2StrList(const QList<int>& input, QStringList& output)
{
	auto iter = input.begin();
	for (iter; iter != input.end(); iter++) {
		output.push_back(QString::number(*iter, 10));
	}
}
