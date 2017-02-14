#include "TTSocketClient.h"

static TTSocketClient* m_instance = NULL;

TTSocketClient::TTSocketClient()
{

	m_isOpenSocketLogin = false;
	m_isOpenSocketGame = false;
	mSocketLogin = 0;
	mSocketGame = 0;
//#if (PlatWhich == PlatWin)
#ifdef WIN32
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

bool TTSocketClient::recvDateLogin()
{
	int iRetCode = 0;
	bool  isContinune;
	TCP_Info   pInfoHead;
	unsigned short wPacketSize;
	unsigned short wPacketSizeSave;

	while (mSocketLogin != 0)
	{
		iRetCode = recv(mSocketLogin, (char*)&pInfoHead, sizeof(TCP_Info), 0);
		log("cocos2d-x header %d", iRetCode);
		isContinune = true;
		if (0 == iRetCode)
		{
			log("cocos2d-x recv 0 header");
			continue;
		}
		else if (iRetCode < 0)
		{
			closeMySocket(0);
			return true;
		}
		else
		{

			wPacketSizeSave = wPacketSize = pInfoHead.wPacketSize - sizeof(TCP_Info);  //去除TCP_info后的长度
			if (wPacketSize > SOCKET_TCP_BUFFER)
			{
				continue;
			}
			if (mSocketLogin == 0)
			{
				return true;
			}
			int recv_len = 0;
			while (mSocketLogin != 0)
			{
				iRetCode = recv(mSocketLogin, (char *)m_cbRecvBufLogin + recv_len, wPacketSize - recv_len, 0);
				log("cocos2d-x Body %d", iRetCode);
				lastRcvTimeLogin = time(NULL);   //更新接收时间
				if (iRetCode == 0)
				{
					isContinune = false;
					continue;
				}
				else if (iRetCode < 0)
				{
					mSocketLogin = 0;
					return true;
				}
				recv_len += iRetCode;
				if (recv_len >= wPacketSize)
				{
					break;
				}
			}
			if (isContinune)
			{
				wPacketSize = wPacketSizeSave;
	
				unsigned short wRealySize = CrevasseBuffer(m_cbRecvBufLogin, wPacketSize);
				if (wRealySize < sizeof(TCP_Command))
				{
					continue;
				}
				char* pData = new char[SOCKET_TCP_BUFFER];
				memcpy(pData, m_cbRecvBufLogin, wPacketSize);
			//	log("new pData %p\n\n\n", pData);
				MTCustomEventQueue::getInstance()->pushCustomEvent("rcvDataLogin", pData);
			}
		}
	}
	//logV("cocos2d-x exit header m_hSocket %d", m_hSocket);
	return true;
}

void TTSocketClient::startSocketLogin(const char *ip, unsigned short port)
{
	bool isConnect = ConnectIPv4(ip, port, 0);
	if (isConnect)
	{
		std::thread recvDate(&TTSocketClient::recvDateLogin, this);
		recvDate.detach();
	}
}

bool TTSocketClient::ConnectIPv4(const char *ip, unsigned short port, unsigned char bSocketType)
{

	if (bSocketType == 0)
	{
		mSocketLogin = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	else if (bSocketType == 1)
	{
		mSocketGame = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	log("mSocketLogin %d", mSocketLogin);

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
int TTSocketClient::Send(const char* buf, int len, unsigned char bSocketType, int flags)
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

// int TTSocketClient::Recv(char* buf, int len, int flags, unsigned char bSocketType)
// {
// 	SOCKET socketTmp;
// 	if (bSocketType == 0)
// 	{
// 		socketTmp = mSocketLogin;
// 	}
// 	else if (bSocketType == 1)
// 	{
// 		socketTmp = mSocketGame;
// 	}
// 	return (recv(socketTmp, buf, len, flags));
// }

int TTSocketClient::closeMySocket(unsigned char bSocketType)
{
	SOCKET socketTmp;
	if (bSocketType == 0)
	{
		socketTmp = mSocketLogin;
		mSocketLogin = 0;
	}
	else if (bSocketType == 1)
	{
		socketTmp = mSocketGame;
		mSocketGame = 0;
	}
//#if (PlatWhich == PlatWin)
#ifdef WIN32
	return (closesocket(socketTmp));
#else
	return (close(socketTmp));
#endif
	
}
