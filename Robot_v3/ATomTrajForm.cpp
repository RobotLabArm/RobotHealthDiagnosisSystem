// ATomTrajForm.cpp : 實作檔
//

#include "stdafx.h"
#include "Robot_v3.h"
#include "ATomTrajForm.h"
#include "afxdialogex.h"
#include <string>
/////////////////////////
#include "ProjectPlay.h"
#include "AtomJointCtL.h"
#include "OpenGLControl.h"
// ATomTrajForm 對話方塊
using namespace std;
int setpointcount;
IMPLEMENT_DYNAMIC(ATomTrajForm, CDialogEx)

ATomTrajForm::ATomTrajForm(CWnd* pParent /*=NULL*/)
	: CDialogEx(ATomTrajForm::IDD, pParent)
{
}

ATomTrajForm::~ATomTrajForm()
{
}

void ATomTrajForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, CListBox_AtomSetPoint);
	DDX_Control(pDX, IDC_TrajSelect, IDC_AtomTrajSelec);
	DDX_Control(pDX, IDC_EditTime, CEdit_AtomSetTime);
	DDX_Control(pDX, IDC_ClearListBox, mClearListBoxBtn);
}


BEGIN_MESSAGE_MAP(ATomTrajForm, CDialogEx)
	ON_BN_CLICKED(IDC_AtomTrajGO, &ATomTrajForm::OnBnClickedAtomtrajgo)
	ON_BN_CLICKED(IDC_AtomSet, &ATomTrajForm::OnBnClickedAtomset)
	ON_LBN_SELCHANGE(IDC_LIST2, &ATomTrajForm::OnLbnSelchangeList2)
	ON_CBN_SELCHANGE(IDC_TrajSelect, &ATomTrajForm::OnCbnSelchangeTrajselect)
	ON_EN_CHANGE(IDC_EditTime, &ATomTrajForm::OnEnChangeEdittime)
	ON_CBN_SELENDOK(IDC_TrajSelect, &ATomTrajForm::OnCbnSelendokTrajselect)
	ON_BN_CLICKED(IDC_ClearListBox, &ATomTrajForm::OnBnClickedClearlistbox)
END_MESSAGE_MAP()


