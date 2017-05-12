#ifndef _ETHERHEAD_H_INCLUDE
#define _ETHERHEAD_H_INCLUDE

#pragma once
class EtherHead
{
public:
	EtherHead();
	EtherHead(const byte * pBuffer, UINT uiBufferLen);
	~EtherHead();

	void GetMacSrcAddr(char * str);

	CString GetMacSrcAddr();

	void GetMacDestAddr(char * str);

	CString GetMacDestAddr();

	CString GetType();

	int SrcAddr[6];     //源MAC地址
	int DestAddr[6];    //目的MAC地址
	int nType;           //网络层报文类型:IP,ARP,RARP
};


#endif // !_ETHERHEAD_H_INCLUDE




