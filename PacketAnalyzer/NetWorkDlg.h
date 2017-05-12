#ifndef _NETWORKDLG_H_INCLUDE
#define _NETWORKDLG_H_INCLUDE
#pragma once


#include "afxwin.h"
#include "PacketType.h"
#include "Include/pcap.h"
#include "PacketAnalyzerDlg.h"
// CNetWorkDlg 对话框

class CNetWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNetWorkDlg)

public:
	CNetWorkDlg(CPacketAnalyzerDlg* pParent = NULL);   // 标准构造函数
	virtual ~CNetWorkDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NETWORK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	


public:
	CPacketAnalyzerDlg* m_ParentDlg;

	CEdit m_CtlEditAdapter;
	CComboBox m_Combo;

	pcap_if_t* m_dev; //当前适配器
	pcap_if_t* m_alldevs;//所有适配器信息

	CBitmap    m_BkBmp;
	char m_errbuf[PCAP_ERRBUF_SIZE];

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonCancle();
	afx_msg void OnCbnSelchangeComboSelect();
};


#endif // !_NETWORKDLG_H_INCLUDE


