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
	if (initOutFolderHierarchy() == -1) {
		emit outFolderHierarchyError_initThread(); return;
	}

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

int SysInitThread::initOutFolderHierarchy()
{
	//��ȡoutput�ļ����µ��ļ��в��
	getOutputFolderInfo(userConfig->OutputDirPath);

	//�����ǶԽ����һЩ��ʼ������ - ����
	if ((*OutputFolderHierarchy).size() == 0) return -1;

	return 0;
}


//��ȡoutput�ļ����µ��ļ���Ϣ
//�ڲ�����������Σ��������͡����κš��������
void SysInitThread::getOutputFolderInfo(QString dirpath)
{
	QDir outputDir(dirpath);
	outputDir.setSorting(QDir::Name | QDir::Time | QDir::Reversed);
	outputDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QFileInfoList typeFolderList = outputDir.entryInfoList();
	if (typeFolderList.isEmpty()) return;

	for (int i = 0; i < typeFolderList.size(); i++) {
		QString typeFolder = typeFolderList.at(i).absoluteFilePath();
		int typeNum = typeFolderList.at(i).baseName().toInt();
		getTypeFolderInfo(typeNum, typeFolder);
	}
}


//��ȡĳһ���������ļ����µ��ļ���Ϣ
void SysInitThread::getTypeFolderInfo(int typeNum, QString typeFolder)
{
	(*OutputFolderHierarchy)[typeNum].clear();
	QDir typeDir(typeFolder);
	typeDir.setSorting(QDir::Name | QDir::Time | QDir::Reversed);
	typeDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QFileInfoList batchFolderList = typeDir.entryInfoList();
	if (batchFolderList.isEmpty()) return;

	for (int i = 0; i < batchFolderList.size(); i++) {
		QString batchFolder = batchFolderList.at(i).absoluteFilePath();
		int batchNum = batchFolderList.at(i).baseName().toInt(); //���κ�
		if ((*OutputFolderHierarchy)[typeNum].contains(batchNum)) continue;

		QDir batchDir(batchFolder);
		batchDir.setSorting(QDir::Time);//  QDir::Name | QDir::Time | QDir::Reversed
		batchDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		QFileInfoList sampleFolderList = batchDir.entryInfoList();
		if (sampleFolderList.size() == 0) { //���κ��ļ������������ļ��У�ɾ�����ļ���
			batchDir.removeRecursively();
			continue;
		}

		QList<int> list_tmp;//�м���������ڽ��ļ��������ִ�С����
		for (int j = 0; j < sampleFolderList.size(); j++) {
			int sampleNum = (QFileInfo(sampleFolderList.at(j)).fileName()).toInt();//�����ڵ��������
			QDir sampleDir(batchFolder + "/" + QString::number(sampleNum));

			//�����ļ����������ļ��У�˵�����������ڼ���У�������DirData�У�����ʾ�ڽ�����
			//if (sampleDir.entryInfoList().size() == 4) continue;

			//ɾ���յ������ļ���
			sampleDir.cd(batchFolder + "/" + QString::number(sampleNum));
			if (sampleDir.entryInfoList().size() <= 2) { //��outputImageΪ��ʱ��size��СΪ2������0
				sampleDir.cd(batchFolder + "/" + QString::number(sampleNum));
				sampleDir.removeRecursively();
				continue;
			}
			//���ǿյ������ļ��м���DirData��list��
			list_tmp.push_back(sampleNum);
		}

		//����ɾ���������ļ��к󣬿��������ļ��п��ˣ���Ҫɾ���յ������ļ���
		if (batchDir.entryInfoList().size() == 0) {
			batchDir.removeRecursively();
			continue;
		}

		//���򲢽���Ϣ��ӵ�DirData��
		if (list_tmp.size() != 0) {
			qSort(list_tmp);
			(*OutputFolderHierarchy)[typeNum].insert(batchNum, list_tmp);
		}
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

