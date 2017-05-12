
// PacketAnalyzerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PacketAnalyzer.h"
#include "PacketAnalyzerDlg.h"
#include "afxdialogex.h"
#include "NetWorkDlg.h"
#include "EtherHead.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_PACKET_RECV WM_USER+100

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPacketAnalyzerDlg 对话框



CPacketAnalyzerDlg::CPacketAnalyzerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PACKETANALYZER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_alldevs = NULL;
	m_dev = NULL;
	m_bStart = FALSE;
}

void CPacketAnalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DETAILED_INFO, m_CtlEdtDetailPack);
	DDX_Control(pDX, IDC_EDIT_PACKET_INFO, m_CtlEdtCurPack);
	DDX_Control(pDX, IDC_LIST_PACKETES, m_CtlList);
}

BEGIN_MESSAGE_MAP(CPacketAnalyzerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_NETWORK, OnNetwork)
	ON_COMMAND(IDM_START, &CPacketAnalyzerDlg::OnStart)
	ON_MESSAGE(WM_PACKET_RECV, &CPacketAnalyzerDlg::OnPacketRecv)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PACKETES, &CPacketAnalyzerDlg::OnLvnItemchangedListPacketes)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

WCHAR* g_FmtEditStr = L"序号： %d    协议类型：%s\r\n长度： %s  字节\r\n源地址：%s\r\n目的地址：%s";
// CPacketAnalyzerDlg 消息处理程序

