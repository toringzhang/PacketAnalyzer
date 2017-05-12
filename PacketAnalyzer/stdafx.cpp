
// stdafx.cpp : 只包括标准包含文件的源文件
// PacketAnalyzer.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


void iptos(u_long in, char output[])
{
	u_char *p;

	p = (u_char *)&in;
	sprintf_s(output, 12,"%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
}