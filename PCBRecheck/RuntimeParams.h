#pragma once

#include <QWidget>
#include <QString>
#include <QTime>
#include <QFileDialog>
#include <QCoreApplication>
#include <QDebug>
#include "pcbFuncLib.h"
#include "MyMessageBox.h"


namespace pcb
{
#ifndef RECHECK_CLASS_RUNTIME_PARAMS 
#define RECHECK_CLASS_RUNTIME_PARAMS
	//程序运行期间使用的临时变量或参数
	class RuntimeParams 
	{
	public:
		QString serialNum; //产品序号
		QString sampleModelNum; //型号
		QString sampleBatchNum; //批次号
		QString sampleNum; //样本编号
		QString AppDirPath; //程序所在目录
		QString BufferDirPath; //缓存目录

		enum ParamsIndex {
			Index_None,
			Index_All,
			Index_All_SerialNum,
			Index_serialNum,
			Index_sampleModelNum,
			Index_sampleBatchNum,
			Index_sampleNum
		};

		enum ErrorCode {
			ValidParams = 0x000,
			ValidValue = 0x000,
			ValidValues = 0x000,
			Unchecked = 0x200,
			Invalid_serialNum = 0x201,
			Invalid_sampleModelNum = 0x202,
			Invalid_sampleBatchNum = 0x203,
			Invalid_sampleNum = 0x204,
			Default = 0x2FF
		};

	private:
		const int serialNumSlice[4] = { 8, 2, 2, 4 }; //序号组成
		ErrorCode errorCode; //错误代码
		ErrorCode errorCode_SerialNum; //错误代码

	public:
		RuntimeParams();
		~RuntimeParams();

		void loadDefaultValue();
		ErrorCode parseSerialNum();//产品序号解析
		QString getRelativeFolderPath();
		ErrorCode checkValidity(ParamsIndex index = Index_All); //检查参数有效性
		bool isValid(ParamsIndex index = Index_All, bool doCheck = false);//判断运行参数类是否有效
		void showMessageBox(QWidget *parent, ErrorCode code = Default); //弹窗警告
	};
#endif //RECHECK_CLASS_RUNTIME_PARAMS
}
