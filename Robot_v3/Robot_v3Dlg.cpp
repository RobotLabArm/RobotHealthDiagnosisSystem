
// Robot_v3Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "Robot_v3.h"
#include "Robot_v3Dlg.h"
#include "global_value.h"
#include "ProjectPlay.h"
//3RD party Control panel
#include "AtomJointCtL.h"
#include "AtomControl.h"
#include "ATomTrajForm.h"
#include "GripperControl.h"
// matlab eg
#include <engine.h>
#include <memory.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
bool GLDrawAtomStickFlag = false;  //  draw the Robot Arm stick
bool ForceSensorClose = true;
HANDLE gSendingThread; // arm control thread handle
DWORD gTIDSending;//  arm control thread ID
bool gSendingLife = true;//  arm control 是否工作旗標
bool gSendingThreadOpened = false; // 指示手臂部控制thread是否打開
void commandThreadControl( bool shallWe);
float gTimer = 5.0; // 5ms



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CRobot_v3Dlg dialog


CRobot_v3Dlg::CRobot_v3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRobot_v3Dlg::IDD, pParent)
	, m_nPointNum(16384)
	, m_c_arrayLength(500)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRobot_v3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OpenGLScene, OpenGLSceneMainF);
	DDX_Control(pDX, IDC_BUTTON1, ProjectTest);
	DDX_Control(pDX, IDC_BUTTON2, Atom_Joint_Btn);
	DDX_Control(pDX, IDC_CHECK_Atom, check_atom);
	DDX_Control(pDX, IDC_NTU8DOFBtn, NTU8DOFbtn);
	DDX_Control(pDX, IDC_CHECK_NTU8DOF, Check_NTU8DOF);
	DDX_Control(pDX, IDC_AtomCtL, Atom_Cont_Btn);
	DDX_Control(pDX, IDC_AtomTraj, Atom_Traj_Btn);
	DDX_Control(pDX, IDC_Gripper, Gripper_Btn);
	DDX_Control(pDX, IDC_ForceSensor, ForceSensor_Btn);
	DDX_Control(pDX, IDC_ChartCtrl1, m_ChartCtrl1);  
	DDX_Control(pDX, IDC_Diagnosis, Diagnosis_Btn);
}

BEGIN_MESSAGE_MAP(CRobot_v3Dlg, CDialogEx)
//	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON1, &CRobot_v3Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRobot_v3Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK_Atom, &CRobot_v3Dlg::OnBnClickedCheckAtom)
	ON_BN_CLICKED(IDC_NTU8DOFBtn, &CRobot_v3Dlg::OnBnClickedNtu8dofbtn)
	ON_BN_CLICKED(IDC_CHECK_NTU8DOF, &CRobot_v3Dlg::OnBnClickedCheckNtu8dof)
	ON_BN_CLICKED(IDC_AtomCtL, &CRobot_v3Dlg::OnBnClickedAtomctl)
	ON_BN_CLICKED(IDC_AtomTraj, &CRobot_v3Dlg::OnBnClickedAtomtraj)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_Gripper, &CRobot_v3Dlg::OnBnClickedGripper)
	ON_BN_CLICKED(IDC_ForceSensor, &CRobot_v3Dlg::OnBnClickedForcesensor)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Runing, &CRobot_v3Dlg::OnBnClickedRuning)
	ON_BN_CLICKED(IDC_Stop, &CRobot_v3Dlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_Diagnosis, &CRobot_v3Dlg::OnBnClickedDiagnosis)
END_MESSAGE_MAP()


// CRobot_v3Dlg message handlers

