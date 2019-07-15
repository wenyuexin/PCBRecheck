#include "Configurator.h"

using pcb::UserConfig;
using pcb::Configurator;


/****************************************************/
/*                     UserConfig                   */
/****************************************************/

UserConfig::UserConfig()
{
	//TemplDirPath = "";//模板文件的存储路径
	//SampleDirPath = "";//样本文件存储路径
	OutputDirPath = "";//检测结果存储路径
	ImageFormat = ""; //图像后缀
	roiSize_W = 0; //ROI区域的宽度(pix)
	roiSize_H = 0; //ROI区域的高度(pix)
}

UserConfig::~UserConfig()
{
	qDebug() << "~UserConfig";
}

//加载默认参数
void UserConfig::loadDefaultValue()
{
	QDir dir(QDir::currentPath()); //根据程序所在目录获取QDir
	dir.cdUp(); //转到上一级目录
	QString upperDirPath = dir.absolutePath(); //上一级目录的绝对路径

	//this->TemplDirPath = upperDirPath + "/template";//模板路径
	//this->SampleDirPath = upperDirPath + "/sample";//样本路径
	this->OutputDirPath = upperDirPath + "/output";//结果路径
	this->ImageFormat = ".bmp"; //图像后缀

	roiSize_W = 300; //ROI区域的宽度(pix)
	roiSize_H = 120; //ROI区域的高度(pix)
}

//检查参数有效性
UserConfig::ErrorCode UserConfig::checkValidity(ConfigIndex index)
{
	ErrorCode code = ErrorCode::Uncheck;
	switch (index)
	{
	case pcb::UserConfig::Index_All:
	//case pcb::UserConfig::Index_TemplDirPath: //模板路径
	//	if (TemplDirPath == "" || !QFileInfo(TemplDirPath).isDir())
	//		code = Invalid_TemplDirPath;
	//	if (code != Uncheck || index != Index_All) break;
	//case pcb::UserConfig::Index_SampleDirPath: //样本路径
	//	if (SampleDirPath == "" || !QFileInfo(SampleDirPath).isDir())
	//		code = Invalid_SampleDirPath;
	//	if (code != Uncheck || index != Index_All) break;
	case pcb::UserConfig::Index_OutputDirPath: //输出路径
		if (OutputDirPath == "" || !QFileInfo(OutputDirPath).isDir())
			code = Invalid_OutputDirPath;
		if (code != Uncheck || index != Index_All) break;
	case pcb::UserConfig::Index_ImageFormat: //图像格式
		if (code != Uncheck || index != Index_All) break;
	case pcb::UserConfig::Index_roiSize_W: //ROI区域的宽度
		if (roiSize_W <= 0) 
			code = Invalid_roiSize_W; 
		if (code != Uncheck || index != Index_All) break;
	case pcb::UserConfig::Index_roiSize_H: //ROI区域的高度
		if (roiSize_H <= 0) 
			code = Invalid_roiSize_H; 
		if (code != Uncheck || index != Index_All) break;
	}

	//代码值等于Uncheck表示检测的参数有效
	if (code == Uncheck) code = ValidConfig;
	//更新错误代码
	if (code != ValidConfig || index == Index_All) errorCode = code;
	return code;
}

//判断用户参数类是否有效
bool UserConfig::isValid(bool doCheck)
{
	if (errorCode == ErrorCode::ValidConfig) return true;

	if (doCheck && errorCode == ErrorCode::Uncheck)
		this->checkValidity(Index_All);
	return (errorCode == ErrorCode::ValidConfig);
}

//弹窗提示
void UserConfig::showMessageBox(QWidget *parent, ErrorCode code)
{
	ErrorCode tempCode = (code == Default) ? errorCode : code;
	if (tempCode == UserConfig::ValidConfig) return;

	QString valueName;
	if (tempCode == ConfigFileMissing) {
		QString message = pcb::chinese(".user.config文件丢失，已生成默认文件!    \n")
			+ pcb::chinese("请在参数设置界面确认参数是否有效 ...   \n");
		message += "Config: User: ErrorCode: " + QString::number(tempCode);

		MyMessageBox messageBox;
		messageBox.set(pcb::MessageBoxType::Warning, message);
		messageBox.doShow();
		return;
	}

	switch (tempCode)
	{
	case pcb::UserConfig::Uncheck:
		valueName = pcb::chinese("参数未验证"); break;
	//case pcb::UserConfig::Invalid_TemplDirPath:
	//	valueName = pcb::chinese("模板路径"); break;
	//case pcb::UserConfig::Invalid_SampleDirPath:
	//	valueName = pcb::chinese("样本路径"); break;
	case pcb::UserConfig::Invalid_OutputDirPath:
		valueName = pcb::chinese("输出路径"); break;
	case pcb::UserConfig::Invalid_ImageFormat:
		valueName = pcb::chinese("图像格式"); break;
	case pcb::UserConfig::Invalid_roiSize_W:
	case pcb::UserConfig::Invalid_roiSize_H:
		valueName = pcb::chinese("产品序号的字符区域大小"); break;
	case pcb::UserConfig::Default:
		valueName = pcb::chinese("-"); break;
	}

	MyMessageBox messageBox;
	QString message = pcb::chinese("用户参数无效，请在参数设置界面重新设置！  \n") +
		pcb::chinese("错误来源：") + valueName + "   \n" +
		"Config: User: ErrorCode: " + QString::number(tempCode);
	messageBox.set(pcb::MessageBoxType::Warning, message);
	messageBox.doShow();
	return;
}



