
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

	//�����ں�
	struct TCP_Info
	{
		unsigned char							cbDataKind;																			//��������
		unsigned char							cbCheckCode;																		//Ч���ֶ�
		unsigned short							wPacketSize;																		//���ݴ�С
	};

	//��������
	struct TCP_Command
	{
		unsigned short							wMainCmdID;																			//��������
		unsigned short							wSubCmdID;																			//��������
	};

	//�����ͷ
	struct TCP_Head
	{
		TCP_Info								TCPInfo;																			//�����ṹ
		TCP_Command								CommandInfo;																		//������Ϣ
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
