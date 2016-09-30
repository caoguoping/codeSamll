#include "TTSocketClient.h"
#include "TTCommon.h"
#include "BasePacket.h"
#include "TTModeCenter.h"
#include "TTLoginModel.h"
#include "TTCommon.h"
#include "ConfigData.h"

TTSocketClient* TTSocketClient::m_instance = NULL;

TTSocketClient::TTSocketClient()
	: m_isOpenSocketThread(false)
	, m_isFirstConnection(true)
	, m_strLoginGameSrvID("")
{
}

TTSocketClient::~TTSocketClient()
{
	if (m_ws)
	{
		delete m_ws;
		m_ws = nullptr;
	}
}

void TTSocketClient::clear()
{
	if (m_instance == nullptr)
	{
		return;
	}
	m_instance->m_ws = nullptr;
	m_instance->m_ods.Close();

	delete m_instance;
	m_instance = nullptr;
}

void TTSocketClient::startSocket(vector<string> vecIP, vector<int> vecPort)
{
	m_strIP = vecIP.front();
	m_port = vecPort.front();

	if (m_isOpenSocketThread)
	{
		return;
	}

	m_isOpenSocketThread  = true;

	/**
	 *@:socket连接成功后，启动消息队列定时器，在每一帧的调用下，保证
	 *  不同线程间发出的消息能第一时间送达主线程
	 */
	Director::getInstance()->getScheduler()->schedule(
		schedule_selector(MTNotificationQueue::postNotifications), MTNotificationQueue::getInstance(), float(1.0 / 60.0), false);

	/**
	 *@：启动网络线程连接服务器
	 */
	thread t_connect([this](vector<string> vecIP, vector<int> vecPort) {

		ODSocket ods;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		ods.Init();
#endif

		// 设置随机种子
		timeval psv;
		gettimeofday(&psv, NULL);
		unsigned int tsrans = psv.tv_sec * 1000 + psv.tv_usec / 1000;
		srand(tsrans);

		bool isConnect = false;
		do
		{
			int idx = CCRANDOM_0_1() * vecIP.size();
			//if ((isConnect = (ods.Create(AF_INET, SOCK_STREAM, 0) && ods.ConnectIPv4(vecIP[idx].c_str(), vecPort[idx]))))
			if ((isConnect = ods.Connect(vecIP[idx].c_str(), vecPort[idx])))
			{
				// 服务器连接成功后，通知主线程
				MTNotificationQueue::getInstance()->postNotification(CONNECT_SERVER_SUCCESS, NULL);

				m_ods = ods;

				int header = 0;
				bool rs = true;
				while (rs)
				{
					int ret = 0;
					if((ret = ods.Recv((char *)&header, sizeof(header))) <= 0)
					{
						rs = false;
					}
					else
					{
						/**
						 *@:首次读取包头信息，读取正确以后解析包头，按照包头的内容读取后续的包体内容
						 *@:读取失败，则直接退出socket连接
						 */
						unsigned int packlen = header & 0x7FFFF;
						int recv_len = 0;
						NetData *pData = new NetData(packlen);
						while ((packlen > 0) && ((ret = ods.Recv((char *)(pData->m_data + recv_len), packlen - recv_len)) > 0))
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
						MTNotificationQueue::getInstance()->postNotification(RECV_DATA, pData);
					}
				}

	#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
				ods.Clean();
	#else
				ods.Close();
	#endif
			}
			m_isOpenSocketThread = false;
			MTNotificationQueue::getInstance()->postNotification(DISCONNECT_GAME_SERVER_EVENT, NULL);
			return;
		} while (!isConnect);
	}, vecIP, vecPort);

	t_connect.detach();
}

void TTSocketClient::startWebsocket(const string &strUrl)
{
	// 初始化websocket
	m_ws = new WebSocket();
	m_ws->init(*this, strUrl);
	m_strUrl = strUrl;
}

/*
void TTSocketClient::sendMsg(unsigned int protocol, char *packet, unsigned int len)
{
	int errCode = m_ods.GetError();
	if (m_ods.IsVaild() && (errCode == 0) && (protocol != 0))
	{
		PACKET_HEAD_INFO head;
		memset(&head, 0, sizeof(PACKET_HEAD_INFO));
		head.nProtocol = protocol;
		head.nPacketLen = len;

		unsigned char buf[MAX_BUFFER_LEN];
		memset(buf, 0, sizeof(buf));
		unsigned int nPackLen = sizeof(PACKET_HEAD_INFO) + len;
		head.nPacketLen = len;
		memcpy(buf, &head, sizeof(PACKET_HEAD_INFO));
		if (packet != NULL)
		{
			memcpy(buf + sizeof(PACKET_HEAD_INFO), packet, len);
		}

		m_ods.Send((const char *)buf, nPackLen);
	}
}
*/

