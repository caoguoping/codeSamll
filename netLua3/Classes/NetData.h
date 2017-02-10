#ifndef  _NET_DATA_H_
#define  _NET_DATA_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "TTSocketClient.h"

USING_NS_CC;




class  DataRcv:public Ref
{
public:
	unsigned short wMain;
	unsigned short wSub;
	char*  phead;
	char*  pNow;
public:
	static DataRcv * create(char*  pData = NULL);  //dataΪ���ݣ�������������
	void destroys(void);
	unsigned short getMainCmd();
	unsigned short getSubCmd();
	char readInt8();
	short readInt16();
	int readInt32();
	unsigned char readByte();
	unsigned short readWORD();
	unsigned int readDWORD();
	long long readUInt64();
	double readDouble();
	std::string readString(int len);
};

class  DataSnd :public Ref
{
public:
	char pBuf[16384];
	char*  pNow;
	unsigned short wPos;
	unsigned short wDataSize;   //ȫ������,����TCP_Head 8�ֽ�
	//unsigned char bSocketType;   //0:login,   1:game
public:
	static DataSnd * create(unsigned short wMainSub, unsigned short wSub);
	void destroys(void);
	unsigned short getDataSize();
	void sendData(unsigned char bSocketType);
	void wrInt8(char);
	void wrInt16(short);
	void wrInt32(int);
	void wrInt64(long long);
	void wrByte(unsigned char);
	void wrWORD(unsigned short);
	void wrDWORD(unsigned int);
	void wrDouble(double);
	void wrString(std::string str, int len);
};	 

#endif