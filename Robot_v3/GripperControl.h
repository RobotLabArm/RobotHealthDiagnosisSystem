#pragma once
#include "afxwin.h"


// GripperControl 對話方塊

class GripperControl : public CDialogEx
{
	DECLARE_DYNAMIC(GripperControl)

public:
	GripperControl(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~GripperControl();

// 對話方塊資料
	enum { IDD = IDD_Gripper };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	DECLARE_MESSAGE_MAP()
public:
	CButton GripConnect_Btn;
	CButton GripDisconnect_Btn;
	CButton GripCommand_Btn;
	afx_msg void OnBnClickedGripconnect();
	afx_msg void OnBnClickedGripperdisconnect();
	afx_msg void OnBnClickedGripcommand();
};