void TTSocketClient::sendMsg(void *packet, unsigned int len)
{
	return;
	/*
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    if (m_ods.IsVaild())
#else
	//int errCode = m_ods.GetError();
	//if (m_ods.IsVaild() && (errCode == 0))
	if (m_ods.IsVaild())
#endif
	{
		m_ods.Send((const char *)packet, len);
	}
	*/
	/*
	if (len > MSG_BUFFER_MAX_LENGETH)
	{
		return;
	}
	m_ws->send((const unsigned char*)packet, len);
	*/
}

void TTSocketClient::sendMsg(MsgType type, ::google::protobuf::Message *pMsg)
{
	/*
	// 设置消息标识
	;unsigned int nFlagLen = setPackFlag(0x03);

	// 组装消息
	MsgPackage pack;
	pack.set_msg_type(type);
	if (pMsg)
	{
		pMsg->SerializeToString(pack.mutable_msg_body());
		pack.set_msg_body_len(pMsg->ByteSize());
	}

	pack.SerializeToArray(m_sendBuf + nFlagLen, SEND_BUFFER_LENGTH - nFlagLen);
	m_ws->send(m_sendBuf, nFlagLen + pack.ByteSize());
	*/

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

void TTSocketClient::onOpen(WebSocket* ws)
{
	// 连接建立成功后向gateway发送游戏服务器ID
	unsigned int nLen = setPackFlag(0x01);
	TTLoginModel *pLoginModel = (TTLoginModel*)TTModeCenter::getInstance()->getModel(ModelType::Login_Scene_Model);
	if (m_strLoginGameSrvID == "")
	{
		ServerInfo &srv = pLoginModel->getSelectedServer();
		Value val(srv.id);
		m_strLoginGameSrvID = val.asString();
	}
	memcpy(m_sendBuf + nLen, m_strLoginGameSrvID.data(), m_strLoginGameSrvID.size());
	nLen += m_strLoginGameSrvID.size();
	ws->send(m_sendBuf, nLen);

	if (m_isFirstConnection)
	{
		m_isFirstConnection = false;
		TTEventDispatch(WEBSOCKET_FIRST_CONNECTION);
	}
	else
	{
		TTCommon::onShowWaitView(false);
		TTEventDispatch(WEBSOCKET_RECONNECTION);
	}
}

void TTSocketClient::onMessage(WebSocket* ws, const WebSocket::Data& data)
{
	if (!data.isBinary)
	{
		/**
		 *@收到心跳数据后立刻响应
		 */
		unsigned int nLen = setPackFlag(0x02);
		ws->send(m_sendBuf, nLen);
	}
	else
	{
		/**
		 *@分发业务逻辑数据
		 */
		MsgPackage pack;
		if (pack.ParseFromArray(data.bytes, data.len))
		{
			NetData *pData = new NetData(pack.msg_body_len());
			//pData->m_protocol = REQUEST_RESPONSE;
			pData->m_responseCode = pack.msg_res_code();
			memcpy(pData->m_data, pack.msg_body().data(), pack.msg_body_len());
			NotificationCenter::getInstance()->postNotification(RECV_DATA, pData);
		}
	}
}

void TTSocketClient::onClose(WebSocket* ws)
{
	//提示重连
	TTEventDispatch(SCOKET_CLIENT_CONNECTION_FAILURE);
}

void TTSocketClient::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
	//提示重连
	TTEventDispatch(SCOKET_CLIENT_CONNECTION_FAILURE);
	//switch (error)
	//{
	//case WebSocket::ErrorCode::TIME_OUT:
	//	{
	//	}
	//	break;
	//case WebSocket::ErrorCode::CONNECTION_FAILURE:
	//	{
	//		TTEventDispatch(SCOKET_CLIENT_CONNECTION_FAILURE);
	//	}
	//	break;
	//default:
	//	{
	//		TTEventDispatch(SCOKET_CLIENT_CONNECTION_FAILURE);
	//	}
	//	break;
	//}
}

unsigned int TTSocketClient::setPackFlag(unsigned char flag)
{
	resetSendBuffer();
	static unsigned int nFlagLen = sizeof(unsigned char);
	memcpy(m_sendBuf, &flag, nFlagLen);
	return nFlagLen;
}

void TTSocketClient::onWSReconnect()
{
	if (m_ws)
	{
		delete m_ws;
		m_ws = nullptr;
	}

	m_ws = new WebSocket();
	if (!m_ws->init(*this, m_strUrl))
	{
		delete m_ws;
		m_ws = nullptr;
	}
}

void TTSocketClient::onCheckConnection()
{
	TTCommon::onShowWaitView(true, g_Config->getLanguage("msg_tips_277"));
	onWSReconnect();
}