BOOL CPacketAnalyzerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_Menu.LoadMenu(IDR_MENU_MAIN);
	SetMenu(&m_Menu);

	m_CtlList.SetHeaderHeight(0.8f);          //设置头部高度
	m_CtlList.SetHeaderFontHW(18, 0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_CtlList.SetHeaderTextColor(RGB(10, 50, 255)); //设置头部字体颜色
	m_CtlList.SetHeaderBKColor(130, 180, 230, 0);   //设置头部背景色

	m_CtlList.SetExtendedStyle(m_CtlList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_CtlList.InsertColumn(0, L"序号", LVCFMT_CENTER, 80);
	m_CtlList.InsertColumn(1, L"协议类型", LVCFMT_CENTER, 110);
	m_CtlList.InsertColumn(2, L"源地址", LVCFMT_CENTER, 180);
	m_CtlList.InsertColumn(3, L"目的地址", LVCFMT_CENTER, 180);
	m_CtlList.InsertColumn(4, L"长度", LVCFMT_CENTER, 115);


	m_BkBmp.LoadBitmap(IDB_BITMAP_BORDER);

	VERIFY(m_Font.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		L"微软雅黑"));                 // lpszFacename

	m_CtlList.SetFont(&m_Font, FALSE);
	m_CtlEdtCurPack.SetFont(&m_Font, FALSE);
	m_CtlEdtDetailPack.SetFont(&m_Font, FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPacketAnalyzerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPacketAnalyzerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPacketAnalyzerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPacketAnalyzerDlg::OnNetwork()
{
	CNetWorkDlg dlg(this);
	dlg.m_alldevs = m_alldevs;
	dlg.m_dev = m_dev;
	if (dlg.DoModal() == IDOK)
	{
		ReleasePacket();
		m_alldevs = dlg.m_alldevs;
		m_dev = dlg.m_dev;
	}
}

void CPacketAnalyzerDlg::ReleasePacket()
{
	m_CtlList.DeleteAllItems();
	int isize = m_apkts.GetSize();
	for (int i = 0; i<isize; i++)
	{
		delete[] m_apkts.GetAt(i)->ppkt_data;
		delete m_apkts.GetAt(i);
	}
	m_apkts.RemoveAll();
}

CRITICAL_SECTION g_CS;
BOOL g_bCompleted = TRUE;
void pcap_handle(u_char *user, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data)
{
	if (((CPacketAnalyzerDlg*)AfxGetApp()->GetMainWnd())->m_bStart == TRUE)
	{
		// 通知主窗口收到了一个包
		PostMessage(((CPacketAnalyzerDlg*)AfxGetApp()->GetMainWnd())->m_hWnd,
			WM_PACKET_RECV,
			(WPARAM)pkt_header,
			(LPARAM)pkt_data);
	}
}
//解析报文
afx_msg LRESULT CPacketAnalyzerDlg::OnPacketRecv(WPARAM wParam, LPARAM lParam)
{
	// 保存收到的包
	packet_info* ppkti = new packet_info;
	ppkti->pkt_hdr.caplen = ((pcap_pkthdr*)wParam)->caplen;
	ppkti->pkt_hdr.len = ((pcap_pkthdr*)wParam)->len;
	ppkti->ppkt_data = new u_char[ppkti->pkt_hdr.len];
	memcpy((void*)ppkti->ppkt_data, (u_char*)lParam, ppkti->pkt_hdr.len);
	m_apkts.Add(ppkti);

	//进行判断包类型
	u_char* pPos = ppkti->ppkt_data;
	pPos += 12;
	int itype = (*pPos) * 0x100;
	pPos++;
	itype += (*pPos);

	// 解析收到的包
	int index = m_apkts.GetSize() - 1;
	CString szPktInfo(""), szIndex, szType(""), szLen(""), szSrcAddr(""), szDstAddr("");
	szIndex.Format(L"%d", index);
	m_CtlList.InsertItem(index, szIndex);
	switch (itype)
	{
	//IP报文
	case IP:
	{
		IPGram ip(ppkti->ppkt_data + 14, ppkti->pkt_hdr.len - 14);
		szType = ip.GetService();
		szLen.Format(L"%d", ip.uiTotallen);
		szSrcAddr = ip.GetSrcAddr();
		szDstAddr = ip.GetDestAddr();
		break;
	}
	//ARP报文
	case ARP:
	{
		ARPGram arp(ppkti->ppkt_data + 14, ppkti->pkt_hdr.len - 14);
		szType = "ARP";
		szSrcAddr = arp.GetSrcPrtAddr();
		szDstAddr = arp.GetDestPrtAddr();
		break;
	}
	//RARP报文
	case RARP:
	{
		RARPGram rarp(ppkti->ppkt_data + 14, ppkti->pkt_hdr.len - 14);
		szType = "RARP";
		szSrcAddr = rarp.GetSrcPrtAddr();
		szDstAddr = rarp.GetDestPrtAddr();
		break;
	}
	default:
		break;
	}
	m_CtlList.SetItemText(index, 1, szType);
	m_CtlList.SetItemText(index, 2, szSrcAddr);
	m_CtlList.SetItemText(index, 3, szDstAddr);
	m_CtlList.SetItemText(index, 4, szLen);
	szPktInfo.Format(g_FmtEditStr, index, szType, szLen, szSrcAddr, szDstAddr);
	m_CtlEdtCurPack.SetWindowText(szPktInfo);

	EnterCriticalSection(&g_CS);
	g_bCompleted = TRUE;
	LeaveCriticalSection(&g_CS);

	return 0;
}

//接收包线程
UINT RecvPackThreaddProc(LPVOID lParam)
{
	CPacketAnalyzerDlg* pMainDlg = (CPacketAnalyzerDlg*)lParam;
	pcap_t *p;
	char errbuf[PCAP_ERRBUF_SIZE];

	// 打开需要监听的适配器
	if ((p = pcap_open_live(pMainDlg->m_dev->name, 65536, 1, 1000, errbuf)) == NULL)
	{
		pMainDlg->MessageBox(L"适配器打开失败!");
		return -1;
	}
	//开启临界区
	InitializeCriticalSection(&g_CS);
	BOOL bCompleted;
	g_bCompleted = TRUE;
	pMainDlg->m_bStart = TRUE;
	do //进行抓包,每抓一个,调用pcap_handle函数
	{
		EnterCriticalSection(&g_CS);
		bCompleted = g_bCompleted;
		g_bCompleted = FALSE;
		LeaveCriticalSection(&g_CS);
		if (bCompleted)
		{
			pcap_loop(p, 1, pcap_handle, NULL);
		}

	} while (pMainDlg->m_bStart);
	//退出临界区
	DeleteCriticalSection(&g_CS);
	pcap_close(p);//关闭网卡
	return 0;
}


void CPacketAnalyzerDlg::OnStart()
{
	// TODO: 在此添加命令处理程序代码
	if (m_dev == NULL)
	{
		CNetWorkDlg dlg(this);
		dlg.m_alldevs = m_alldevs;
		dlg.m_dev = m_dev;
		if (dlg.DoModal() == IDOK)
		{
			ReleasePacket();
			m_alldevs = dlg.m_alldevs;
			m_dev = dlg.m_dev;
		}
		else
			return;
	}

	if (m_bStart == TRUE && m_apkts.GetSize() > 0)
	{
		AfxMessageBox(L"已经开始了！");
		return;
	}

	m_bStart = FALSE;
	AfxBeginThread(AFX_THREADPROC(RecvPackThreaddProc), (LPVOID)this);
}


CString CPacketAnalyzerDlg::ParsePacket(const packet_info& pkti)
{
	CString retStr;
	//Init for Ether 解析以太帧
	EtherHead ether(pkti.ppkt_data, 14);
	retStr = L"目标MAC地址:  " + ether.GetMacDestAddr();
	retStr += L"\r\n源MAC地址:  " + ether.GetMacSrcAddr();
	retStr += L"\r\n以太网帧类型:  ";

	switch (ether.nType)
	{
	case IP:
	{
		IPGram ip(pkti.ppkt_data + 14, pkti.pkt_hdr.len);
		CString stripversion;
		CString stripIHL;
		CString stripprecedence;
		CString stripservicetype;
		CString striptotallen;
		CString stripid;
		CString stripfragoffset;
		CString stripTTL;
		CString stripproto;
		CString stripchecksum;
		CString stripdatalen;
		CString stripopt;
		CString strData("\r\n数据内容：");

		CString strTmp;
		for (int i = 0; i<ip.nDataLen; i++)
		{
			if (i % 15 == 0) strData += "\r\n";
			strTmp.Format(L"%02X ", ip.pData[i]);
			strData += strTmp;
		}

		stripversion.Format(L"%d", ip.nVersion);
		stripIHL.Format(L"%d", ip.nIHL * 4);
		stripservicetype.Format(L"%d", ip.nServiceType);
		stripprecedence.Format(L"%d", ip.nPrecedence);
		striptotallen.Format(L"%d", ip.uiTotallen);
		stripid.Format(L"%d", ip.nIdentification);
		stripopt.Format(L"%d", ip.nOptionLen);
		stripfragoffset.Format(L"%d", ip.nFragmentOffset);
		stripTTL.Format(L"%d", ip.nTTL);
		stripproto.Format(L"%d", ip.nProtocol);
		stripchecksum.Format(L"0x%X", ip.uiChecksum);
		stripdatalen.Format(L"%d", ip.nDataLen);

		retStr += L"IP包";
		retStr += L"\r\nIP版本号:  " + stripversion;
		retStr += L"\r\nIP头长度 :  " + stripIHL + L" Byte";
		retStr += L"\r\nIP服务类型:  " + stripservicetype;
		retStr += L"\r\n延迟:  ";
		retStr += ip.bDelay ? L"TRUE" : L"FALSE";
		retStr += L"\r\n吞吐量:  ";
		retStr += ip.bThroughtPut ? L"TRUE" : L"FALSE";
		retStr += L"\r\n可靠性:  ";
		retStr += ip.bReliability ? L"TRUE" : L"FALSE";
		retStr += L"\r\nIP总长度:  " + striptotallen + L" Byte";
		retStr += L"\r\nIP包标识:  " + stripid;
		retStr += L"\r\nIP包分片标志(DF):  ";
		retStr += ip.bDF ? L"TRUE" : L"FALSE";
		retStr += L"\r\nIP包分片标志(MF):  ";
		retStr += ip.bMF ? L"TRUE" : L"FALSE";
		retStr += L"\r\nIP包分片偏移:  " + stripfragoffset;
		retStr += L"\r\nIP包生存时间:  " + stripTTL + L" Second";
		retStr += L"\r\nIP协议: " + stripproto + L"  " + ip.GetService();
		retStr += L"\r\nIP头部校验和:  " + stripchecksum;
		retStr += L"\r\nIP包源IP:  " + ip.GetSrcAddr();
		retStr += L"\r\nIP包目标IP:  " + ip.GetDestAddr();
		retStr += L"\r\nIP数据:  " + stripdatalen + L" Byte\r\n";
		retStr += strData;
	}
	break;
	case ARP:
	{
		ARPGram arp(pkti.ppkt_data + 14, pkti.pkt_hdr.len - 14);
		CString strarphdwtype;
		CString strarpprttype;
		CString strarphdwaddrlen;
		CString strarpprtaddrlen;
		strarphdwtype.Format(L"%u", arp.nHdwAddrType);
		strarpprttype.Format(L"%u", arp.nPrtAddrType);
		strarphdwaddrlen.Format(L"%u", arp.nHdwAddrLen);
		strarpprtaddrlen.Format(L"%u", arp.nPrtAddrLen);
		retStr += L"ARP包";
		retStr += L"\r\n硬件地址类型:  " + strarphdwtype;
		retStr += L"\r\nARP协议地址类型: " + strarpprttype + "  " + arp.GetPrtType();
		retStr += L"\r\n硬件地址长度:  " + strarphdwaddrlen;
		retStr += L"\r\n协议地址长度:  " + strarpprtaddrlen;
		retStr += L"\r\n操作类型:  " + arp.GetOperation();
		retStr += L"\r\nARP包发送方MAC:  " + arp.GetSrcHdwAddr();
		retStr += L"\r\nARP包发送方IP:  " + arp.GetSrcPrtAddr();
		retStr += L"\r\nARP包接收方MAC:  " + arp.GetDestHdwAddr();
		retStr += L"\r\nARP包接收方IP:  " + arp.GetDestPrtAddr();
	}
	break;
	case RARP:
	{
		RARPGram rarp(pkti.ppkt_data + 14, pkti.pkt_hdr.len - 14);
		CString strrarphdwtype;
		CString strrarpprttype;
		CString strrarphdwaddrlen;
		CString strrarpprtaddrlen;
		strrarphdwtype.Format(L"%u", rarp.nHdwAddrType);
		strrarpprttype.Format(L"%u", rarp.nPrtAddrType);
		strrarphdwaddrlen.Format(L"%u", rarp.nHdwAddrLen);
		strrarpprtaddrlen.Format(L"%u", rarp.nPrtAddrType);
		retStr += L"RARP包";
		retStr += L"\r\n硬件地址类型:  " + strrarphdwtype;
		retStr += L"\r\nRARP协议地址类型:  " + strrarpprttype + L"  " + rarp.GetPrtType();
		retStr += L"\r\n硬件地址长度:  " + strrarphdwaddrlen;
		retStr += L"\r\n协议地址长度:  " + strrarpprtaddrlen;
		retStr += L"\r\nHardware Address Type:  " + rarp.GetOperation();
		retStr += L"\r\nRARP包发送方MAC:  " + rarp.GetSrcHdwAddr();
		retStr += L"\r\nRARP包发送方IP:  " + rarp.GetSrcPrtAddr();
		retStr += L"\r\nRARP包接收方MAC:  " + rarp.GetDestHdwAddr();
		retStr += L"\r\nRARP包接收方IP:  " + rarp.GetDestPrtAddr();
	}
	default: retStr += L"未知"; break;
	}

	return retStr;
}


void CPacketAnalyzerDlg::OnLvnItemchangedListPacketes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	// TODO: 在此添加控件通知处理程序代码
	m_CtlEdtDetailPack.SetWindowText(ParsePacket(*m_apkts.GetAt(pNMLV->iItem)));
	
	*pResult = 0;
}


HBRUSH CPacketAnalyzerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == &m_CtlEdtCurPack || pWnd == &m_CtlEdtDetailPack)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		return CreateSolidBrush(RGB(255, 255, 255));
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CPacketAnalyzerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcWnd;
	GetClientRect(&rcWnd);
	CDC pDCTmp;
	pDCTmp.CreateCompatibleDC(pDC);
	pDCTmp.SelectObject(m_BkBmp);

	BITMAP bm;
	m_BkBmp.GetObject(sizeof(BITMAP), &bm);
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &pDCTmp, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	return CDialogEx::OnEraseBkgnd(pDC);
}
