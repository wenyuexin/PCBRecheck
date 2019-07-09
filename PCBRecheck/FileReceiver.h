#pragma once

#include <array>
#include <fstream>
#include <string>
#include <memory>
#include <asio.hpp>
#include <iostream>
#include <QDir>


//�Ự��
class Session
    : public std::enable_shared_from_this<Session>
{
public:
    using TcpSocket = asio::ip::tcp::socket;

    Session(TcpSocket t_socket);
    void start() { doRead(); }

private:
    void doRead();
    void processRead(size_t t_bytesTransferred);
    void createFile();
    void readData(std::istream &stream);
    void doReadFileContent(size_t t_bytesTransferred);
    void handleError(std::string const& t_functionName, system::error_code const& t_ec);

private:
    TcpSocket m_socket;
    enum { MaxLength = 1024*40 };
    std::array<char, MaxLength> m_buf;
    asio::streambuf m_requestBuf_;
    std::ofstream m_outputFile;
    size_t m_fileSize;
    std::string m_fileName;
	std::string m_filePath;
	std::string m_fullName;
};


//�ļ�������
class FileReceiver
{
public:
    using TcpSocket = asio::ip::tcp::socket;
    using TcpAcceptor = asio::ip::tcp::acceptor;
    using IoService = asio::io_service;

    FileReceiver(short t_port, std::string const& t_workDirectory);//ָ�����ն˿ں��ļ���
	~FileReceiver();
	void startListen();//��ʼ����

private:
    void doAccept();
    void createWorkDirectory();

private:
	std::string m_workpath;//�ļ��洢·��

	IoService  *m_iomanager;
    TcpSocket *m_socket;
    TcpAcceptor *m_acceptor;

    std::string m_workDirectory;
	short m_port;
};
