#include "stdafx.h"
#include "IPGram.h"


IPGram::IPGram()
{
}

IPGram::IPGram(const byte *pBuffer, UINT uiBufferLen)
{
	//定义一个游走指针
	byte *pPos;

	pPos = (byte*)pBuffer;
	//首字节除以16，商为版本号，余数为首部长度，单位4字节
	nVersion = *pPos / 16;
	nIHL = *pPos % 16;

	//第二字节为服务类型
	//[优先级|D|T|R|C| ]
	pPos++;
	nServiceType = *pPos;
	//
	nPrecedence = (*pPos & 16) * 8 + (*pPos & 8) * 4 + (*pPos & 4) * 2 + (*pPos & 2);
	if (*pPos & 16)
		bDelay = true;            //最小延迟
	else
		bDelay = false;
	if (*pPos & 8)
		bThroughtPut = true;      //最大吞吐率
	else
		bThroughtPut = false;
	if (*pPos & 4)
		bReliability = true;      //最高可靠性
	else
		bReliability = false;
	if (*pPos & 2)
		bCost = true;             //最低成本
	else
		bCost = false;
	//包长 2字节 单位：字节
	pPos++;
	uiTotallen = (*pPos) * 256 + (*(pPos + 1));

	//标识 2字节
	pPos += 2;
	nIdentification = (*pPos) * 256 + (*(pPos + 1));
	//标志 3位 这里表明是否分片
	pPos += 2;
	if (*pPos & 64)
		bDF = true;     //不分片
	else
		bDF = false;
	if (*pPos & 32)
		bMF = true;     //分片
	else
		bMF = false;

	//片偏移 13位
	nFragmentOffset = (*pPos % 32) * 256 + (*(pPos + 1));
	
	//TTL
	pPos += 2;
	nTTL = *pPos;
	//协议
	pPos++;
	nProtocol = *pPos;
	//校验和
	pPos++;
	uiChecksum = (*pPos) * 256 + (*(pPos + 1));
	//源IP
	pPos += 2;
	lSrcAddr = (*pPos) * 0x1000000 + (*(pPos + 1)) * 0x10000 + (*(pPos + 2)) * 0x100 + (*(pPos + 3));
	//目的IP
	pPos += 4;
	lDestAddr = (*pPos) * 0x1000000 + (*(pPos + 1)) * 0x10000 + (*(pPos + 2)) * 0x100 + (*(pPos + 3));

	//如果头部长度大于20字节，则说明该报文有IP可选项
	if (nIHL>5) 
	{
		pPos += 4;
		//可选部分长度
		nOptionLen = (nIHL - 5) * 4;
		//选项部分
		pOptions = new byte[nOptionLen];
		memcpy(pOptions, pBuffer + 20, nOptionLen);
	}
	else 
	{
		nOptionLen = 0;
		pOptions = NULL;
	}
	//这里将游走指针指向数据域
	pPos = (byte *)(pBuffer + nIHL * 4);
	//数据段长度 = 数据报总长度 - 头部长度
	nDataLen = uiTotallen - nIHL * 4;
	if (nDataLen>0)
	{
		pData = new byte[nDataLen];
		memcpy(pData, pBuffer + nIHL * 4, nDataLen);
	}
	else
		pData = NULL;
}

IPGram::~IPGram()
{
	if (pData != NULL)
		delete[] pData;
	if (pOptions != NULL)
		delete[] pOptions;
}

//解析协议类型
CString IPGram::GetService()
{
	CString str;

	switch (this->nProtocol)
	{
	case 6:
		str.Format(L"TCP"); break;
	case 17:
		str.Format(L"UDP"); break;
	case 1:
		str.Format(L"ICMP"); break;
	case 89:
		str.Format(L"OSPF"); break;
	default:
		str.Format(L"%d", nProtocol);
	}

	return str;
}

//解析源IP
void IPGram::GetSrcAddr(char *str)
{
	int temp = lSrcAddr;
	byte i0, i1, i2, i3;

	i0 = (temp & 0xff000000) / 0x1000000;
	i1 = (temp & 0x00ff0000) / 0x10000;
	i2 = (temp & 0x0000ff00) / 0x100;
	i3 = temp & 0x000000ff;
	sprintf_s(str, 1,"%u.%u.%u.%u", i0, i1, i2, i3);
}

//源IP
CString IPGram::GetSrcAddr()
{
	CString str;
	int temp = lSrcAddr;
	byte i0, i1, i2, i3;

	i0 = (temp & 0xff000000) / 0x1000000;
	i1 = (temp & 0x00ff0000) / 0x10000;
	i2 = (temp & 0x0000ff00) / 0x100;
	i3 = temp & 0x000000ff;
	str.Format(L"%u.%u.%u.%u", i0, i1, i2, i3);

	return str;
}

//获得目的IP
void IPGram::GetDestAddr(char *str)
{
	int temp = lDestAddr;
	byte i0, i1, i2, i3;

	i0 = (temp & 0xff000000) / 0x1000000;
	i1 = (temp & 0x00ff0000) / 0x10000;
	i2 = (temp & 0x0000ff00) / 0x100;
	i3 = temp & 0x000000ff;
	sprintf_s(str,1, "%u.%u.%u.%u", i0, i1, i2, i3);
}

CString IPGram::GetDestAddr()
{
	CString str;
	int temp = lDestAddr;
	byte i0, i1, i2, i3;

	i0 = (temp & 0xff000000) / 0x1000000;
	i1 = (temp & 0x00ff0000) / 0x10000;
	i2 = (temp & 0x0000ff00) / 0x100;
	i3 = temp & 0x000000ff;
	str.Format(L"%u.%u.%u.%u", i0, i1, i2, i3);

	return str;
}

//获得标识，每个IP数据报有一个标识，标志着这两个IP之间正在进行通信的报文
CString IPGram::GetID()
{
	CString str;
	str.Format(L"%u", this->nIdentification);
	return str;
}