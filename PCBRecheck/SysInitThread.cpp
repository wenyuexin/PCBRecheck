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

//执行线程
void SysInitThread::run()
{
	//系统初始化操作，读取配置文件
	if (!initUserConfig()) return;

	//读取output文件夹下的目录层次
	if (!initOutputFolderHierarchy()) return;

	emit sysInitFinished_initThread();
}


/****************** 检修系统初始化 *******************/

//初始化用户参数
bool SysInitThread::initUserConfig()
{
	//emit sysInitStatus_initThread(pcb::chinese("正在获取系统参数   "));

	if (!Configurator::loadConfigFile(".user.config", userConfig)) {
		emit userConfigError_initThread(); return false;
	}
	else {
		//参数有效性判断
		UserConfig::ErrorCode code;//错误代码
		code = userConfig->checkValidity(UserConfig::Index_All);
		if (code != UserConfig::ValidConfig) {
			emit userConfigError_initThread(); return false;
		}
	}

	//emit sysInitStatus_initThread(pcb::chinese("系统参数获取结束   "));
	pcb::delay(800);
	return true;
}


/************************* 路径信息初始化 **************************/

bool SysInitThread::initOutputFolderHierarchy()
{
	//获取output文件夹下的文件夹层次
	if (!getOutputFolderInfo(userConfig->OutputDirPath)) {
		emit outFolderHierarchyError_initThread(); return false;
	}
	return true;
}


//获取output文件夹下的文件信息
//内部包含三个层次：型号、批次号、样本编号
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


//获取某型号文件夹下的文件信息
void SysInitThread::getModelFolderInfo(int modelIndex, QString &modelFolderPath)
{
	QDir typeDir(modelFolderPath);
	typeDir.setSorting(QDir::Name | QDir::Time | QDir::Reversed);
	typeDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QFileInfoList batchFolderList = typeDir.entryInfoList();
	if (batchFolderList.isEmpty()) return;

	(*OutputFolderHierarchy)[modelIndex].clear(); //清空
	for (int i = 0; i < batchFolderList.size(); i++) {
		QString batchFolderPath = batchFolderList.at(i).absoluteFilePath();
		int batchIndex = batchFolderList.at(i).baseName().toInt(); //批次号
		getBatchFolderInfo(modelIndex, batchIndex, batchFolderPath);
	}
}

//获取某个批次号文件夹下不同样本编号的检测结果信息
void SysInitThread::getBatchFolderInfo(int modelIndex, int batchIndex, QString &batchFolderPath)
{
	QDir batchDir(batchFolderPath);
	batchDir.setSorting(QDir::Name);
	batchDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QFileInfoList sampleFolderList = batchDir.entryInfoList();
	//批次号文件夹中无样本文件夹，则删除该文件夹
	if (sampleFolderList.size() == 0) {
		batchDir.removeRecursively(); return;
	}

	QList<int> tmpList;//中间变量，用于将文件按照数字大小排序
	for (int j = 0; j < sampleFolderList.size(); j++) {
		int sampleIndex = (QFileInfo(sampleFolderList.at(j)).fileName()).toInt();//批次内的样本编号
		QDir sampleDir(batchFolderPath + "/" + QString::number(sampleIndex));

		//删除空的样本文件夹
		if (sampleDir.entryInfoList().size() <= 2) { //文件夹为空，size大小为2，不是0
			sampleDir.removeRecursively(); continue;
		}
		//将非空的样本文件夹加入DirData的list中
		tmpList.push_back(sampleIndex);
	}

	//上面删除了样本文件夹后，可能批次文件夹空了，需要删除空的批次文件夹
	if (batchDir.entryInfoList().size() == 0) {
		batchDir.removeRecursively(); 
		(*OutputFolderHierarchy)[modelIndex].remove(batchIndex);
		return;
	}

	//排序并将信息添加到DirData中
	if (tmpList.size() != 0) {
		qSort(tmpList);
		(*OutputFolderHierarchy)[modelIndex].insert(batchIndex, tmpList);
	}
}

//数字列表转字符串列表
void SysInitThread::NumList2StrList(const QList<int>& input, QStringList& output)
{
	auto iter = input.begin();
	for (iter; iter != input.end(); iter++) {
		output.push_back(QString::number(*iter, 10));
	}
}
