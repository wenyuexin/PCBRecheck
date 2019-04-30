#pragma once
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextStream>
#include <QDebug>
#include <QJsonObject>
#include <string.h>
#include "RuntimeLib.h"
#include "windows.h"

namespace pcb 
{
#ifndef STRUCT_RECHECK_CONFIG
#define STRUCT_RECHECK_CONFIG 
	//参数配置
	class UserConfig 
	{
	public:
		//QString TemplDirPath;//模板文件的存储路径
		//QString SampleDirPath;//样本文件存储路径
		QString OutputDirPath;//检测结果存储路径
		QString ImageFormat; //图像后缀

		//参数索引
		enum ConfigIndex {
			Index_All,
			Index_None,
			//Index_SampleDirPath,
			//Index_TemplDirPath,
			Index_OutputDirPath,
			Index_ImageFormat
		};

		//错误代码
		enum ErrorCode {
			ValidConfig = 0x000,
			ValidValue = 0x000,
			Uncheck = 0x100,
			ConfigFileMissing = 0x101,
			//Invalid_TemplDirPath = 0x102,
			//Invalid_SampleDirPath = 0x103,
			Invalid_OutputDirPath = 0x104,
			Invalid_ImageFormat = 0x105,
			Default = 0x2FF
		};

	private:
		ErrorCode errorCode = Uncheck;

	public:
		UserConfig();
		~UserConfig();

		void loadDefaultValue(); //加载默认参数
		ErrorCode checkValidity(ConfigIndex index = Index_All);
		bool isValid(bool doCheck = false);
		inline void markConfigFileMissing() { errorCode = ConfigFileMissing; } //标记文件丢失
		inline void resetErrorCode() { errorCode = Uncheck; } //重置错误代码
		inline ErrorCode getErrorCode() { return errorCode; } //获取错误代码
		void showMessageBox(QWidget *parent, ErrorCode code = Default); //弹窗警告
	};
#endif //STRUCT_RECHECK_CONFIG


#ifndef CLASS_CONFIGURATOR
#define CLASS_CONFIGURATOR
	//参数配置文件的读写
	class Configurator
	{
	private:
		QFile *configFile;

	public:
		Configurator(QFile *file = Q_NULLPTR);
		~Configurator();

		static void init(QString filePath);
		static void createConfigFile(QString filePath);
		void jsonSetValue(const QString &key, QString &value);
		void jsonReadValue(const QString &key, QString &value);

		static bool loadConfigFile(const QString &fileName, pcb::UserConfig *config);
		static bool saveConfigFile(const QString &fileName, pcb::UserConfig *config);

		quint64 getDiskFreeSpace(QString driver);
		bool checkDir(QString dirpath);
	};
#endif //CONFIGURATOR_H
}