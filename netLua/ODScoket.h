
#ifndef _ODSOCKET_H_
#define _ODSOCKET_H_

#ifdef WIN32
	//#include <winsock.h>
	#include <ws2tcpip.h>
	typedef int				socklen_t;
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <arpa/inet.h>
	typedef int				SOCKET;
	#define INVALID_SOCKET	-1
	#define SOCKET_ERROR	-1

#endif

#include <string>

	//网络内核
	struct TCP_Info
	{
		unsigned char							cbDataKind;																			//数据类型
		unsigned char							cbCheckCode;																		//效验字段
		unsigned short							wPacketSize;																		//数据大小
	};

	//网络命令
	struct TCP_Command
	{
		unsigned short							wMainCmdID;																			//主命令码
		unsigned short							wSubCmdID;																			//子命令码
	};

	//网络包头
	struct TCP_Head
	{
		TCP_Info								TCPInfo;																			//基础结构
		TCP_Command								CommandInfo;																		//命令信息
	};

class ODSocket {

public:
	ODSocket(SOCKET sock = INVALID_SOCKET);
	virtual ~ODSocket();
	bool Create(int af, int type, int protocol = 0);
	bool Connect(const char* host, unsigned short port);
	bool Connect(const std::string &host);
	bool ConnectIPv4(const char *ip, unsigned short port);
	bool ConnectIPV6(const char *ip, unsigned int port);
	int Send(const char* buf, int len, int flags = 0);
	int Recv(char* buf, int len, int flags = 0);
	int Close();
	int GetError();
	bool IsVaild();
	static int Init();	
	static int Clean();
	static bool DnsParse(const char* domain, char* ip);
	ODSocket& operator = (SOCKET s);
	operator SOCKET ();

protected:
	SOCKET	m_sock;
};

#endif
