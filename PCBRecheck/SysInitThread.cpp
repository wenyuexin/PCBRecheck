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
	if (initOutFolderHierarchy() == -1) {
		emit outFolderHierarchyError_initThread(); return;
	}

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

int SysInitThread::initOutFolderHierarchy()
{
	//获取output文件夹下的文件夹层次
	getOutputFolderInfo(userConfig->OutputDirPath);

	//下面是对界面的一些初始化设置 - 暂无
	if ((*OutputFolderHierarchy).size() == 0) return -1;

	return 0;
}


//获取output文件夹下的文件信息
//内部包含三个层次：样本类型、批次号、样本编号
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


//获取某一样本类型文件夹下的文件信息
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
		int batchNum = batchFolderList.at(i).baseName().toInt(); //批次号
		if ((*OutputFolderHierarchy)[typeNum].contains(batchNum)) continue;

		QDir batchDir(batchFolder);
		batchDir.setSorting(QDir::Time);//  QDir::Name | QDir::Time | QDir::Reversed
		batchDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		QFileInfoList sampleFolderList = batchDir.entryInfoList();
		if (sampleFolderList.size() == 0) { //批次号文件夹中无样本文件夹，删除该文件夹
			batchDir.removeRecursively();
			continue;
		}

		QList<int> list_tmp;//中间变量，用于将文件按照数字大小排序
		for (int j = 0; j < sampleFolderList.size(); j++) {
			int sampleNum = (QFileInfo(sampleFolderList.at(j)).fileName()).toInt();//批次内的样本编号
			QDir sampleDir(batchFolder + "/" + QString::number(sampleNum));

			//样本文件夹有两个文件夹，说明该样本正在检测中，不列入DirData中，不显示在界面上
			//if (sampleDir.entryInfoList().size() == 4) continue;

			//删除空的样本文件夹
			sampleDir.cd(batchFolder + "/" + QString::number(sampleNum));
			if (sampleDir.entryInfoList().size() <= 2) { //当outputImage为空时，size大小为2，不是0
				sampleDir.cd(batchFolder + "/" + QString::number(sampleNum));
				sampleDir.removeRecursively();
				continue;
			}
			//将非空的样本文件夹加入DirData的list中
			list_tmp.push_back(sampleNum);
		}

		//上面删除了样本文件夹后，可能批次文件夹空了，需要删除空的批次文件夹
		if (batchDir.entryInfoList().size() == 0) {
			batchDir.removeRecursively();
			continue;
		}

		//排序并将信息添加到DirData中
		if (list_tmp.size() != 0) {
			qSort(list_tmp);
			(*OutputFolderHierarchy)[typeNum].insert(batchNum, list_tmp);
		}
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

