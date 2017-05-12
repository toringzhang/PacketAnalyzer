#include "stdafx.h"
#include "ARPGram.h"


ARPGram::ARPGram()
{
}

ARPGram::ARPGram(const byte *pBuffer, UINT uiBufferLen)
{
	//定义游走指针
	byte *pPos = (byte *)pBuffer;
	nHdwAddrType = (*pPos) * 0x100 + (*(pPos + 1));

	pPos += 2;
	nPrtAddrType = (*pPos) * 0x100 + (*(pPos + 1));

	pPos += 2;
	nHdwAddrLen = (*pPos);

	pPos++;
	nPrtAddrLen = (*pPos);

	pPos++;
	nOperation = (*pPos) * 0x100 + (*(pPos + 1));

	pPos += 2;
	for (int i = 0; i<6; i++) 
	{
		SrcHdwAddr[i] = BYTE(*pPos);
		pPos++;
	}

	nSrcPrtAddr = (*pPos) * 0x1000000 + (*(pPos + 1)) * 0x10000 + (*(pPos + 2)) * 0x100 + (*(pPos + 3));

	pPos += 4;
	for (int i = 0; i<6; i++) 
	{
		DestHdwAddr[i] = BYTE(*pPos);
		pPos++;
	}

	nDestPrtAddr = (*pPos) * 0x1000000 + (*(pPos + 1)) * 0x10000 + (*(pPos + 2)) * 0x100 + (*(pPos + 3));

}

ARPGram::~ARPGram()
{
}

//获得源硬件地址
CString ARPGram::GetSrcHdwAddr()
{
	CString str;
	str.Format(L"%02X:%02X:%02X:%02X:%02X:%02X", SrcHdwAddr[0], SrcHdwAddr[1], SrcHdwAddr[2], SrcHdwAddr[3], SrcHdwAddr[4], SrcHdwAddr[5]);
	return str;
}
//获得源IP地址
CString ARPGram::GetSrcPrtAddr()
{
	CString str;
	byte *p;

	p = (byte *)&nSrcPrtAddr;
	str.Format(L"%u.%u.%u.%u", p[3], p[2], p[1], p[0]);
	return str;
}

//获得目的站点硬件地址
CString ARPGram::GetDestHdwAddr()
{
	CString str;
	str.Format(L"%02X:%02X:%02X:%02X:%02X:%02X", DestHdwAddr[0], DestHdwAddr[1], DestHdwAddr[2], DestHdwAddr[3], DestHdwAddr[4], DestHdwAddr[5]);
	return str;
}

CString ARPGram::GetDestPrtAddr()
{
	CString str;
	byte *p;

	p = (unsigned char *)&nDestPrtAddr;
	str.Format(L"%u.%u.%u.%u", p[3], p[2], p[1], p[0]);
	return str;
}

//获得操作类型
CString ARPGram::GetOperation()
{
	CString str;
	switch (this->nOperation) 
	{
	case 1:
		str.Format(L"ARP Request");  //ARP请求报文
		break;
	case 2:
		str.Format(L"ARP Reply");    //ARP回答报文
		break;
	default:
		str.Format(L"");
	}
	return str;
}
//获得协议类型
CString ARPGram::GetPrtType()
{
	CString str;
	switch (nPrtAddrType) 
	{
	case 0x0800:
		str.Format(L"IP");              //IP报文
		break;
	default:
		str.Format(L"");
	}
	return str;
}
