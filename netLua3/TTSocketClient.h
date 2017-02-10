#ifndef _TT_SOCKET_CLIENT_H_
#define _TT_SOCKET_CLIENT_H_

#include "MTNotificationQueue.h"
#include <thread>
#include "ODScoket.h"
#include "PlatFormControl.h"

using namespace cocos2d;

#define SEND_BUFFER_LENGTH	2048
#define SOCKET_TCP_BUFFER	16384	


// struct PACKET_HEAD_INFO
// {
// 	// 协议版本号
// 	unsigned int  nVersion;
// 	// 包的总长度
// 	unsigned int  nPacketLen;
// 	// 协议类型
// 	unsigned int  nProtocol;
// };

class TTSocketClient
{
public:
	TTSocketClient();
	~TTSocketClient();
	static TTSocketClient* getInstance();
	void startSocketLogin(const std::string &host);
	//void startSocketGame(const std::string &host);
	ODSocket* getSocket(unsigned char bSocketType);
	void closeMySocket(unsigned char bSocketType);

	unsigned short CrevasseBuffer(unsigned char pcbDataBuffer[], unsigned short wDataSize);									//解密数据
	unsigned char MapRecvByte(unsigned char const cbData);

	bool					m_isOpenSocketLogin;
	bool					m_isOpenSocketGame;
	ODSocket*				mSocketLogin;
	ODSocket*				mSocketGame;
	unsigned char			m_cbRecvBufLogin[SOCKET_TCP_BUFFER];		//接收缓冲
	unsigned char			m_cbRecvBufGame[SOCKET_TCP_BUFFER];		//接收缓冲
};

#endif 
