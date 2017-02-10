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


ODSocket* TTSocketClient::getSocket(unsigned char bSocketType)
{
	if (bSocketType == 0)
	{
		return mSocketLogin;
	}
	else if (bSocketType == 1)
	{
		return mSocketGame;
	}
}

void TTSocketClient::closeMySocket(unsigned char bSocketType)
{
#if (PlatWhich == PlatWin)
	if (bSocketType == 0)
	{
		closesocket(mSocketLogin->m_sock);
	}
	else if (bSocketType == 1)
	{
		closesocket(mSocketGame->m_sock);
	}
#else
	if (bSocketType == 0)
	{
		close(mSocketLogin->m_sock);
	}
	else if (bSocketType == 1)
	{
		close(mSocketGame->m_sock);
	}
#endif
}

void TTSocketClient::startSocketLogin(const std::string &host)
{
	if (m_isOpenSocketLogin)
	{
		return;
	}
	m_isOpenSocketLogin = true;

	socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	std::thread t_connect([this, host]() 
	{
		mSocketLogin = ODSocket::create();
		bool isConnect = false;
		while (!isConnect)
		{
			if ((isConnect = mSocketGame->Connect(host)))
			{
				// 服务器连接成功后，通知主线程
			//	MTNotificationQueue::getInstance()->postNotification(CONNECT_SERVER_SUCCESS, NULL);
				int header = 0;
				bool rs = true;
				TCP_Info   pInfoHead;
				while (rs)
				{
					int ret = 0;
					if ((ret = mSocketLogin->Recv((char*)&pInfoHead, sizeof(TCP_Info))) <= 0)
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
						while ((packlen > 0) && ((ret = mSocketLogin->Recv((char *)(m_cbRecvBufLogin + recv_len), packlen - recv_len)) > 0))
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
				mSocketLogin->Close();
			}
			m_isOpenSocketLogin = false;
			//MTNotificationQueue::getInstance()->postNotification(DISCONNECT_GAME_SERVER_EVENT, NULL);
			return;
		}
	});

	t_connect.detach();
}