BOOL CRobot_v3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.	

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitConsole();  //  open the console

	//*********Opengl Control*************
	CRect rect;

	OpenGLSceneMainF.GetWindowRect(rect);
	ScreenToClient(rect);

	m_OpenGLControl.openGLCreate(rect,this);
	m_OpenGLControl.Dwidth = rect.right-rect.left;
	m_OpenGLControl.Dhigh = rect.bottom-rect.top;

	// Setup the OpenGL Window's timer to render
	m_OpenGLControl.m_unpTimer = m_OpenGLControl.SetTimer(1,1,0);

	//*********Opengl Control end*************

	//*********ChartCtrl1*************
	//數據軸
	CChartAxis *pAxis= NULL;   
	pAxis = m_ChartCtrl1.CreateStandardAxis(CChartCtrl::BottomAxis);  
	pAxis->SetAutomatic(true);  
	pAxis = m_ChartCtrl1.CreateStandardAxis(CChartCtrl::LeftAxis);  
	pAxis->SetAutomatic(true);  

	//標題
	TChartString str1;
	str1 = _T("Monitoring");  
	m_ChartCtrl1.GetTitle()->AddString(str1); 

	//坐標軸名稱
	TChartString str2 = _T("Std");  
	m_ChartCtrl1.GetLeftAxis()->GetLabel()->SetText(str2);    
	str2 = _T("Time"); 
	m_ChartCtrl1.GetBottomAxis()->GetLabel()->SetText(str2);

	// 畫圖
	pLineSerie1 = m_ChartCtrl1.CreateLineSerie();  
	pLineSerie1->SetSeriesOrdering(poNoOrdering);
	pLineSerie2 = m_ChartCtrl1.CreateLineSerie();  
	pLineSerie2->SetSeriesOrdering(poNoOrdering);
	pLineSerie3 = m_ChartCtrl1.CreateLineSerie();  
	pLineSerie3->SetSeriesOrdering(poNoOrdering);

	//*********OChartCtrl1*************

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CRobot_v3Dlg::OnOK(void)
{
	CWnd* pWnd = GetFocus();
	if(GetDlgItem(IDOK) == pWnd)
	{
		CDialog::OnOK();
		return;
	}
	// Enter key was hit -> do whatever you want
}
//void CRobot_v3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
//{
//	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
//	{
//		CAboutDlg dlgAbout;
//		dlgAbout.DoModal();
//	}
//	else
//	{
//		CDialogEx::OnSysCommand(nID, lParam);
//	}
//}
void CRobot_v3Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	switch (nType)
	{
	case SIZE_RESTORED:
		if (this->m_OpenGLControl.m_bIsMaximized)
		{
			this->m_OpenGLControl.OnSize(nType,cx,cy);
			this->m_OpenGLControl.m_bIsMaximized = false;
		}
		break;
	case SIZE_MAXIMIZED:
		this->m_OpenGLControl.OnSize(nType,cx,cy);
		this->m_OpenGLControl.m_bIsMaximized = true;
		break;

	}
}
BOOL CRobot_v3Dlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	this->m_OpenGLControl.OnMouseWheel(nFlags, zDelta,pt);
	return CDialog::OnMouseWheel(nFlags,zDelta,pt);
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRobot_v3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRobot_v3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRobot_v3Dlg::InitConsole()  
{  
	int nRet= 0;  
	FILE* fp;  
	AllocConsole();  
	nRet= _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);  
	fp = _fdopen(nRet, "w");  
	*stdout = *fp;  
	setvbuf(stdout, NULL, _IONBF, 0);  
} 

void CRobot_v3Dlg::OnBnClickedButton1()
{
	ProjectPlay mProjectplay;   //  sub-Project C++ can write anything... 
	mProjectplay.AtomDynControl();
	//m_OpenGLControl.a = true;
}


void CRobot_v3Dlg::OnBnClickedButton2()
{
//	AtomJointCtL m_AtomJointCtL;
//	m_AtomJointCtL.DoModal();
	AtomJointCtL *pDlg = new AtomJointCtL();
	pDlg->Create(AtomJointCtl,this);
	pDlg->ShowWindow(SW_NORMAL);
}


void CRobot_v3Dlg::OnBnClickedCheckAtom()
{
	//UINT nCheck = check_atom.GetState();
	ProjectPlay mProjectplay;   //  sub-Project C++ can write anything... 
	int nCheck = check_atom.GetCheck();
	if (nCheck)
	{
		//Atom Initial
		mProjectplay.project_InitAtom();
		Atom_Joint_Btn.EnableWindow(true);
		Atom_Cont_Btn.EnableWindow(true);
		Atom_Traj_Btn.EnableWindow(true);
		Gripper_Btn.EnableWindow(true);
	}
	else
	{
		mProjectplay.project_ATomThreadClose();
		Atom_Joint_Btn.EnableWindow(false);
		Atom_Cont_Btn.EnableWindow(false);
		Atom_Traj_Btn.EnableWindow(false);
		Gripper_Btn.EnableWindow(false);
	}
	UpdateData(true);
}


void CRobot_v3Dlg::OnBnClickedNtu8dofbtn()
{
	// TODO: 在此加入控制項告知處理常式程式碼
}




