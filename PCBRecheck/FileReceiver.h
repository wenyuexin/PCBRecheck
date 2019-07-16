#pragma once

#include <asio.hpp>
#include <array>
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <filesystem>
#include <QDir>


//文件接收类
class FileReceiver
{
public:
    using TcpSocket = asio::ip::tcp::socket;
    using TcpAcceptor = asio::ip::tcp::acceptor;
    using IoService = asio::io_service;

    FileReceiver(short port, std::string const& workDirectory);//指定接收端口和文件夹
	~FileReceiver();
	void startListen();//开始监听

private:
    void doAccept();
    void createWorkDirectory(const std::string &workDirectory);

private:
	std::string m_workpath;//文件存储路径

	IoService  *m_iomanager;
    TcpSocket *m_socket;
    TcpAcceptor *m_acceptor;
};



//会话层
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
	void handleError(std::string const& t_functionName, asio::error_code const& t_ec);

private:
	TcpSocket m_socket;
	enum { MaxLength = 1024 * 40 };
	std::array<char, MaxLength> m_buf;
	asio::streambuf m_requestBuf_;
	std::ofstream m_outputFile;
	size_t m_fileSize;
	std::string hierarchy; //目录层次："类型/批次号/样本编号"
	std::string m_fileName; //文件名
	std::string m_filePath; //文件相对路径
	std::string m_fullName; //输入文件绝对路径
	std::string fileOutPath; //输出文件的绝对路径
};