// ATomTrajForm 訊息處理常式
BOOL ATomTrajForm::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	AtomJointCtL mAtomJointCtL;
	setpointcount = 1;
	//Initial ListBox
	CString setPointstring;
	setPointstring.Format("%d.P(x,y,z,rx,ry,rz)=(%.3f,%.3f,%.3f,%.1f,%.1f,%.1f)",
		setpointcount,
		mAtomJointCtL.Atom_CarAll[0],
		mAtomJointCtL.Atom_CarAll[1],
		mAtomJointCtL.Atom_CarAll[2],
		mAtomJointCtL.Atom_CarAll[3],
		mAtomJointCtL.Atom_CarAll[4],
		mAtomJointCtL.Atom_CarAll[5]);

	CListBox_AtomSetPoint.AddString(setPointstring);
	// Initial time listbox
	SetDlgItemText(IDC_EditTime,"3");
	// Initial 下拉式選單
	IDC_AtomTrajSelec.ResetContent();
	IDC_AtomTrajSelec.InsertString(0,"1. 212");
	IDC_AtomTrajSelec.InsertString(1,"2. 434");
	IDC_AtomTrajSelec.InsertString(2,"3. 535");
	IDC_AtomTrajSelec.InsertString(3,"4. Cubic");
	IDC_AtomTrajSelec.InsertString(4,"5. Line");
	IDC_AtomTrajSelec.InsertString(5,"6. Simple");
	IDC_AtomTrajSelec.SetCurSel(5);
	return true;
}
void ATomTrajForm::OnBnClickedAtomtrajgo()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	ProjectPlay mProject;
	mProject.Atom_TrajectoryGO();
}
void ATomTrajForm::OnBnClickedAtomset()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	//ProjectPlay mProject;
	//CEdit *timeEdit;
	//timeEdit = (CEdit*)GetDlgItem(IDC_EditTime);  // get time CEDIT value
	//CString str;
	//float times;
	//timeEdit->GetWindowTextA(str);   // CEdit to string
	//times = atof(str);  //  string to float
	//setpointcount++;
	////////////////////////////////////////////////////////
	//CString setPointstring,TrajType;
	//int index_ = IDC_AtomTrajSelec.GetCurSel();
	//IDC_AtomTrajSelec.GetLBText(index_,TrajType);
	//setPointstring.Format("%d.Type:%s,Ts:%.1f P=(%.3f,%.3f,%.3f,%.1f,%.1f,%.1f)",
	//	setpointcount,
	//	TrajType,
	//	times,
	//	AtomJointCtL::Atom_CarAll[0],
	//	AtomJointCtL::Atom_CarAll[1],
	//	AtomJointCtL::Atom_CarAll[2],
	//	AtomJointCtL::Atom_CarAll[3],
	//	AtomJointCtL::Atom_CarAll[4],
	//	AtomJointCtL::Atom_CarAll[5]);

	//CListBox_AtomSetPoint.AddString(setPointstring);
	//mProject.Atom_Setpoint(AtomJointCtL::Atom_thetaALL,times,index_);

	////////////////////////////////////////////////////
	//COpenGLControl mOpenGLControl;
	//mOpenGLControl.Draw_sphere(0.05,mAtomJointCtL.Atom_CarAll[0],mAtomJointCtL.Atom_CarAll[1],mAtomJointCtL.Atom_CarAll[2]);

	ProjectPlay mProject;
	float Atom_theta[6]={0.0};

	for (int i=0; i < 6; i++)
	{
		if (i == 0 || i == 2 || i == 4)
		{
			Atom_theta[i] = 15 *M_PI/180;
			mProject.Atom_Setpoint(Atom_theta, 0.5, 0);

			Atom_theta[i] = -15 *M_PI/180;
			mProject.Atom_Setpoint(Atom_theta, 1.0, 0);

			Atom_theta[i] = 15 *M_PI/180;
			mProject.Atom_Setpoint(Atom_theta, 1.0, 0);

			Atom_theta[i] = -15 *M_PI/180;
			mProject.Atom_Setpoint(Atom_theta, 1.0, 0);

			Atom_theta[i] = 0.0 *M_PI/180;
			mProject.Atom_Setpoint(Atom_theta, 0.5, 0);
		}
		else
		{
			Atom_theta[i] = 10 *M_PI/180;
			mProject.Atom_Setpoint(Atom_theta, 0.5, 0);

			Atom_theta[i] = -10 *M_PI/180;
			mProject.Atom_Setpoint(Atom_theta, 1.0, 0);

			Atom_theta[i] = 10 *M_PI/180;
			mProject.Atom_Setpoint(Atom_theta, 1.0, 0);

			Atom_theta[i] = -10 *M_PI/180;
			mProject.Atom_Setpoint(Atom_theta, 1.0, 0);

			Atom_theta[i] = 0.0 *M_PI/180;
			mProject.Atom_Setpoint(Atom_theta, 0.5, 0);
		}
	}

	//CString setPointstring;

	//// x = 0.4918 y= 0 z = 1.463
	//Atom_theta[0] = 5.38e-010;
	//Atom_theta[1] = -0.223;
	//Atom_theta[2] = -7.56e-011;
	//Atom_theta[3] = -0.242;
	//Atom_theta[4] = -4.81e-011;
	//Atom_theta[5] = -0.465;
	//mProject.Atom_Setpoint(Atom_theta, 2.0, 0);
	//setPointstring.Format("2.Type:1. 212,Ts:2.0 P=(%.3f,%.3f,%.3f,%.1f,%.1f,%.1f)",
	//	0.4918,
	//	-0.0,
	//	1.463,
	//	-0.0,
	//	-0.0,
	//	-0.0);
	//CListBox_AtomSetPoint.AddString(setPointstring);

	//// x = 0.4916 y= 0 z = 1.360
	//Atom_theta[0] = 5.51e-010;
	//Atom_theta[1] = -0.389;
	//Atom_theta[2] = -9.78e-011;
	//Atom_theta[3] = -0.44;
	//Atom_theta[4] = -3.36e-011;
	//Atom_theta[5] = -0.828;
	//mProject.Atom_Setpoint(Atom_theta, 1.0, 4);
	//setPointstring.Format("3.Type:5. Line,Ts:1.0 P=(%.3f,%.3f,%.3f,%.1f,%.1f,%.1f)",
	//	0.4916,
	//	-0.0,
	//	1.360,
	//	-0.0,
	//	-0.0,
	//	-0.0);
	//CListBox_AtomSetPoint.AddString(setPointstring);

	//// x = 0.4915 y= 0 z = 1.4631
	//Atom_theta[0] = 5.38e-010;
	//Atom_theta[1] = -0.222;
	//Atom_theta[2] = -7.63e-011;
	//Atom_theta[3] = -0.242;
	//Atom_theta[4] = -4.82e-011;
	//Atom_theta[5] = -0.465;
	//mProject.Atom_Setpoint(Atom_theta, 1.0, 4);
	//setPointstring.Format("4.Type:5. Line,Ts:1.0 P=(%.3f,%.3f,%.3f,%.1f,%.1f,%.1f)",
	//	0.4915,
	//	-0.0,
	//	1.4631,
	//	-0.0,
	//	-0.0,
	//	-0.0);
	//CListBox_AtomSetPoint.AddString(setPointstring);

	//// x = 0.5724 y= 0 z = 1.4630
	//Atom_theta[0] = 2.51e-010;
	//Atom_theta[1] = -0.426;
	//Atom_theta[2] = 1.97e-010;
	//Atom_theta[3] = 0.0599;
	//Atom_theta[4] = -1.51e-010;
	//Atom_theta[5] = -0.366;
	//mProject.Atom_Setpoint(Atom_theta, 1.0, 0);
	//setPointstring.Format("5.Type:1. 212,Ts:1.0 P=(%.3f,%.3f,%.3f,%.1f,%.1f,%.1f)",
	//	0.5724,
	//	-0.0,
	//	1.4630,
	//	-0.0,
	//	-0.0,
	//	-0.0);
	//CListBox_AtomSetPoint.AddString(setPointstring);

	//// x = 0.5722 y= 0 z = 1.3600
	//Atom_theta[0] = 3.27e-010;
	//Atom_theta[1] = -0.556;
	//Atom_theta[2] = 8.5e-011;
	//Atom_theta[3] = -0.131;
	//Atom_theta[4] = -1.35e-010;
	//Atom_theta[5] = -0.687;
	//mProject.Atom_Setpoint(Atom_theta, 1.0, 4);
	//setPointstring.Format("6.Type:5. Line,Ts:1.0 P=(%.3f,%.3f,%.3f,%.1f,%.1f,%.1f)",
	//	0.5722,
	//	-0.0,
	//	1.3600,
	//	-0.0,
	//	-0.0,
	//	-0.0);
	//CListBox_AtomSetPoint.AddString(setPointstring);

	//// x = 0.5721 y= 0 z = 1.4631
	//Atom_theta[0] = 2.51e-010;
	//Atom_theta[1] = -0.425;
	//Atom_theta[2] = 1.97e-010;
	//Atom_theta[3] = 0.0589;
	//Atom_theta[4] = -1.51e-010;
	//Atom_theta[5] = -0.366;
	//mProject.Atom_Setpoint(Atom_theta, 1.0, 4);
	//setPointstring.Format("7.Type:5. Line,Ts:1.0 P=(%.3f,%.3f,%.3f,%.1f,%.1f,%.1f)",
	//	0.5721,
	//	-0.0,
	//	1.4631,
	//	-0.0,
	//	-0.0,
	//	-0.0);
	//CListBox_AtomSetPoint.AddString(setPointstring);

	//// x = 0.4351 y= 0 z = 1.6010
	//Atom_theta[0] = 0.0;
	//Atom_theta[1] = 0.0;
	//Atom_theta[2] = 0.0;
	//Atom_theta[3] = 0.0;
	//Atom_theta[4] = 0.0;
	//Atom_theta[5] = 0.0;
	//mProject.Atom_Setpoint(Atom_theta, 2.0, 0);
	//setPointstring.Format("8.Type:1. 212,Ts:2.0 P=(%.3f,%.3f,%.3f,%.1f,%.1f,%.1f)",
	//	0.4351,
	//	-0.0,
	//	1.6010,
	//	-0.0,
	//	-0.0,
	//	-0.0);
	//CListBox_AtomSetPoint.AddString(setPointstring);
	cout << "Set finished" <<endl;
}
void ATomTrajForm::OnLbnSelchangeList2()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	UpdateWindow();
}
void ATomTrajForm::OnCbnSelchangeTrajselect()
{
	// TODO: 在此加入控制項告知處理常式程式碼
}
void ATomTrajForm::OnEnChangeEdittime()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
}

void ATomTrajForm::OnCbnSelchangeCombo3()
{
	// TODO: 在此加入控制項告知處理常式程式碼
}


void ATomTrajForm::OnCbnSelendokTrajselect()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString str;
	int index = IDC_AtomTrajSelec.GetCurSel();
}


void ATomTrajForm::OnBnClickedClearlistbox()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	ProjectPlay mProjectPlay;
	mProjectPlay.Atom_TrajClear();
	CListBox_AtomSetPoint.ResetContent();
	ASSERT(CListBox_AtomSetPoint.GetCount()==0);
	OnInitDialog();
}
