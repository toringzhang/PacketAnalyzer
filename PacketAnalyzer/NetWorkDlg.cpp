// NetWorkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PacketAnalyzer.h"
#include "NetWorkDlg.h"
#include "PacketAnalyzerDlg.h"
#include "afxdialogex.h"


// CNetWorkDlg 对话框

IMPLEMENT_DYNAMIC(CNetWorkDlg, CDialogEx)

CNetWorkDlg::CNetWorkDlg(CPacketAnalyzerDlg* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_NETWORK, pParent)
{
	m_ParentDlg = pParent;
}

CNetWorkDlg::~CNetWorkDlg()
{
}

void CNetWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, m_CtlEditAdapter);
	DDX_Control(pDX, IDC_COMBO_SELECT, m_Combo);
}


BEGIN_MESSAGE_MAP(CNetWorkDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CNetWorkDlg::OnBnClickedButtonOk)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_CANCLE, &CNetWorkDlg::OnBnClickedButtonCancle)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT, &CNetWorkDlg::OnCbnSelchangeComboSelect)
END_MESSAGE_MAP()


// CNetWorkDlg 消息处理程序


BOOL CNetWorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_alldevs == NULL)
	{
		if (pcap_findalldevs(&m_alldevs, m_errbuf) == -1)
			return FALSE;
	}

	int ibak = 0;
	pcap_if_t *tempdev = m_alldevs;
	for (int i = 0; tempdev != NULL; tempdev = tempdev->next, i++)
	{
		if (m_dev == NULL) m_dev = tempdev;
		if (strcmp(tempdev->name, m_dev->name) == 0) ibak = i;
		CString v1(tempdev->description);
		//插入适配器描述
		m_Combo.InsertString(i, v1);
	}
	
	m_Combo.SetCurSel(ibak);
	if (m_dev != NULL)
	{
		CString szAdapter;
		char tmp[12] = {0}; 
		iptos(((sockaddr_in *)m_dev->addresses->addr)->sin_addr.s_addr, tmp);

		szAdapter.Format(L"Name: %S\r\nDescription: %S\r\nAddress: %S",
			m_dev->name,
			m_dev->description,
			tmp);
		m_CtlEditAdapter.SetWindowText(szAdapter);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CNetWorkDlg::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int iCurSel = m_Combo.GetCurSel();
	pcap_if_t *tempdev = m_alldevs;
	for (int i = 0; tempdev != NULL; tempdev = tempdev->next, i++)
	{
		if (iCurSel == i)
		{
			m_dev = tempdev;
			break;
		}
	}
	CDialog::OnOK();
}


HBRUSH CNetWorkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	if (pWnd == &m_CtlEditAdapter)
	{
		pDC->SetBkMode(TRANSPARENT);
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


BOOL CNetWorkDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcWnd;
	GetClientRect(&rcWnd);
	CDC pDCTmp;
	pDCTmp.CreateCompatibleDC(pDC);
	pDCTmp.SelectObject(m_ParentDlg->m_BkBmp);

	BITMAP bm;
	m_ParentDlg->m_BkBmp.GetObject(sizeof(BITMAP), &bm);
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &pDCTmp, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	return CDialogEx::OnEraseBkgnd(pDC);
}


void CNetWorkDlg::OnBnClickedButtonCancle()
{
	// TODO: 在此添加控件通知处理程序代码

	return CDialogEx::OnCancel();
}


void CNetWorkDlg::OnCbnSelchangeComboSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_Combo.GetCurSel();
	pcap_if_t *tempdev = m_alldevs;
	for (int i = 0; i<=index; tempdev = tempdev->next, i++)
	{
		m_dev = tempdev;
	}
	
	CString szAdapter;
	char tmp[12] = { 0 };
	iptos(((sockaddr_in *)m_dev->addresses->addr)->sin_addr.s_addr, tmp);

	szAdapter.Format(L"Name: %S\r\nDescription: %S\r\nAddress: %S",
		m_dev->name,
		m_dev->description,
		tmp);
	m_CtlEditAdapter.SetWindowText(szAdapter);

}
