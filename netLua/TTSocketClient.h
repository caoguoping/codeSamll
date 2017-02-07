#ifndef _TT_SOCKET_CLIENT_H_
#define _TT_SOCKET_CLIENT_H_

#include "MTNotificationQueue.h"
#include <thread>
#include "ODScoket.h"

using namespace cocos2d;

#define SEND_BUFFER_LENGTH	2048
#define SOCKET_TCP_BUFFER	16384	


struct PACKET_HEAD_INFO
{
	// 协议版本号
	unsigned int  nVersion;
	// 包的总长度
	unsigned int  nPacketLen;
	// 协议类型
	unsigned int  nProtocol;
};

class TTSocketClient
{
public:
	TTSocketClient();
	~TTSocketClient();
	static TTSocketClient* getInstance();
	static void clear();
	void startSocket(const std::string &host);
//	void sendMsg(MsgType type, ::google::protobuf::Message *pMsg);
	void resetSendBuffer()
	{
		memset(m_sendBuf, 0, SEND_BUFFER_LENGTH);
	}
	bool					m_isOpenSocketThread;
	bool					m_isFirstConnection;
	unsigned char			m_sendBuf[SEND_BUFFER_LENGTH];
	string					m_strUrl;
	string					m_strLoginGameSrvID;
	string					m_strIP;
	unsigned int			m_port;
	ODSocket				m_ods;
	unsigned char			m_cbRecvBuf[SOCKET_TCP_BUFFER];		//接收缓冲
};

#endif 
