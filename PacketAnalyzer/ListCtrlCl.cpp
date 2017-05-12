// ListCtrlCl.cpp : 实现文件
//

#include "stdafx.h"
#include "PacketAnalyzer.h"
#include "ListCtrlCl.h"


// CListCtrlCl

struct stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
};



IMPLEMENT_DYNAMIC(CListCtrlCl, CListCtrl)

CListCtrlCl::CListCtrlCl() : m_nRowHeight(0), m_FontHeight(12), m_FontWith(0)
{
	m_Color = RGB(0, 0, 0);//黑色
}

CListCtrlCl::~CListCtrlCl()
{
}


BEGIN_MESSAGE_MAP(CListCtrlCl, CListCtrl)
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()



// CListCtrlCl 消息处理程序


void CListCtrlCl::PreSubclassWindow()
{
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
}

void CListCtrlCl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nRowHeight>0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


int CListCtrlCl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */)
{
	m_Header.m_HChar.Add(lpszColumnHeading);
	switch (nFormat)
	{
	case LVCFMT_LEFT: m_Header.m_Format = m_Header.m_Format + L"0"; break;
	case LVCFMT_CENTER:	m_Header.m_Format = m_Header.m_Format + L"1"; break;
	case LVCFMT_RIGHT: m_Header.m_Format = m_Header.m_Format + L"2";
	default: m_Header.m_Format = m_Header.m_Format + L"1"; break;
	}
	return CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}

// Gradient - 渐变系数，立体背景用,不用渐变设为0
void CListCtrlCl::SetHeaderBKColor(int R, int G, int B, int Gradient) //设置表头背景色
{
	m_Header.m_R = R;
	m_Header.m_G = G;
	m_Header.m_B = B;
	m_Header.m_Gradient = Gradient;
}

// 设置表头高度
void CListCtrlCl::SetHeaderHeight(float Height) //设置表头高度
{
	m_Header.m_Height = Height;
}

void CListCtrlCl::SetHeaderFontHW(int nHeight, int nWith) //设置头部字体宽和高
{
	m_Header.m_FontHeight = nHeight;
	m_Header.m_FontWith = nWith;
}
void CListCtrlCl::SetHeaderTextColor(COLORREF color) //设置头部字体颜色
{
	m_Header.m_Color = color;
}