/****************************************************/
/*                    Configurator                  */
/****************************************************/

Configurator::Configurator(QFile *file)
{
	this->configFile = file;
}

Configurator::~Configurator()
{
	qDebug() << "~Configurator";
}

/************** 生成默认的参数配置文件 **************/

//生成默认的参数配置文件
void Configurator::createConfigFile(QString filePath)
{
	QFileInfo config(filePath);
	if (!config.isFile()) { //没有配置文件，则生成配置文件
		QFile file(filePath);
		file.open(QIODevice::WriteOnly);
		QTextStream textStrteam(&file);
		QVariantMap pathConfig;
		//pathConfig.insert("###", "###");
		QJsonDocument jsonDocument = QJsonDocument::fromVariant(pathConfig);
		textStrteam << jsonDocument.toJson();
		file.close();
	}
}

//生成默认的参数配置文件 - 没用
//void Configurator::init(QString filePath)
//{
//	QFileInfo config(filePath);
//	if (!config.isFile()) { //没有配置文件  则创建文件 ; 生成配置文件
//		QFile file(filePath);
//		file.open(QIODevice::WriteOnly);
//		QTextStream textStrteam(&file);
//		QVariantMap pathConfig;
//
//		QDir dir(QDir::currentPath());
//		dir.cdUp(); //转到上一级目录
//		QString appUpperDirPath = dir.absolutePath(); //上一级目录的绝对路径
//		//pathConfig.insert("TemplDirPath", appUpperDirPath + "/template");
//		//pathConfig.insert("SampleDirPath", appUpperDirPath + "/sample");
//		pathConfig.insert("OutputDirPath", appUpperDirPath + "/output");
//		pathConfig.insert("ImageFormat", ".bmp");
//
//		QJsonDocument jsonDocument = QJsonDocument::fromVariant(pathConfig);
//		textStrteam << jsonDocument.toJson();
//		file.close();
//	}
//}

/********** 将单个参数的写入config文件中 ************/

//设置参数 - QString
bool Configurator::jsonSetValue(const QString &key, QString &value)
{
	QTextStream textStrteam(configFile);
	configFile->seek(0);
	QString val = configFile->readAll();
	QJsonParseError json_error;
	QJsonDocument confDcoument = QJsonDocument::fromJson(val.toUtf8(), &json_error);

	if (json_error.error == QJsonParseError::NoError) {
		if (!confDcoument.isNull() || !confDcoument.isEmpty()) {
			if (confDcoument.isObject()) {
				QJsonObject obj = confDcoument.object();
				obj[key] = value;
				QJsonDocument document = QJsonDocument::fromVariant(obj.toVariantMap());
				configFile->resize(0);
				textStrteam << document.toJson();
				return true;
			}
		}
		else { //文件为空
			qDebug() << "文件空";
		}
	}
	return false;
}

//将参数写入配置文件中 - int
bool Configurator::jsonSetValue(const QString &key, int &value)
{
	return jsonSetValue(key, QString::number(value));
}

//将参数写入配置文件中 - double
bool Configurator::jsonSetValue(const QString &key, double &value)
{
	return jsonSetValue(key, QString::number(value, 'f', 6));
}


//读取参数 - QString
bool Configurator::jsonReadValue(const QString &key, QString &value)
{
	configFile->seek(0);
	QString val = configFile->readAll();
	QJsonParseError json_error;
	QJsonDocument confDcoument = QJsonDocument::fromJson(val.toUtf8(), &json_error);
	if (json_error.error == QJsonParseError::NoError) {
		if (!confDcoument.isNull() || !confDcoument.isEmpty()) {
			if (confDcoument.isObject()) {
				QJsonObject obj = confDcoument.object();
				value = obj[key].toString();
				return true;
			}
		}
		else {
			qDebug() << "文件空";
			value = "";
		}
	}
	return false;
}

