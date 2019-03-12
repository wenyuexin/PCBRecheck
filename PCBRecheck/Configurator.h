#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H
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
#include "windows.h"

namespace Ui {
	class Configurator;
	enum DATA_TYPE;

#ifndef STRUCT_RECHECK_CONFIG
#define STRUCT_RECHECK_CONFIG 
	//参数配置
struct RecheckConfig {
	QString OutputDirPath;//检测结果存储路径
	QString TemplDirPath;//模板文件的存储路径
	QString ImageFormat; //图像后缀
};
#endif //STRUCT_RECHECK_CONFIG

#ifndef STRUCT_RECHECK_PARAMS
#define STRUCT_RECHECK_PARAMS
//程序运行期间使用的临时变量或参数
struct RecheckParams {
	QString serialNum;
	QString sampleModelNum; //型号
	QString sampleBatchNum; //批次号
	QString sampleNum; //样本编号
};
#endif //STRUCT_RECHECK_PARAMS
}


//参数配置文件的读写
class Configurator
{

public:
	enum DATA_TYPE { TYPE_QSTRING, TYPE_INT, TYPE_LONG, TYPE_DOUBLE, TYPE_FLOAT };

private:
	QFile *configFile;

public:
	Configurator(QFile *file = Q_NULLPTR);
	~Configurator();

	static void init(QString filePath);
	void jsonSetValue(const QString &key, QString &value);
	void jsonReadValue(const QString &key, QString &value);

	template<typename T>
	void jsonReadValue(const QString &key, T &value, DATA_TYPE type = PCB_QSTRING)
	{
		configFile->seek(0);
		QString val = configFile->readAll();
		QJsonParseError json_error;
		QJsonDocument confDcoument = QJsonDocument::fromJson(val.toUtf8(), &json_error);
		if (json_error.error == QJsonParseError::NoError)
		{
			if (!confDcoument.isNull() || !confDcoument.isEmpty())
			{
				if (confDcoument.isObject())
				{
					QJsonObject obj = confDcoument.object();
					QString value_tmp = obj[key].toString();

					switch (type) //类型转换
					{
					case TYPE_INT:
						value = value_tmp.toInt(); break;
					case TYPE_LONG:
						value = value_tmp.toLong(); break;
					case TYPE_DOUBLE:
						value = value_tmp.toDouble(); break;
					case TYPE_FLOAT:
						value = value_tmp.toFloat(); break;
					default:
						value = -1; break;
					}
				}
			}
			else {
				qDebug() << "文件空";
				value = -1;
			}
		}
	}

	quint64 getDiskFreeSpace(QString driver);
	bool checkDir(QString dirpath);
};

#endif //CONFIGURATOR_H
