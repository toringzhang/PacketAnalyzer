#ifndef _PACKET_TYPE_H_INCLUDE
#define _PACKET_TYPE_H_INCLUDE
#pragma once

#include "Include/pcap.h"
#include <Windows.h>
#define MAX_PROTO_TEXT_LEN 16 // 子协议名称最大长度
#define MAX_PROTO_NUM 12 // 子协议数量

// 定义数据包协议类型
#define IP 0x0800
#define ARP 0x0806
#define RARP 0x8035

// 定义IP地址结构
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

// 定义IP首部格式
typedef struct ip_header
{
	u_char	ver_ihl;		// 版本和首部长度
	u_char	tos;			// 服务类型 
	u_short tlen;			// 总长度 
	u_short identification; // 标识号
	u_short flags_fo;		// 段偏移量
	u_char	ttl;			// 生存时间
	u_char	proto;			// 协议
	u_short crc;			// 首部校验和
	DWORD	saddr;		    // 源IP地址
	DWORD	daddr;		    // 目的地址
	u_int	op_pad;			// 选项和填补位
}ip_header;

// 定义TCP首部格式
typedef struct tcp_header
{
	unsigned short th_sport;  // 源端口号        
	unsigned short th_dport;  // 目的端口号       
	unsigned int th_seq;      // SEQ序号    
	unsigned int th_ack;      // ACK序号
	unsigned char th_lenres;  //  首部长度    
	unsigned char th_flag;    // 控制位  
	unsigned short th_win;    // 窗口大小        
	unsigned short th_sum;    // 校验和        
	unsigned short th_urp;    // 紧急指针         
}tcp_header;

// 定义UDP首部格式
typedef struct udp_header
{
	u_short sport;			// 16位源端口
	u_short dport;			// 16位目的端口
	u_short len;			// 16位长度
	u_short crc;			// 16位校验和
}udp_header;

// 定义ICMP首部格式
typedef struct icmp_header
{
	BYTE i_type; // 8位类型
	BYTE i_code; // 8位代码
	unsigned short i_cksum; // 16位校验和
	unsigned short i_id; // 识别号
	unsigned short i_seq; // 报文序列号
	unsigned long timestamp; // 时间戳
}icmp_header;

// 定义子协议映射表
typedef struct _protomap
{
	int ProtoNum;
	char ProtoText[MAX_PROTO_TEXT_LEN];
}PROTOMAP;

// 为子协议映射表赋值
PROTOMAP ProtoMap[MAX_PROTO_NUM] = 
{
	{ IPPROTO_IP,"IP" },
	{ IPPROTO_ICMP,"ICMP" },
	{ IPPROTO_IGMP,"IGMP" },
	{ IPPROTO_GGP,"GGP" },
	{ IPPROTO_TCP,"TCP" },
	{ IPPROTO_PUP,"PUP" },
	{ IPPROTO_UDP,"UDP" },
	{ IPPROTO_IDP,"IDP" },
	{ IPPROTO_ND,"ND" },
	{ IPPROTO_RAW,"RAW" },
	{ IPPROTO_MAX,"MAX" },
	{ NULL,"" }
};


char TcpFlag[6] = { 'F','S','R','P','A','U' }; // TCP标志位

#endif