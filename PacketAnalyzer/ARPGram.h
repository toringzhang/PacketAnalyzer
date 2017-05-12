#ifndef _ARPGRAM_H_INCLUDE
#define _ARPGRAM_H_INCLUDE

#pragma once

/*
ARP报文格式
0           8           16           24             31
+++++++++++++++++++++++++++++++++++++++++++++++++++++
|        硬件类型        |          协议类型          |
|硬件地址长度|协议地址长度|          操作类型          |
|              发送站点硬件地址                       |
|                       |           IP地址           |
|                       |     接收站点硬件地址        |
|                                                    |
|                 目的站点IP地址                      |
++++++++++++++++++++++++++++++++++++++++++++++++++++++

*/


class ARPGram
{
public:
	ARPGram();
	ARPGram(const byte * pBuffer, UINT uiBufferLen);
	~ARPGram();

	CString GetSrcHdwAddr();

	CString GetSrcPrtAddr();

	CString GetDestHdwAddr();

	CString GetDestPrtAddr();

	CString GetOperation();

	CString GetPrtType();


	int  nHdwAddrType;     //硬件地址类型
	int  nPrtAddrType;     //协议地址类型
	int  nHdwAddrLen;      //硬件地址长度
	int  nPrtAddrLen;      //协议地址长度
	int  nOperation;       //操作类型
	BYTE SrcHdwAddr[6];    //发送站点硬件地址
	int  nSrcPrtAddr;      //发送站点IP地址
	BYTE DestHdwAddr[6];   //目的站点硬件地址
	int  nDestPrtAddr;     //目的站点IP地址

};

#endif