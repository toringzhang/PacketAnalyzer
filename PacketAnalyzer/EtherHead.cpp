#include "stdafx.h"
#include "EtherHead.h"


EtherHead::EtherHead()
{
}

EtherHead::EtherHead(const byte *pBuffer, UINT uiBufferLen)
{
	byte *pPos;

	pPos = (byte*)pBuffer;

	for (int i = 0; i<6; i++) 
	{
		DestAddr[i] = *pPos;
		pPos++;
	}

	for (int i = 0; i<6; i++) 
	{
		SrcAddr[i] = *pPos;
		pPos++;
	}

	nType = (*pPos) * 0x100 + (*(pPos + 1));
}

EtherHead::~EtherHead()
{
}

void EtherHead::GetMacSrcAddr(char *str)
{
	sprintf_s(str, 1,"%02X-%02X-%02X-%02X-%02X-%02X", SrcAddr[0], SrcAddr[1], SrcAddr[2], SrcAddr[3], SrcAddr[4], SrcAddr[5]);
}

CString EtherHead::GetMacSrcAddr()
{
	CString str;

	str.Format(L"%02X-%02X-%02X-%02X-%02X-%02X", SrcAddr[0], SrcAddr[1], SrcAddr[2], SrcAddr[3], SrcAddr[4], SrcAddr[5]);
	return str;
}

void EtherHead::GetMacDestAddr(char *str)
{
	sprintf_s(str, 1,"%02X-%02X-%02X-%02X-%02X-%02X", DestAddr[0], DestAddr[1], DestAddr[2], DestAddr[3], DestAddr[4], DestAddr[5]);
}

CString EtherHead::GetMacDestAddr()
{
	CString str;

	str.Format(L"%02X-%02X-%02X-%02X-%02X-%02X", DestAddr[0], DestAddr[1], DestAddr[2], DestAddr[3], DestAddr[4], DestAddr[5]);
	return str;
}

CString  EtherHead::GetType()
{
	CString str;
	switch (this->nType) 
	{
	case 2048:
		str.Format(L"IP");
		break;
	default:
		str.Format(L"");
	}
	return str;
}