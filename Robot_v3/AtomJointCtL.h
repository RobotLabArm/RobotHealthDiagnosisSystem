#ifndef __AtomJointCtL_H
#define __AtomJointCtL_H 
#endif

#pragma once
#include "afxwin.h"


// AtomJointCtL 對話方塊

class AtomJointCtL : public CDialogEx
{
	DECLARE_DYNAMIC(AtomJointCtL)

public:
	AtomJointCtL(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~AtomJointCtL();

// 對話方塊資料
	enum { IDD = IDD_AtomJointCtl };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
public:
	void AtomControlInit();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedJ1n();
	afx_msg void OnBnClickedJ1n2();
	afx_msg void OnBnClickedJ1n3();
	afx_msg void OnBnClickedJ1n4();
	afx_msg void OnBnClickedJ1n5();
	afx_msg void OnBnClickedJ1n6();
	afx_msg void OnBnClickedJ1n7();
	afx_msg void OnBnClickedJ1n8();
	afx_msg void OnBnClickedJ1n9();
	afx_msg void OnBnClickedJ1n10();
	afx_msg void OnBnClickedJ1n11();
	afx_msg void OnBnClickedJ1n12();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	CEdit CEdit_theta1;
	CEdit CEdit_theta2;
	CEdit CEdit_theta3;
	CEdit CEdit_theta4;
	CEdit CEdit_theta5;
	CEdit CEdit_theta6;
	afx_msg void OnBnClickedxp();
	CEdit CEdit_x;
	CEdit CEdit_y;
	CEdit CEdit_z;
	CEdit CEdit_rx;
	CEdit CEdit_ry;
	CEdit CEdit_rz;
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnBnClickedxn();
	afx_msg void OnBnClickedyp();
	afx_msg void OnBnClickedyn();
	afx_msg void OnBnClickedzp();
	afx_msg void OnBnClickedzn();
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnBnClickedrxp();
	afx_msg void OnBnClickedrxn();
	afx_msg void OnBnClickedryp();
	afx_msg void OnBnClickedryn();
	afx_msg void OnBnClickedrzp();
	afx_msg void OnBnClickedrzn();
	afx_msg void OnBnClickedArrfw();
	CButton ControllerConnect;
	CButton ControllerDisconnect;
	afx_msg void OnBnClickedAtomconnect();
	afx_msg void OnBnClickedAtomdisconnect();
	afx_msg void OnBnClickedAtomsave();
	afx_msg void OnBnClickedAtomload();
	afx_msg void OnBnClickedAtomreset();
	CButton AtomRestBtn;
	afx_msg void OnBnClickedAtomsimulate();
	CButton check_atomsimulate;

public:
	static float Atom_thetaALL[6];
	static float Atom_CarAll[6];
};
