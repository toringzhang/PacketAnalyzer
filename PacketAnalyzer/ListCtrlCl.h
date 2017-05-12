#ifndef _LISTCTRLCL_H_INCLUDE
#define _LISTCTRLCL_H_INCLUDE

#pragma once
#include "HeaderCtrlCl.h"

// CListCtrlCl

class CListCtrlCl : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlCl)

public:
	CListCtrlCl();
	virtual ~CListCtrlCl();


	CHeaderCtrlCl m_Header;

	// 行高
	int m_nRowHeight;
	// 字体高度
	int m_FontHeight;
	// 字体宽度
	int m_FontWith;
	COLORREF m_Color;


protected:
	DECLARE_MESSAGE_MAP()

	virtual void PreSubclassWindow();

public:
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	
	CPtrList m_ptrListCol;    //保存列颜色
	CPtrList m_ptrListItem;   //保存Item颜色表
	CPtrList m_colTextColor;  //保存列字体颜色
	CPtrList m_ItemTextColor; //保存单元格字体颜色
	// 设置表头高度
	void SetHeaderHeight(float Height);
	void SetHeaderFontHW(int nHeight, int nWith); //设置表头字体大小
	void SetHeaderTextColor(COLORREF color);
	// Gradient - 渐变系数，立体背景用,不用渐变设为0
	void SetHeaderBKColor(int R, int G, int B, int Gradient);

};


#endif