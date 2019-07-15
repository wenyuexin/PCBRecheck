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
	//���������ڼ�ʹ�õ���ʱ���������
	class RuntimeParams 
	{
	public:
		QString serialNum; //��Ʒ���
		QString sampleModelNum; //�ͺ�
		QString sampleBatchNum; //���κ�
		QString sampleNum; //�������
		QString AppDirPath; //��������Ŀ¼
		QString BufferDirPath; //����Ŀ¼

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
		const int serialNumSlice[4] = { 8, 2, 2, 4 }; //������
		ErrorCode errorCode; //�������
		ErrorCode errorCode_SerialNum; //�������

	public:
		RuntimeParams();
		~RuntimeParams();

		void loadDefaultValue();
		ErrorCode parseSerialNum();//��Ʒ��Ž���
		QString getRelativeFolderPath();
		ErrorCode checkValidity(ParamsIndex index = Index_All); //��������Ч��
		bool isValid(ParamsIndex index = Index_All, bool doCheck = false);//�ж����в������Ƿ���Ч
		void showMessageBox(QWidget *parent, ErrorCode code = Default); //��������
	};
#endif //RECHECK_CLASS_RUNTIME_PARAMS
}
