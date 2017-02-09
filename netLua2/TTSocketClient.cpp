#include "TTSocketClient.h"

static TTSocketClient* m_instance = NULL;

TTSocketClient::TTSocketClient()
	: m_isOpenSocketThread(false)
	, m_isFirstConnection(true)
	, m_strLoginGameSrvID("")
{
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



void TTSocketClient::clear()
{
	if (m_instance == nullptr)
	{
		return;
	}
	m_instance->m_ods.Close();

	delete m_instance;
	m_instance = nullptr;
}

ODSocket TTSocketClient::getOdSocket(unsigned char bSocketType)
{
	if (bSocketType == 0)
	{
		return m_ods;
	}
	else if (bSocketType == 1)
	{
		return m_ods;
	}
}

void TTSocketClient::startSocket(const std::string &host)
{
	m_strIP = host;

	if (m_isOpenSocketThread)
	{
		return;
	}

	m_isOpenSocketThread = true;

	/**
	*@：启动网络线程连接服务器
	*/
	thread t_connect([this, host]() {

		ODSocket ods;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		ods.Init();
#endif

		bool isConnect = false;
		do
		{
			if ((isConnect = ods.Connect(host)))
			{
				// 服务器连接成功后，通知主线程
			//	MTNotificationQueue::getInstance()->postNotification(CONNECT_SERVER_SUCCESS, NULL);

				m_ods = ods;

				int header = 0;
				bool rs = true;
				TCP_Info   pInfoHead;
				while (rs)
				{
					int ret = 0;
					if ((ret = ods.Recv((char*)&pInfoHead, sizeof(TCP_Info))) <= 0)
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
						while ((packlen > 0) && ((ret = ods.Recv((char *)(m_cbRecvBuf + recv_len), packlen - recv_len)) > 0))
						{
							recv_len += ret;
							if (packlen == recv_len)
							{
								packlen = 0;
								break;
							}
						}

						//解密数据//
						unsigned short wRealySize = CrevasseBuffer(m_cbRecvBuf, packetSize);
						if (wRealySize < sizeof(TCP_Command))
						{
							continue;
						}

						unsigned char* pData = new unsigned char[packetSize];

						/**
						*@:消息接收成功后发往主线程
						*/
						MTNotificationQueue::getInstance()->postNotification("rcv_loginData", pData);
					}
				}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
				ods.Clean();
#else
				ods.Close();
#endif
			}
			m_isOpenSocketThread = false;
			//MTNotificationQueue::getInstance()->postNotification(DISCONNECT_GAME_SERVER_EVENT, NULL);
			return;
		} while (!isConnect);
	});

	t_connect.detach();
}


void TTSocketClient::sendData(unsigned short wMainCmdID, unsigned short wSubCmdID, void * pData, unsigned short wDataSize)
{
	if (wDataSize > SOCKET_TCP_BUFFER) return;

	//构造数据
	unsigned char cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head* pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (wDataSize > 0)
	{
		memcpy(cbDataBuffer + sizeof(TCP_Head), pData, wDataSize);
	}
	EncryptBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize);
	m_ods.Send((char *)cbDataBuffer, sizeof(TCP_Head)+wDataSize);
}