void CRobot_v3Dlg::OnBnClickedCheckNtu8dof()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	UpdateData(true);
	ProjectPlay mProjectplay;   //  sub-Project C++ can write anything... 
	if (Check_NTU8DOF)
	{
		//Atom Initial
		mProjectplay.ProjectPlay_InitNTU8DOF();
		NTU8DOFbtn.EnableWindow(true);
	}
	else
	{
		NTU8DOFbtn.EnableWindow(FALSE);
	}

}


void CRobot_v3Dlg::OnBnClickedAtomctl()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	//AtomControl mAtomControl;
	//mAtomControl.DoModal();
	AtomControl *pDlg = new AtomControl();
	pDlg->Create(IDD_AtomControl,this);
	pDlg->ShowWindow(SW_NORMAL);

	

}


void CRobot_v3Dlg::OnBnClickedAtomtraj()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	ATomTrajForm *pDlg = new ATomTrajForm();
	pDlg->Create(IDD_AtomTrajForm,this);
	pDlg->ShowWindow(SW_NORMAL);
}


void CRobot_v3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CDialogEx::OnSysCommand(nID, lParam);
}


void CRobot_v3Dlg::OnBnClickedGripper()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	GripperControl *pDlg = new GripperControl();
	pDlg->Create(IDD_Gripper,this);
	pDlg->ShowWindow(SW_NORMAL);
}


void CRobot_v3Dlg::OnBnClickedForcesensor()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	ProjectPlay mProjectplay;

	//open
	if (ForceSensorClose == true)
	{
	mProjectplay.project_InitForceSensor();
	ForceSensor_Btn.SetWindowTextA("ForceSensor Close");
	ForceSensorClose = false;
	}
	//close
	else
	{
	mProjectplay.project_ForceSensorThreadClose();
	ForceSensor_Btn.SetWindowTextA("ForceSensor Open");
	ForceSensorClose = true;
	}

	//for (int i = 0; i < 6;i++) 
	//cout << AtomJointCtL::Atom_thetaALL[i] <<endl;


	//UpdateData(TRUE);
}


void CRobot_v3Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	if(0 == nIDEvent)
	{	
		++m_count;
		pLineSerie1->ClearSerie();
		LeftMoveArray(m_ChartArrayX,m_c_arrayLength, ProjectPlay::Monitoring_std);
		LeftMoveArray(m_X,m_c_arrayLength,m_count);
		pLineSerie1->AddPoints(m_X,m_ChartArrayX,m_c_arrayLength);

		pLineSerie2->ClearSerie();
		LeftMoveArray(m_UpperBound,m_c_arrayLength, 3.0);
		LeftMoveArray(m_Upper,m_c_arrayLength,m_count);
		pLineSerie2->AddPoints(m_Upper,m_UpperBound,m_c_arrayLength);

		pLineSerie3->ClearSerie();
		LeftMoveArray(m_LowerBound,m_c_arrayLength, -3.0);
		LeftMoveArray(m_Lower,m_c_arrayLength,m_count);
		pLineSerie3->AddPoints(m_Lower,m_LowerBound,m_c_arrayLength);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CRobot_v3Dlg::LeftMoveArray(double* ptr,size_t length,double data)  
{  
    for (size_t i=1;i<length;++i)  
    {  
        ptr[i-1] = ptr[i];  
    }  
    ptr[length-1] = data;
}

void CRobot_v3Dlg::OnBnClickedRuning()
{	
	/*
		作者: 曾建興
		日期: 2016/6/20
		功能: 檢測系統 畫折線圖工具
		有使用: ChartCtrl
	*/
	// TODO: 在此加入控制項告知處理常式程式碼
    KillTimer(0);  
    ZeroMemory(&m_ChartArrayX,sizeof(double)*m_c_arrayLength);  
    ZeroMemory(&m_UpperBound,sizeof(double)*m_c_arrayLength);  
    ZeroMemory(&m_LowerBound,sizeof(double)*m_c_arrayLength);  
    for (size_t i=0;i<m_c_arrayLength;++i)  
    {  
        m_X[i] = i;  
        m_Upper[i] = i;  
        m_Lower[i] = i;  
    }  
    m_count = m_c_arrayLength;
    pLineSerie1->ClearSerie();
    pLineSerie2->ClearSerie();
    pLineSerie3->ClearSerie();

    SetTimer(0,0,NULL);
}


void CRobot_v3Dlg::OnBnClickedStop()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	KillTimer(0);
}

double randf(double min,double max)
{
	int minInteger = (int)(min*10000);
	int maxInteger = (int)(max*10000);
	int randInteger = rand()*rand();
	int diffInteger = maxInteger - minInteger;
	int resultInteger = randInteger % diffInteger + minInteger;
	return resultInteger/10000.0;
}

