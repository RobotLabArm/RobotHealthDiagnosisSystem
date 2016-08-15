#ifndef __AtomControl_H_
#define __AtomControl_H_
#pragma once

#include "afxwin.h"
#include <Windows.h>
#
// AtomControl 對話方塊

class AtomControl : public CDialogEx
{
	DECLARE_DYNAMIC(AtomControl)

public:
	AtomControl(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~AtomControl();

// 對話方塊資料
	enum { IDD = IDD_AtomControl };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedTorquectl();
	afx_msg void OnBnClickedImpedance();
	afx_msg void OnBnClickedButton2();
};

#endif