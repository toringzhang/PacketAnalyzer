#ifndef _RARPGRAM_H_INCLUDE
#define _RARPGRAM_H_INCLUDE

#pragma once
/*
RARP数据报格式


*/

class RARPGram
{
public:
	RARPGram();
	RARPGram(const byte * pBuffer, UINT uiBufferLen);
	~RARPGram();

	CString GetSrcHdwAddr();

	CString GetSrcPrtAddr();

	CString GetDestHdwAddr();

	CString GetDestPrtAddr();

	CString GetOperation();

	CString GetPrtType();


	int  nHdwAddrType;         //硬件地址类型
	int  nPrtAddrType;         //协议地址类型
	int  nHdwAddrLen;          //硬件地址长度
	int  nPrtAddrLen;          //协议地址长度
	int  nOperation;           //操作类型
	BYTE SrcHdwAddr[6];        //发送站点硬件地址
	int  nSrcPrtAddr;          //发送站点协议地址
	BYTE DestHdwAddr[6];       //目的站点硬件地址
	int  nDestprtAddr;         //目的站点协议地址
};


#endif // !_RARPGRAM_H_INCLUDE



