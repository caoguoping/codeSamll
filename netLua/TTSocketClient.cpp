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

// 				m_ods = ods;
// 
// 				int header = 0;
// 				bool rs = true;
// 				while (rs)
// 				{
// 					int ret = 0;
// 					if ((ret = ods.Recv((char *)&header, sizeof(header))) <= 0)
// 					{
// 						rs = false;
// 					}
// 					else
// 					{
// 						/**
// 						*@:首次读取包头信息，读取正确以后解析包头，按照包头的内容读取后续的包体内容
// 						*@:读取失败，则直接退出socket连接
// 						*/
// 						unsigned int packlen = header & 0x7FFFF;
// 						int recv_len = 0;
// 						NetData *pData = new NetData(packlen);
// 						while ((packlen > 0) && ((ret = ods.Recv((char *)(pData->m_data + recv_len), packlen - recv_len)) > 0))
// 						{
// 							recv_len += ret;
// 							if (packlen == recv_len)
// 							{
// 								packlen = 0;
// 								break;
// 							}
// 						}
// 
// 						/**
// 						*@:消息接收成功后发往主线程
// 						*/
// 						MTNotificationQueue::getInstance()->postNotification("recive_data", pData);
// 					}

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

						/**
						*@:消息接收成功后发往主线程
						*/
						MTNotificationQueue::getInstance()->postNotification("recive_data", pData);
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

#if 0
void TTSocketClient::sendMsg(MsgType type, ::google::protobuf::Message *pMsg)
{
	// 组装消息
	MsgPackage pack;
	pack.set_msg_type(type);
	if (pMsg)
	{
		pMsg->SerializeToString(pack.mutable_msg_body());
		pack.set_msg_body_len(pMsg->ByteSize());
	}

	int nPackSize = pack.ByteSize();
	int header = 0;
	header = (header << 19) | nPackSize;
	memcpy(m_sendBuf, &header, sizeof(header));
	pack.SerializeToArray(m_sendBuf + sizeof(header), SEND_BUFFER_LENGTH - sizeof(header));
	m_ods.Send((char *)m_sendBuf, sizeof(header) + header);
}
#endif
