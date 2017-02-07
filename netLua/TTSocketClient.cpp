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
	*@�����������߳����ӷ�����
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
				// ���������ӳɹ���֪ͨ���߳�
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
// 						*@:�״ζ�ȡ��ͷ��Ϣ����ȡ��ȷ�Ժ������ͷ�����հ�ͷ�����ݶ�ȡ�����İ�������
// 						*@:��ȡʧ�ܣ���ֱ���˳�socket����
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
// 						*@:��Ϣ���ճɹ��������߳�
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
						*@:�״ζ�ȡ��ͷ��Ϣ����ȡ��ȷ�Ժ������ͷ�����հ�ͷ�����ݶ�ȡ�����İ�������
						*@:��ȡʧ�ܣ���ֱ���˳�socket����
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
						*@:��Ϣ���ճɹ��������߳�
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
	// ��װ��Ϣ
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
