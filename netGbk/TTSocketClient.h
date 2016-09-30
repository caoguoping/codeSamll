#ifndef _TT_SOCKET_CLIENT_H_
#define _TT_SOCKET_CLIENT_H_

#include "MTNotificationQueue.h"
#include <thread>
#include "ODScoket.h"
#include "DataStruct.h"
#include "network/WebSocket.h"
#include "MessageType.pb.h"
#include "MsgPackage.pb.h"

using namespace cocos2d;
using namespace network;

#define SEND_BUFFER_LENGTH	2048

struct PACKET_HEAD_INFO
{
	// Э��汾��
	unsigned int  nVersion;
	// �����ܳ���
	unsigned int  nPacketLen;
	// Э������
	unsigned int  nProtocol;
};

class TTSocketClient
	: public WebSocket::Delegate
{
private:
	TTSocketClient();
	~TTSocketClient();

public:
	static TTSocketClient* getInstance()
	{
		if (m_instance == NULL)
		{
			m_instance = new TTSocketClient;
		}

		return m_instance;
	}

	static void clear();

	void startSocket(vector<string> vecIP, vector<int> vecPort);
	void startWebsocket(const string &strUrl);
	//void sendMsg(unsigned int protocol, char *packet, unsigned int len);
	void sendMsg(void *packet, unsigned int len);
	void sendMsg(MsgType type, ::google::protobuf::Message *pMsg);

	/************************************************************************/
	/* websocket delelagate                                                 */
	/************************************************************************/
	virtual void onOpen(WebSocket* ws);
	virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);
	virtual void onClose(WebSocket* ws);
	virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);

	void onCheckConnection();

private:
	void resetSendBuffer()
	{
		memset(m_sendBuf, 0, SEND_BUFFER_LENGTH);
	}

	 /************************************
	 * Author   : young
	 * Date     : 2016/02/01
	 * Method   : setPackFlag
	 * Describe : �������ݰ��ı�ʶλ
	 * Returns  : unsigned int
	 * Parameter: 
	 *	@unsigned char flag: 
	 *					0x01: ��gateway���͵�¼����Ϸ������ID
	 *					0x02: ������Ӧ
	 *					0x03: ����ҵ����Ϣ
	 ************************************/
	 unsigned int setPackFlag(unsigned char flag);

	 /************************************
	 * Author   : young
	 * Date     : 2016/02/21
	 * Method   : onWSReconnect
	 * Describe : websocket��������
	 * Returns  : void
	 * Parameter: 
	 ************************************/
	 void onWSReconnect();

private:
	static TTSocketClient*	m_instance;
	ODSocket m_ods;
	bool					m_isOpenSocketThread;
	WebSocket				*m_ws;
	bool					m_isFirstConnection;
	unsigned char			m_sendBuf[SEND_BUFFER_LENGTH];
	string					m_strUrl;
	string					m_strLoginGameSrvID;

	string					m_strIP;
	unsigned int			m_port;
};

#endif // !_TT_SOCKET_CLIENT_H_
