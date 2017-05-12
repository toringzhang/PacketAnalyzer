#ifndef _HEADERCTRLCL_H_INCLUDE
#define _HEADERCTRLCL_H_INCLUDE

#pragma once


// CHeaderCtrlCl

class CHeaderCtrlCl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CHeaderCtrlCl)

public:
	CHeaderCtrlCl();
	virtual ~CHeaderCtrlCl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	int m_R;
	int m_G;
	int m_B;
	int m_Gradient;	  // 画立体背景，渐变系数
	float m_Height;   //表头高度，这是倍数,
	int m_FontHeight; //字体高度
	int m_FontWith;   //字体宽度
	COLORREF     m_Color;
	CStringArray m_HChar;
	CString      m_Format; //表示对齐类型的整型数组,0表示左对齐，1表示中间对齐，2表示右对齐

	LRESULT OnLayout(WPARAM wParam, LPARAM lParam);
};

#endif // !_HEADERCTRLCL_H_INCLUDE