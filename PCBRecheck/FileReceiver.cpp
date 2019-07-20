#include "FileReceiver.h"

using std::string;

FileReceiver::FileReceiver(short port, std::string const& workDirectory)
{
	m_iomanager = new IoService;
	m_socket = new TcpSocket(*m_iomanager);
	m_acceptor = new TcpAcceptor(*m_iomanager, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
	createWorkDirectory(workDirectory);
	Session::setStorePath(workDirectory);
	doAccept();
}

FileReceiver::~FileReceiver()
{
	m_socket->close();
	delete m_socket;
	m_socket = nullptr;
	m_iomanager->stop();
	pcb::delay(300);
	delete m_acceptor;
	delete m_iomanager;
}


void FileReceiver::startListen()
{
		m_iomanager->run();
		if (m_iomanager->stopped())
		{
			m_acceptor->close();
		}
}

void FileReceiver::doAccept()
{
    m_acceptor->async_accept(*m_socket,
        [this](asio::error_code ec)
    {
        if (!ec) std::make_shared<Session>(std::move(*m_socket))->start();
        doAccept();
    });
}

void FileReceiver::createWorkDirectory(const string &workDirectory)
{
    using namespace std::filesystem;
    auto currentPath = path(workDirectory);
	if (!exists(currentPath) && !create_directory(currentPath)) {
        //BOOST_LOG_TRIVIAL(error) << "Coudn't create working directory: " << m_workDirectory;
	}
	//current_path(currentPath);
}



/****************************************/

std::string Session::m_storeRootPath = "";//��̬������ʼ��

Session::Session(TcpSocket t_socket)
	: m_socket(std::move(t_socket))
{
}

void Session::setStorePath(const std::string & path)
{
	m_storeRootPath = path;
}


void Session::doRead()
{
	auto self = shared_from_this();
	async_read_until(m_socket, m_requestBuf_, "\n\n", //�����ļ���+�ļ�size����"\n\n"��β��
		[this, self](asio::error_code ec, size_t bytes)
	{
		if (!ec)
			processRead(bytes);//��ʼ��ȡ�׽���
		else
			handleError(__FUNCTION__, ec);
	});
}


void Session::processRead(size_t t_bytesTransferred)
{
	std::istream requestStream(&m_requestBuf_);
	readData(requestStream);

	auto pos1 = m_fullName.find(hierarchy);//Ѱ�ұ�ʾĿ¼��ε����ַ���
	auto pos2 = m_fullName.find_last_of("/");
	if (pos2 == std::string::npos) {
		pos2 = m_fullName.find_last_of("\\");
	}

	if (pos1 != std::string::npos && pos2 != std::string::npos) {
		fileOutPath = m_storeRootPath + "/" + m_fullName.substr(pos1, pos2 - pos1 + 1);//�ļ�·������Ŀ¼���ļ���ǰ
		m_fileName = fileOutPath + m_fullName.substr(pos2 + 1);//�����ļ���������·��
	}

	createFile();//����Ŀ¼���ļ�

	//дʣ�µ�����
	do {
		requestStream.read(m_buf.data(), m_buf.size());
		m_outputFile.write(m_buf.data(), requestStream.gcount());
	} while (requestStream.gcount() > 0);

	auto self = shared_from_this();
	m_socket.async_read_some(asio::buffer(m_buf.data(), m_buf.size()),
		[this, self](asio::error_code ec, size_t bytes)
	{
		if (!ec)
			doReadFileContent(bytes);
		else
			handleError(__FUNCTION__, ec);
	});
}


void Session::readData(std::istream &stream)//��ȡ�ļ������ļ���С
{
	stream >> hierarchy;//Ŀ¼���
	stream >> m_fullName;//�ļ�����·��
	stream >> m_fileSize;//�ļ���С
	stream.read(m_buf.data(), 2);
}


void Session::createFile()
{
	using namespace std::filesystem;
	if (!exists(fileOutPath)) {
		create_directories(fileOutPath);
	}
	m_outputFile.open(m_fileName, std::ios_base::binary);
	if (!m_outputFile) {
		//BOOST_LOG_TRIVIAL(error) << __LINE__ << ": �����ļ�ʧ��: " << m_fileName;
		return;
	}
}


void Session::doReadFileContent(size_t t_bytesTransferred)
{
	if (t_bytesTransferred > 0) {
		m_outputFile.write(m_buf.data(), static_cast<std::streamsize>(t_bytesTransferred));

		if (m_outputFile.tellp() >= static_cast<std::streamsize>(m_fileSize)) {
			std::cout << "Received file: " << m_fileName << std::endl;
			return;
		}
	}
	auto self = shared_from_this();
	m_socket.async_read_some(asio::buffer(m_buf.data(), m_buf.size()),
		[this, self](asio::error_code ec, size_t bytes)
	{
		doReadFileContent(bytes);
	});
}


void Session::handleError(std::string const& t_functionName, asio::error_code const& t_ec)
{
	//BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " in " << t_functionName << " due to " 
	//    << t_ec << " " << t_ec.message() << std::endl;
	std::cout << __FUNCTION__ << " in " << t_functionName << " due to "
		<< t_ec << " " << t_ec.message() << std::endl;

}
