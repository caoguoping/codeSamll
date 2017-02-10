#include "TTSocketClient.h"

static TTSocketClient* m_instance = NULL;

TTSocketClient::TTSocketClient()
{

	m_isOpenSocketLogin = false;
	m_isOpenSocketGame = false;

#if (PlatWhich == PlatWin)
	unsigned short wVersionRequested;
	wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet != 0)
	{
		return;
	}
#endif
}

TTSocketClient::~TTSocketClient()
{
}

TTSocketClient* TTSocketClient::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new TTSocketClient;
	}

	return m_instance;
}



//解密数据
unsigned short TTSocketClient::CrevasseBuffer(unsigned char pcbDataBuffer[], unsigned short wDataSize)
{
	if (wDataSize < sizeof(TCP_Command))
	{
		return 0;
	}
	//调整长度
	unsigned short wSnapCount = 0;
	if ((wDataSize % sizeof(unsigned int)) != 0)
	{
		wSnapCount = sizeof(unsigned int)-wDataSize % sizeof(unsigned int);
		memset(pcbDataBuffer + wDataSize, 0, wSnapCount);
	}

	for (int i = 0; i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
	}

	return wDataSize;
}



//映射接收数据
unsigned char TTSocketClient::MapRecvByte(unsigned char const cbData)
{
	unsigned char cbMap = g_RecvByteMap[cbData];
	return cbMap;
}


void TTSocketClient::startSocketLogin(const std::string &host)
{
	if (m_isOpenSocketLogin)
	{
		return;
	}
	m_isOpenSocketLogin = true;

	std::thread t_connect([this, host]() 
	{
		bool isConnect = false;
		while (!isConnect)
		{
			if ((isConnect = Connect(host, 0)))
			{
				// 服务器连接成功后，通知主线程
			//	MTNotificationQueue::getInstance()->postNotification(CONNECT_SERVER_SUCCESS, NULL);
				int header = 0;
				bool rs = true;
				TCP_Info   pInfoHead;
				while (rs)
				{
					int ret = 0;
					if ((ret = Recv((char*)&pInfoHead, sizeof(TCP_Info), 0, 0)) <= 0)
					{
						rs = false;
					}
					else
					{
						/**
						*@:首次读取包头信息，读取正确以后解析包头，按照包头的内容读取后续的包体内容
						*@:读取失败，则直接退出socket连接
						*/
						unsigned int packlen = pInfoHead.wPacketSize;
						unsigned int packetSize = packlen;
						int recv_len = 0;
						while ((packlen > 0) && ((ret = Recv((char *)(m_cbRecvBufLogin + recv_len), packlen - recv_len, 0, 0)) > 0))
						{
							recv_len += ret;
							if (packlen == recv_len)
							{
								packlen = 0;
								break;
							}
						}

						//解密数据//
						unsigned short wRealySize = CrevasseBuffer(m_cbRecvBufLogin, packetSize);
						unsigned char* pData = new unsigned char[packetSize];

						/**
						*@:消息接收成功后发往主线程
						*/
						MTNotificationQueue::getInstance()->postNotification("rcvDataLogin", pData);
					}
				}
				Close(0);
			}
			m_isOpenSocketLogin = false;
			//MTNotificationQueue::getInstance()->postNotification(DISCONNECT_GAME_SERVER_EVENT, NULL);
			return;
		}
	});

	t_connect.detach();
}


bool TTSocketClient::Connect(const char* host, unsigned short port, unsigned char bSocketType)
{
	if (bSocketType == 0)
	{
		mSocketLogin = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	} 
	else if (bSocketType == 1)
	{
		mSocketGame = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	bool isc = ConnectIPv4(host, port, bSocketType);
	return isc;
}

bool TTSocketClient::Connect(const string &host, unsigned char bSocketType)
{
	int nDot = host.find(":");
	const char*  ip = string(host.begin(), host.begin() + nDot).c_str();
	unsigned short port = atoi(string(host.begin() + nDot + 1, host.end()).c_str());
	return Connect(ip, port, bSocketType);
}

bool TTSocketClient::ConnectIPv4(const char *ip, unsigned short port, unsigned char bSocketType)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);

	SOCKET socketTmp;
	if (bSocketType == 0)
	{
		socketTmp = mSocketLogin;
	}
	else if (bSocketType == 1)
	{
		socketTmp = mSocketGame;
	}
	int ret = connect(socketTmp, (const struct sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR) 
	{
		return false;
	}
	return true;
}


//加密后的buf
int TTSocketClient::Send(const char* buf, int len, int flags, unsigned char bSocketType)
{
	int bytes;
	int count = 0;

	SOCKET socketTmp;
	if (bSocketType == 0)
	{
		socketTmp = mSocketLogin;
	}
	else if (bSocketType == 1)
	{
		socketTmp = mSocketGame;
	}

	while (count < len) {

		bytes = send(socketTmp, buf + count, len - count, flags);
		if (bytes == -1 || bytes == 0)
			return -1;
		count += bytes;
	}

	return count;
}

int TTSocketClient::Recv(char* buf, int len, int flags, unsigned char bSocketType)
{
	SOCKET socketTmp;
	if (bSocketType == 0)
	{
		socketTmp = mSocketLogin;
	}
	else if (bSocketType == 1)
	{
		socketTmp = mSocketGame;
	}
	return (recv(socketTmp, buf, len, flags));
}

int TTSocketClient::Close(unsigned char bSocketType)
{
	SOCKET socketTmp;
	if (bSocketType == 0)
	{
		socketTmp = mSocketLogin;
	}
	else if (bSocketType == 1)
	{
		socketTmp = mSocketGame;
	}
#if (PlatWhich == PlatWin)
	return (closesocket(socketTmp));
#else
	return (close(socketTmp));
#endif
}
