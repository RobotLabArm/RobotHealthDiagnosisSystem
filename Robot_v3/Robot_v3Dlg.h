
// Robot_v3Dlg.h : header file
//
#ifndef __ROBOT_V3_H
#define __ROBOT_V3_H
#endif

#pragma once
#include "afxwin.h"
#include "OpenGLControl.h"  //  opengl header
#include <afxcmn.h>  //  開console header
// ChartCtrl
#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartTitle.h"
#include "ChartCtrl/ChartAxisLabel.h"
#include "ChartCtrl/ChartLineSerie.h"
#include "ChartCtrl/ChartAxis.h"
#include <time.h>

// C++ 寄信class library
#include "CMail.h"

// CRobot_v3Dlg dialog
class CRobot_v3Dlg : public CDialogEx
{
// Construction
public:
	CRobot_v3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ROBOT_V3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:

public:
	COpenGLControl m_OpenGLControl;
public:
	CStatic OpenGLSceneMainF;

public:
	void InitConsole(); //  開一個console 控制台
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CButton ProjectTest;
	afx_msg void OnBnClickedButton1();
	CButton Atom_Joint_Btn;
	afx_msg void OnBnClickedButton2();
	CButton check_atom;
	afx_msg void OnBnClickedCheckAtom();
	afx_msg void OnBnClickedNtu8dofbtn();
	CButton NTU8DOFbtn;
	afx_msg void OnBnClickedCheckAtom2();
	afx_msg void OnBnClickedCheckNtu8dof();
	CButton Check_NTU8DOF;
	CButton Atom_Cont_Btn;
	afx_msg void OnBnClickedAtomctl();
	afx_msg void OnBnClickedAtomtraj();
	CButton Atom_Traj_Btn;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	void OnOK(void);
	CButton Gripper_Btn;
	afx_msg void OnBnClickedGripper();
	CButton ForceSensor_Btn;
	afx_msg void OnBnClickedForcesensor();
	// ChartCtrl
	CChartCtrl m_ChartCtrl1;
	CChartLineSerie *pLineSerie1;
	CChartLineSerie *pLineSerie2;
	CChartLineSerie *pLineSerie3;
	//
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	double m_ChartArrayX[500];   
	double m_X[500]; 
	double m_UpperBound[500];  
	double m_Upper[500]; 
	double m_LowerBound[500]; 
	double m_Lower[500]; 
	unsigned int m_count;  
	const size_t m_c_arrayLength;  
	long m_nPointNum;
	void LeftMoveArray(double* ptr,size_t length,double data);
public:
	afx_msg void OnBnClickedRuning();
	afx_msg void OnBnClickedStop();
	CButton Diagnosis_Btn;
	afx_msg void OnBnClickedDiagnosis();
};
	double randf(double min,double max);