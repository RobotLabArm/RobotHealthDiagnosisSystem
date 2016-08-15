// GripperControl.cpp : 實作檔
//

#include "stdafx.h"
#include "Robot_v3.h"
#include "GripperControl.h"
#include "afxdialogex.h"

///////////////////////////
#include "ProjectPlay.h"

bool FlagGripperControl = true;
// GripperControl 對話方塊

IMPLEMENT_DYNAMIC(GripperControl, CDialogEx)

GripperControl::GripperControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(GripperControl::IDD, pParent)
{

}

GripperControl::~GripperControl()
{
}

void GripperControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GripConnect, GripConnect_Btn);
	DDX_Control(pDX, IDC_Gripperdisconnect, GripDisconnect_Btn);
	DDX_Control(pDX, IDC_GripCommand, GripCommand_Btn);
}


BEGIN_MESSAGE_MAP(GripperControl, CDialogEx)
	ON_BN_CLICKED(IDC_GripConnect, &GripperControl::OnBnClickedGripconnect)
	ON_BN_CLICKED(IDC_Gripperdisconnect, &GripperControl::OnBnClickedGripperdisconnect)
	ON_BN_CLICKED(IDC_GripCommand, &GripperControl::OnBnClickedGripcommand)
END_MESSAGE_MAP()


// GripperControl 訊息處理常式

void GripperControl::OnBnClickedGripconnect()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	ProjectPlay mProjectplay;
	mProjectplay.GripperConnectOrDisConnect(1);
	GripCommand_Btn.EnableWindow(true);
}


void GripperControl::OnBnClickedGripperdisconnect()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	ProjectPlay mProjectplay;
	mProjectplay.GripperConnectOrDisConnect(0);
	GripCommand_Btn.EnableWindow(false);
}


void GripperControl::OnBnClickedGripcommand()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	ProjectPlay mProjectplay;

	//close
	if (FlagGripperControl == true)
	{
	mProjectplay.Grip_Command(1);
	GripCommand_Btn.SetWindowTextA("Gripper Open");
	FlagGripperControl = false;
	}
	//open
	else
	{
	mProjectplay.Grip_Command(0);
	GripCommand_Btn.SetWindowTextA("Gripper Close");
	FlagGripperControl = true;
	}

	//UpdateData(TRUE);
}