//从配置文件中读取参数 - int
bool Configurator::jsonReadValue(const QString &key, int &value)
{
	QString valueStr = "";
	if (jsonReadValue(key, valueStr)) {
		value = valueStr.toInt(); return true;
	}
	return false;
}

//从配置文件中读取参数 - double
bool Configurator::jsonReadValue(const QString &key, double &value)
{
	QString valueStr = "";
	if (jsonReadValue(key, valueStr)) {
		value = valueStr.toDouble(); return true;
	}
	return false;
}


/****************** 配置文件的读写 ********************/

//将UserConfig中的参数保存到配置文件中
bool Configurator::loadConfigFile(const QString &fileName, UserConfig *config)
{
	bool success = true;
	QString configFilePath = QDir::currentPath() + "/" + fileName;
	QFile configFile(configFilePath);
	if (!configFile.exists() || !configFile.open(QIODevice::ReadWrite)) { //判断配置文件读写权限
		createConfigFile(configFilePath);//创建配置文件
		config->loadDefaultValue();//加载默认值
		config->markConfigFileMissing();//标记文件丢失
		saveConfigFile(fileName, config);//保存默认config
		success = false;
	}
	else { //文件存在，并且可以正常读写
		Configurator configurator(&configFile);
		//configurator.jsonReadValue("TemplDirPath", config->TemplDirPath);
		//configurator.jsonReadValue("SampleDirPath", config->SampleDirPath);
		configurator.jsonReadValue("OutputDirPath", config->OutputDirPath);
		configurator.jsonReadValue("ImageFormat", config->ImageFormat);
		configurator.jsonReadValue("roiSize_W", config->roiSize_W);
		configurator.jsonReadValue("roiSize_H", config->roiSize_H);
		configFile.close();
	}
	return success;
}

//将配置文件中的参数加载到UserConfig中
bool Configurator::saveConfigFile(const QString &fileName, UserConfig *config)
{
	bool success = true;
	QString configFilePath = QDir::currentPath() + "/" + fileName;
	QFile configFile(configFilePath);
	if (!configFile.exists() || !configFile.open(QIODevice::ReadWrite)) { //判断配置文件读写权限
		createConfigFile(configFilePath);//创建配置文件
		config->loadDefaultValue();//加载默认值
		config->markConfigFileMissing();//标记文件丢失
		saveConfigFile(fileName, config);//保存默认config
		success = false;
	}
	else { //文件存在，并且可以正常读写
		Configurator configurator(&configFile);
		//configurator.jsonSetValue("TemplDirPath", config->TemplDirPath);
		//configurator.jsonSetValue("SampleDirPath", config->SampleDirPath);
		configurator.jsonSetValue("OutputDirPath", config->OutputDirPath);
		configurator.jsonSetValue("ImageFormat", config->ImageFormat);
		configurator.jsonSetValue("roiSize_W", config->roiSize_W);
		configurator.jsonSetValue("roiSize_H", config->roiSize_H);
		configFile.close();
	}
	return success;
}


/*************** 暂时没用 **********************/

//获取当前磁盘剩余空间
//quint64 Configurator::getDiskFreeSpace(QString driver)  
//{
//	LPCWSTR lpcwstrDriver = (LPCWSTR)driver.utf16();
//	ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
//
//	if (!GetDiskFreeSpaceEx(lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes)) {
//		qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
//		return 0;
//	}
//	return (quint64)liTotalFreeBytes.QuadPart / 1024 / 1024 / 1024;
//}

//bool Configurator::checkDir(QString dirpath)
//{
//	QDir dir(dirpath);
//	dir.setSorting(QDir::Name | QDir::Time | QDir::Reversed);
//	dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
//	QFileInfoList folder_list = dir.entryInfoList();
//	if (folder_list.size() < 1) {
//		return false;
//	}
//	else {
//		QString name = folder_list.at(0).absoluteFilePath();
//		QDir dir(name);
//		dir.setSorting(QDir::Name | QDir::Time | QDir::Reversed);
//		dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
//		QFileInfoList file_list = dir.entryInfoList();
//		if (file_list.size() < 1) return false;
//
//		QString name2 = file_list.at(0).absoluteFilePath();
//		QFileInfo config(name2 + "/" + "outputImage/");
//		if (!config.isDir()) return false; //没有配置文件 则创建文件
//	}
//	return true;
//}
