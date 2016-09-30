/*
 * Source file about portable socket class. 
 *
 * design:odison
 * e-mail:odison@126.com>
 * 
 */

#include "ODScoket.h"
#include <iostream>

using namespace std;

#ifdef WIN32
	#pragma comment(lib, "wsock32")
#else
int errno;
#endif

ODSocket::ODSocket(SOCKET sock)
{
	m_sock = sock;
}

ODSocket::~ODSocket()
{
}

int ODSocket::Init()
{
#ifdef WIN32
	/*
	http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx

	typedef struct WSAData { 
		WORD wVersion;								//winsock version
		WORD wHighVersion;							//The highest version of the Windows Sockets specification that the Ws2_32.dll can support
		char szDescription[WSADESCRIPTION_LEN+1]; 
		char szSystemStatus[WSASYSSTATUS_LEN+1]; 
		unsigned short iMaxSockets; 
		unsigned short iMaxUdpDg; 
		char FAR * lpVendorInfo; 
	}WSADATA, *LPWSADATA; 
	*/
	WSADATA wsaData;
	//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8)) 
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if ( ret ) {
		cerr << "Initilize winsock error !" << endl;
		return -1;
	}
#endif
	
	return 0;
}
//this is just for windows
int ODSocket::Clean()
{
#ifdef WIN32
		return (WSACleanup());
#endif
		return 0;
}

ODSocket& ODSocket::operator = (SOCKET s)
{
	m_sock = s;
	return (*this);
}

ODSocket::operator SOCKET ()
{
	return m_sock;
}

//create a socket object win/lin is the same
// af:
bool ODSocket::Create(int af, int type, int protocol)
{
	m_sock = socket(af, type, protocol);
	if ( m_sock == INVALID_SOCKET ) {
		return false;
	}
	return true;
}

bool ODSocket::Connect(const char* host, unsigned short port)
{
	bool isIP = true;
	for (int i = 0; i < strlen(host); ++i)
	{
		if ((host[i] != '.') && (host[i] < '0' || host[i] > '9'))
		{
			isIP = false;
			break;
		}
	}

	if (isIP)
	{
		return Create(AF_INET, SOCK_STREAM) && ConnectIPv4(host, port);
	}

	struct addrinfo * result;
	if (getaddrinfo(host, NULL, NULL, &result) != 0)
	{
		return false;
	}

	for (struct addrinfo *res = result; res != NULL; res = res->ai_next)
	{
		char hostname[1025] = "";
		if (getnameinfo(res->ai_addr, res->ai_addrlen, hostname, 1025, NULL, 0, 0) == 0)
		{
			switch (res->ai_addr->sa_family)
			{
				// IPv4
				case AF_INET:
					return Create(AF_INET, SOCK_STREAM) && ConnectIPv4(hostname, port);
				// IPv6
				case AF_INET6:
					return Create(AF_INET6, SOCK_STREAM) && ConnectIPV6(hostname, port);
				default:
					return false;
			}
		}
	}

	return false;
}

bool ODSocket::ConnectIPv4(const char *ip, unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
	int ret = connect(m_sock, (const struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}

bool ODSocket::ConnectIPV6(const char *ip, unsigned int port)
{
	struct sockaddr_in6 svraddr;
	memset(&svraddr, 0, sizeof(sockaddr_in6));

	svraddr.sin6_family = AF_INET6;
	svraddr.sin6_port = htons(port);
	if (inet_pton(AF_INET6, ip, &svraddr.sin6_addr) < 0)
	{
		return false;
	}

	if (connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool ODSocket::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);

	int opt =  1;
	if ( setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0 ) 
		return false;

    int ret = ::bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}
//for server
bool ODSocket::Listen(int backlog)
{
	int ret = listen(m_sock, backlog);
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}

bool ODSocket::Accept(ODSocket& s, char* fromip)
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	SOCKET sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);
	if ( sock == SOCKET_ERROR ) {
		return false;
	}

	s = sock;
	if ( fromip != NULL )
		sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));

	return true;
}

int ODSocket::Send(const char* buf, int len, int flags)
{
	int bytes;
	int count = 0;

	while ( count < len ) {

        bytes = send(m_sock, buf + count, len - count, flags);
		if ( bytes == -1 || bytes == 0 )
			return -1;
		count += bytes;
	} 

	return count;
}

int ODSocket::Recv(char* buf, int len, int flags)
{
    return (recv(m_sock, buf, len, flags));
}

int ODSocket::Close()
{
#ifdef WIN32
	return (closesocket(m_sock));
#else
	return (close(m_sock));
#endif
}

int ODSocket::GetError()
{
#ifdef WIN32
	return (WSAGetLastError());
#else
	return (errno);
#endif
}

bool ODSocket::IsVaild()
{
	return (m_sock != INVALID_SOCKET);
}

bool ODSocket::DnsParse(const char* domain, char* ip)
{
	struct hostent* p;
	if ( (p = gethostbyname(domain)) == NULL )
		return false;
		
	sprintf(ip, 
		"%u.%u.%u.%u",
		(unsigned char)p->h_addr_list[0][0], 
		(unsigned char)p->h_addr_list[0][1], 
		(unsigned char)p->h_addr_list[0][2], 
		(unsigned char)p->h_addr_list[0][3]);
	
	return true;
}