void CRobot_v3Dlg::OnBnClickedDiagnosis()
{
	/*
		作者: 曾建興
		日期: 2016/6/20
		功能: 診斷系統與寄信功能
		有使用: engine.h, memory.h, CMail.h
	*/
	#pragma region 呼叫 Matlab SVM RHI
	printf("Diagnosing...\n");
	Engine *ep;
	if (!(ep = engOpen("\0")))
	{
		printf("Can't start MATLAB engine\n");
	}
	engEvalString(ep, "run(\'D:\\newRobotAtom\\matlab\\RHDS\\onlineSVM.m\')");

	mxArray *OutLable = engGetVariable(ep, "OutLable");
	mxArray *RHI = engGetVariable(ep, "RHI");
	bool IsNormal = true;

	printf("Robot Health Index is %f.\n", mxGetPr(RHI)[0]);
	printf("The results are as follows:\n");

	for (int i = 0;i < 6;i++)
	{
		if (mxGetPr(OutLable)[i] == 1)
			printf("Joint%d is normal.\n",i+1);
		else
		{
			if (mxGetPr(OutLable)[i] == 2)
				printf("Joint%d is 'Gear wear'. Reaction and repair: Replace\n",i+1);
			if (mxGetPr(OutLable)[i] == 3)
				printf("Joint%d is 'Gear lacking of lubrication'. Reaction and repair: Gear oil supplement\n",i+1);
			if (mxGetPr(OutLable)[i] == 4)
				printf("Joint%d is 'Timing belt looseness'. Reaction and repair: Tighten or replace\n",i+1);
			if (mxGetPr(OutLable)[i] == 5)
				printf("Joint%d is 'Screw looseness'. Reaction and repair: Tighten the screws\n",i+1);

			IsNormal = false;
		}
	}

	if(IsNormal)
	{
		mxDestroyArray(OutLable);
		mxDestroyArray(RHI);
		engClose(ep);
		return;
	}

	#pragma endregion


	#pragma region C++寄信
	
	printf("正在發信中...\n");

	bool bError = false;

	try
	{
		CMail mail;

		//mail.Set_SMTPServer("smtp.gmail.com", 587);
		//mail.Set_Username("abcd1234");
		//mail.Set_Password("abcd1234");
		//mail.Set_FromAddress("ken82318@gmail.com");
  		//mail.Set_FromName("鄒振宇");
  		//mail.Set_ToAddress("ken82318@gmail.com");
  		//mail.Set_Mail_Subject("測試信件");
		//mail.Set_Mail_Attachment("C:\\Users\\Ken\\Desktop\\雷尼爾山上方的銀河 (美國華盛頓州).jpg");
		mail.Load_Login("C++_Send_Mail\\Login.ini");

		mail.Set_Mail_Data("以下為故障診斷結果\n");
		mail.Set_Mail_Data("電腦發送，請勿回復\n");
		
		char Data[1024] = {'\0'};
		
		for (int i = 0;i < 6;i++)
		{
			if (mxGetPr(OutLable)[i] == 1)
				sprintf(Data, "Joint%d is normal.\n", i+1);
			if (mxGetPr(OutLable)[i] == 2)
				sprintf(Data, "Joint%d is ''Gear wear''. Reaction and repair: Replace\n", i+1);
			if (mxGetPr(OutLable)[i] == 3)
				sprintf(Data, "Joint%d is ''Gear lacking of lubrication''. Reaction and repair: Gear oil supplement\n", i+1);
			if (mxGetPr(OutLable)[i] == 4)
				sprintf(Data, "Joint%d is ''Timing belt looseness''. Reaction and repair: Tighten or replace\n", i+1);
			if (mxGetPr(OutLable)[i] == 5)
				sprintf(Data, "Joint%d is ''Screw looseness''. Reaction and repair: Tighten the screws\n", i+1);

			mail.Set_Mail_Data(Data);
			memset(Data, '\0', sizeof(Data));
		}
		
		
		mail.Load_Data("C++_Send_Mail\\Data.txt");
		
		mail.Send();
		
	}
	catch(ECMail e)
	{
		std::cout << "Error: " << e.GetErrorText() << "\n";
		bError = true;
	}

	if(!bError)
		std::cout << "信件已成功寄出\n";

	#pragma endregion

	mxDestroyArray(OutLable);
	mxDestroyArray(RHI);
	engClose(ep);
}
