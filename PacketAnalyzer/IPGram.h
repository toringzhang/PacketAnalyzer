#ifndef _IPGRAM_H_INCLUDE
#define _IPGRAM_H_INCLUDE
/*
IP数据包格式   头部
0       4       8        16    19                   31        
++++++++++++++++++++++++++++++++++++++++++++++++++++
|  版本 |首部长度|服务类型 |      数据报总长度         |          
|        标识            | 标志 |     片偏移         |
|   生存时间     |  协议  |       首部校验和          |
|                     源IP地址                       |  
|                    目的IP地址                      |
|                      IP选项                       |
+++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
#pragma once
//解析IP数据包
class IPGram
{
public:
	int	 nVersion;	       //版本
	int  nIHL;	           //头部长度
	int  nServiceType;	   //服务类型 type of service
	int  nPrecedence;	   //优先级
	bool bDelay;	       //延迟
	bool bThroughtPut;	   //吞吐量
	bool bReliability;	   //可靠性
	bool bCost;            //最低成本
	UINT uiTotallen;	   //总长total length
	int  nIdentification;  //标识
	bool bDF;	           //不要分段
	bool bMF;	           //还有进一步的分段
	int  nFragmentOffset;  //分片偏移fragment offset
	int  nTTL;	           //生命期time to live
	int  nProtocol;	       //协议,如TCP,UDP
	UINT uiChecksum;	   //头部校验和
	long lSrcAddr;	       //源IP地址
	long lDestAddr;	       //目的IP地址
	int  nOptionLen;	   //选项长度
	byte *pOptions;	       //选项内容
	int  nDataLen;	       //数据长度
	byte *pData;	       //数据内容

	IPGram();
	IPGram(const byte* pBuffer, UINT uiBufferLen);
	virtual ~IPGram();
	CString GetService();
	void GetSrcAddr(char * str);
	CString GetSrcAddr();
	void GetDestAddr(char * str);
	CString GetDestAddr();
	CString GetID();
};

#endif // !_IPGRAM_H_INCLUDE

