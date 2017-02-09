#include "NetData.h"
DataRcv* DataRcv::create(char*  pData)
{
	DataRcv *pDataRcv = new DataRcv();
	if (pDataRcv)
	{
		pDataRcv->phead = pData;

		memcpy(&pDataRcv->wMain, pData, 2);
		pData += 2;
		memcpy(&pDataRcv->wSub, pData, 2);
		pData += 2;

		//从纯数据开始算偏移， 不包括主副命令
		pDataRcv->pNow = pData;
		pDataRcv->autorelease();
		pDataRcv->retain();
	}
	else
	{
		CC_SAFE_DELETE(pDataRcv);
	}
	return pDataRcv;
}

void DataRcv::destroys()
{
	if (phead != NULL)
	{
		delete phead;
		phead = NULL;
		pNow = NULL;
	}
	this->release();
}

unsigned short  DataRcv::getMainCmd()
{
	return wMain;
}

unsigned short  DataRcv::getSubCmd()
{
	return wSub;
}

char DataRcv::readInt8()
{
    char i[1];
	memcpy(i, pNow, 1);
	pNow++;
	return i[0];
}


short DataRcv::readInt16()
{
	short i[1];
	memcpy(i, pNow, 2);
	pNow += 2;
	return i[0];
}

int DataRcv::readInt32()
{
	int i[1];
	memcpy(i, pNow, 4);
	pNow += 4;
	return i[0];
}


unsigned char DataRcv::readByte()
{
	unsigned char i[1];
	memcpy(i, pNow, 1);
	pNow++;
	return i[0];
}


unsigned short DataRcv::readWORD()
{
	unsigned short i[1];
	memcpy(i, pNow, 2);
	pNow += 2;
	return i[0];
}

unsigned int DataRcv::readDWORD()
{
	unsigned int i[1];
	memcpy(i, pNow, 4);
	pNow += 4;
	return i[0];
}

long long DataRcv::readUInt64()
{
	long long i[1];
	memcpy(i, pNow, 8);
	pNow += 8;
	return i[0];
}

double DataRcv::readDouble()
{
	double i[1];
	memcpy(i, pNow, 8);
	pNow += 8;
	return i[0];
}

std::string DataRcv::readString(int len)
{
	std::string s;
	char i[1500];
	memcpy(i, pNow, len);
	s = i;
	pNow += len;
	return s;
}

DataSnd* DataSnd::create(unsigned short wMainCmd, unsigned short wSubCmd, unsigned char bSocketType)
{
	DataSnd *pDataSnd = new DataSnd();
	if (pDataSnd)
	{
		pDataSnd->pNow = pDataSnd->pBuf;
		pDataSnd->bSocketType = bSocketType;

		memcpy(pDataSnd->pNow, &wMainCmd, 2);
		pDataSnd->pNow += 2;
		memcpy(&pDataSnd->pNow, &wSubCmd, 2);
		pDataSnd->pNow += 2;
		pDataSnd->wDataSize = 4;
		pDataSnd->autorelease();
		pDataSnd->retain();
	}
	else
	{
		CC_SAFE_DELETE(pDataSnd);
	}
	return pDataSnd;
}

void  DataSnd::sendData()
{





	TTSocketClient::getInstance()->getOdSocket(bSocketType).Send(pBuf, wDataSize + 4);
}

//映射发送数据
unsigned char DataSnd::MapSendByte(unsigned char const cbData)
{
	unsigned char cbMap = g_SendByteMap[(unsigned char)(cbData)];
	return cbMap;
}

//加密数据, 
unsigned short DataSnd::EncryptBuffer(unsigned char pcbDataBuffer[], unsigned short wDataSize)
{
	if (wDataSize < sizeof(TCP_Head) || wDataSize >(sizeof(TCP_Head)+SOCKET_TCP_BUFFER))
	{
		return 0;
	}
	//调整长度
	unsigned short wEncryptSize = wDataSize - sizeof(TCP_Command);
	unsigned short wSnapCount = 0;
	if ((wEncryptSize % sizeof(unsigned int)) != 0)
	{
		wSnapCount = sizeof(unsigned int)-wEncryptSize % sizeof(unsigned int);
		memset(pcbDataBuffer + sizeof(TCP_Info)+wEncryptSize, 0, wSnapCount);
	}

	//效验码与字节映射，不包括TCP_Info, 即开头4字节
	unsigned char cbCheckCode = 0;
	unsigned short i = 0;
	for (i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		cbCheckCode += pcbDataBuffer[i];
		pcbDataBuffer[i] = MapSendByte(pcbDataBuffer[i]);
	}

	//填写信息头
	TCP_Head* pHead = (TCP_Head *)pcbDataBuffer;
	pHead->TCPInfo.cbCheckCode = ~cbCheckCode + 1;
	pHead->TCPInfo.wPacketSize = wDataSize;
	return wDataSize;
}