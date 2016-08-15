
#include "StdAfx.h"
#include "OpenGLControl.h"
#include "global_value.h"
#ifndef PI
#define PI 3.14159
#endif

#include "ProjectPlay.h"   //  測試Project
//   初始化IO變數  static寫法
bool COpenGLControl::glFlagATomStick = false;
bool COpenGLControl::glFlagNTU8DOFStick = false;
float COpenGLControl::gl_NTU8DOF_P[24] = {0.0};
float COpenGLControl::gl_NTU8DOF_Rxyz[3] = {0.0};
int COpenGLControl::Dwidth = 100; //視窗的寬度
int COpenGLControl::Dhigh = 100;  //視窗的高度

float COpenGLControl::PosX = 0;
float COpenGLControl::PosY = 0;
float COpenGLControl::PosZ = 0;
float COpenGLControl::SphereRad = 0.05;

COpenGLControl::COpenGLControl()
: r_mouse_button(false)
, m_mouse_button(false)
, l_mouse_button(false)
, r_mouse_ini_pos(0)
, m_mouse_ini_pos(0)
{
	//---------------------------------------------------------------------------------
	// 初始場景的視角參數
	//--------initial 場景轉換的參數--------------------------------------------------
	rot_m [0] = -50.0f ;   // Rx
	rot_m [1] = 0.0;       //  Ry
	trans_m[0] = 0;
	trans_m[1] = 1;
	scale_m = 3.0;
	move_sensitivity = 1.0;
	//--------------Initial Camera----------------------------
	eye[0]    = -2;	eye[1]    = 0;	eye[2]    = 1.5;
	center[0] = 0;	center[1] = 0;	center[2] = 0;
	up[0]     = 0;  up[1]     = 0;	up[2]     = 1;
	// ------------------------------------------------------
	m_bIsMaximized = false;
}

COpenGLControl::~COpenGLControl()
{
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(this->hrc);
	DeleteDC(this->hdc);

}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;
	this->openGLInitialize();  //  初始化opneGL相關參數

	return 0;
}
void COpenGLControl::OnPaint()
{
	//OpenGL是使用Timer來進行畫面的繪製，因此就不適用於MFC所提供的繪製畫面的方法。
	//因此需要跳過Windows繪圖的函式 - OnPaint()(WM_PAINT)。在OnPaint()內，
	//使用ValidateRect()來讓Picture Control不被更新
	ValidateRect(NULL);  //Remove all of region of the client from the update region

}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	//當視窗有需要自由變更大小時，則需改寫OnSize()(WM_SIZE)
	//記得改寫Dialog的OnSize()。再由Dialog來呼叫OpenGL class來進行重新繪製
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;
	//Resize
	switch (nType)
	{
		// If window resize token is "maximize"
	case SIZE_MAXIMIZED:
		{
			// Get the current window rect
			GetWindowRect(m_rect);

			// Move the window accordingly
			MoveWindow(6, 6, cx - 14, cy - 14);

			// Get the new window rect
			GetWindowRect(m_rect);

			// Store our old window as the new rect
			m_oldWindow = m_rect;

			break;
		}

		// If window resize token is "restore"
	case SIZE_RESTORED:
		{
			// If the window is currently maximized
			if (m_bIsMaximized)
			{
				// Get the current window rect
				GetWindowRect(m_rect);

				// Move the window accordingly (to our stored old window)
				MoveWindow(m_oldWindow.left, m_oldWindow.top - 18, m_originalRect.Width() - 4, m_originalRect.Height() - 4);

				// Get the new window rect
				GetWindowRect(m_rect);

				// Store our old window as the new rect
				m_oldWindow = m_rect;
			}

			break;
		}
	}
	//Map the OpengGL coordinates  
	glViewport(0, 0, cx, cy);  
	//Projection view  
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();  
	//Set our current view perspective  
	gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);  
	//Mode view  
	glMatrixMode(GL_MODELVIEW);  
}

void COpenGLControl::OnTimer(UINT nIDEvent)
{
	//新增圖像更新用的OnTimer()(WM_TIMER)
	switch (nIDEvent)
	{
	case 1:
		{
			// Clear color and depth buffer bits
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw OpenGL scene
			DrawScene();

			// Swap buffers
			SwapBuffers(hdc);

			break;
		}

	default:
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void COpenGLControl::OnDraw(CDC *pdc)
{

	// If the current view is perspective...
	// TODO: Camera controls.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();

	GLdouble aspect = Dwidth/Dhigh;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55.0f,aspect,0.1f,20.0f);
	glMatrixMode(GL_MODELVIEW);

	move_sensitivity = 5*scale_m;
	center[0] = -cos(rot_m[0])*sin(rot_m[1])*trans_m[1] + sin(rot_m[0])*trans_m[0];
	center[1] =	-sin(rot_m[0])*sin(rot_m[1])*trans_m[1] - cos(rot_m[0])*trans_m[0];
	center[2] =  cos(rot_m[1])*trans_m[1];
	//-----------------------攝影機位置----------------------------------------------
	eye[0] = scale_m*cos(rot_m[0])*cos(rot_m[1])+center[0];
	eye[1] = scale_m*sin(rot_m[0])*cos(rot_m[1])+center[1];
	eye[2] = scale_m*sin(rot_m[1])+center[2];
	up[0] = -cos(rot_m[0])*sin(rot_m[1]);
	up[1] = -sin(rot_m[0])*sin(rot_m[1]);
	up[2] =  cos(rot_m[1]);	
	gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],up[0],up[1],up[2]);

	glGetDoublev(GL_MODELVIEW_MATRIX,ModelView);
	glGetDoublev(GL_PROJECTION_MATRIX,Projection);
	glGetIntegerv(GL_VIEWPORT,ViewPort);
}

void COpenGLControl::openGLInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	hdc = GetDC()->m_hDC;

	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	// Basic Setup: 
	//
	// Set color to use when clearing the background.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  //  
	glClearDepth(1.0f);


	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// Send draw request
	OnDraw(NULL);

}


void COpenGLControl::openGLCreate(CRect rect, CWnd *parent)
{
	//向系統註冊Class、建立OpenGL使用之視窗屬性以及記錄視窗的初始設定
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx(0, className, LPCSTR("OpenGL"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

	// Set initial variables' values
	this->m_oldWindow	   = rect;
	this->m_originalRect = rect;

	this->hWnd = parent;
}
//-------------   畫一堆有的沒的東西  ------------------
void COpenGLControl::DrawScene(void)
{
	if (glFlagATomStick)
	{	
		glPushMatrix();
		glTranslatef(0.5,-0.0,0);
		Draw_Atom_Robot_Stick();   // 畫出黑金鋼棒棒人
		glPopMatrix();
	}
	if (glFlagNTU8DOFStick)
	{
		glPushMatrix();
		glTranslatef(-0.0,0.55,0);
		Draw_NTU8DOF_Robot_Stick();   // 畫出八軸棒棒人
		glPopMatrix();
	}

	// 畫地板
	glPushMatrix();
	glTranslatef(0,0,0);
	glPushMatrix();
	glRotatef(0,1,0,0);
	glRotatef(0,0,1,0);
	glRotatef(0,0,0,1);
	Draw_Axis(50);
	Draw_grid();
	glPopMatrix();
	glPopMatrix();

}
void COpenGLControl::Draw_Axis(float LINK_LENGTH)
{
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	float LINK_RADIUS  = 0.005;

	glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();
	glColor3f(0.8,0,0);
	gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,20,32);
	glTranslatef(0, 0,LINK_LENGTH);
	glutSolidCone(0.008,0.012,10,10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0,0.8,0);
	glRotatef(-90,1,0,0);
	gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,20,32);
	glTranslatef(0, 0 ,LINK_LENGTH);
	glutSolidCone(0.008,0.012,10,10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0,0,0.8);
	glRotatef(90,0,1,0);
	gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,20,32);
	glTranslatef(0, 0,LINK_LENGTH);
	glutSolidCone(0.008,0.012,10,10);
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
}
void COpenGLControl::Draw_Cute_Axis(float LINK_LENGTH)
{
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	float LINK_RADIUS  = 0.01;

	glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,30,30);
	glTranslatef(0, 0,LINK_LENGTH);
	glutWireCone(0.03,0.05,150,150);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,1.0,0.0);
	glRotatef(-90,1,0,0);
	gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,30,30);
	glTranslatef(0, 0 ,LINK_LENGTH);
	glutWireCone(0.03,0.05,150,150);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.0,1.0);
	glRotatef(90,0,1,0);
	gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,30,30);
	glTranslatef(0, 0,LINK_LENGTH);
	glutWireCone(0.03,0.05,150,150);
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
}
void COpenGLControl::Draw_Atom_Robot_Stick()
{
	///////////畫一個箱子/////////////////
	glPushMatrix();
	glTranslatef(-0.5,-0.25,0);
	Draw_Cube(0.5,0.5,1.2);
	glPopMatrix();
	Matrix Tlast(4,4),T(4,4);
	float t[16], tlast[16];
	for (int i=0;i<gAtom_DOF;i++)
	{
		T = robot_atom.kine(i+1).t();
		if (i!=0)
			Tlast = robot_atom.kine(i).t();
		else Tlast = 0;
		for(int l = 0; l < 4; ++l)
			for(int m = 0; m < 4; ++m)
				t[l*4+m] = T(l+1,m+1);

		if (i==gAtom_DOF-1)
		{
			//  畫出end箭頭
			glPushMatrix();
			glMultMatrixf(t);
			Draw_Cute_Axis(0.08);
			glPopMatrix();
			// 畫出棒棒
			glLineWidth(5.0); 
			glColor3f(0.007, 0.968, 0.564);
			glBegin(GL_LINES);
			glVertex3f(Tlast(4,1),Tlast(4,2),Tlast(4,3));
			glVertex3f(T(4,1),T(4,2),T(4,3));
			glEnd();
		}
		else
		{
			//  畫出DOF個Joint的點
			glPushMatrix();
			glMultMatrixf(t);
			glPushMatrix();
			glColor3f(0.95,0.5,0);
			glutSolidSphere(0.03,10,10);
			glPopMatrix();
			glPopMatrix();
			// 畫出棒棒
			glLineWidth(5.0); 
			glColor3f(0.007, 0.968, 0.564);
			glBegin(GL_LINES);
			glVertex3f(Tlast(4,1),Tlast(4,2),Tlast(4,3));
			glVertex3f(T(4,1),T(4,2),T(4,3));
			glEnd();
		}
	}
}
void COpenGLControl::Draw_NTU8DOF_Robot_Stick()
{
	///////////畫一個箱子/////////////////
	glPushMatrix();
	glTranslatef(-0.5,-0.25,0);
	Draw_Cube(0.5,0.5,1.2);
	glPopMatrix();
	//glLoadIdentity();
	int count = 0;
	//  畫出DOF個Joint的點
	for (int i=0;i<gNTU8DOF_DOF;i++)
	{	
		glPushMatrix();
		glTranslatef(gl_NTU8DOF_P[0+count],gl_NTU8DOF_P[1+count],gl_NTU8DOF_P[2+count]);
		glPushMatrix();
			glColor3f(0.95,0.5,0);
		glutSolidSphere(0.03,10,10);
		glPopMatrix();
		glPopMatrix();

		if (i==gNTU8DOF_DOF-1)
		{
			glPushMatrix();
			glTranslatef(gl_NTU8DOF_P[0+count],gl_NTU8DOF_P[1+count],gl_NTU8DOF_P[2+count]);
			glRotatef(gl_NTU8DOF_Rxyz[0],1,0,0);
			glRotatef(gl_NTU8DOF_Rxyz[1],0,1,0);
			glRotatef(gl_NTU8DOF_Rxyz[2],0,0,1);
			//glPushMatrix();
			//glColor3f(0.8,0 00.0,0.5);
			Draw_Cute_Axis(0.08);
			//glPopMatrix();
			glPopMatrix();
		}
		count = count+3;
	}
	// 畫出棒棒Link
	count = 0;
	for (int i=0;i<gNTU8DOF_DOF-1;i++){
		glLineWidth(5.0); 
		glColor3f(0.007, 0.968, 0.564);
		glBegin(GL_LINES);
		glVertex3f( gl_NTU8DOF_P[0+count], 
			gl_NTU8DOF_P[1+count], 
			gl_NTU8DOF_P[2+count]);
		glVertex3f( gl_NTU8DOF_P[3+count], 
			gl_NTU8DOF_P[4+count],
			gl_NTU8DOF_P[5+count]);
		glEnd();
		count = count+3;
	}
}
void COpenGLControl::Draw_grid()
{
	GLfloat fSizes[2];     // Line width range metrics
	GLfloat fCurrSize;     // Save current size
	glGetFloatv(GL_LINE_WIDTH_RANGE,fSizes);
	fCurrSize = fSizes[0];
	glLineWidth(fCurrSize);

	int Width[2]  = {-10,10};
	int Length[2] = {-10,10};
	float scale=0.5;
	glColor3f(0.3,0.3,0.3);
	//glLineStipple(1,0xaaaa);
	//glEnable(GL_LINE_STIPPLE);
	for(float x=Width[0] ; x <= Width[1] ;x=x+scale)
	{
		glBegin(GL_LINES);
		glVertex3f(x,Length[0],0);
		glVertex3f(x,Length[1],0);
		glEnd();
	}
	for(float y=Length[0] ; y <= Length[1] ;y=y+scale)
	{
		glBegin(GL_LINES);
		glVertex3f(Width[0],y,0);
		glVertex3f(Width[1],y,0);
		glEnd();
	}
	//glDisable(GL_LINE_STIPPLE);
	glColor3f(1,1,1);
}
void COpenGLControl::Draw_Cube( float Width, float Length, float Height)
{
	// width, length, height are arranged in x, y, z directions

	if( Width == 0 || Length == 0 || Height == 0 ) {
		return; // draw nothing
	}

	//glEnable( GL_BLEND ); // required for showing alpha
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); // required for showing alpha
	//glAlphaFunc( GL_GREATER, 0.01 );
	//glEnable( GL_ALPHA_TEST );

	// When rendering polygon, please render counter-clockwise for correct display!!!
	// button face
	glColor4f( 0.3, 0.3, 0.3, 1);

	glBegin( GL_POLYGON );
	glNormal3f( 0.0, 0.0, -1.0 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 0.0, Length, 0.0 );
	glVertex3f( Width, Length, 0.0 );
	glVertex3f( Width, 0.0, 0.0 );
	glEnd();
	// side face 1
	glBegin( GL_POLYGON );
	glNormal3f( 0.0, -1.0, 0.0 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( Width, 0.0, 0.0 );
	glVertex3f( Width, 0.0, Height );
	glVertex3f( 0.0, 0.0, Height );
	glEnd();
	// side face 2
	glBegin( GL_POLYGON );
	glNormal3f( 1.0, 0.0, 0.0 );
	glVertex3f( Width, 0.0, 0.0 );
	glVertex3f( Width, Length, 0.0 );
	glVertex3f( Width, Length, Height );
	glVertex3f( Width, 0.0, Height );
	glEnd();
	// side face 3
	glBegin( GL_POLYGON );
	glNormal3f( 0.0, 1.0, 0.0 );
	glVertex3f( 0.0, Length, 0.0 );
	glVertex3f( 0.0, Length, Height );
	glVertex3f( Width, Length, Height );
	glVertex3f( Width, Length, 0.0 );
	glEnd();
	// side face 4
	glBegin( GL_POLYGON );
	glNormal3f( -1.0, 0.0, 0.0 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 0.0, 0.0, Height );
	glVertex3f( 0.0, Length, Height );
	glVertex3f( 0.0, Length, 0.0 );
	glEnd();
	// top face 
	glBegin( GL_POLYGON );
	glNormal3f( 0.0, 0.0, 1.0 );
	glVertex3f( 0.0, 0.0, Height );
	glVertex3f( Width, 0.0, Height );
	glVertex3f( Width, Length, Height );
	glVertex3f( 0.0, Length, Height );
	glEnd();

}
void COpenGLControl::Draw_sphere(float r,float x, float y,float z)
{
	PosX = x;PosY = y;PosZ = z;SphereRad = r;
}

int COpenGLControl::Draw_Robot_Stick(float gl_Robot_POS[],float gl_Robot_ORI[],int nDOF)
{
	///////////畫一個箱子/////////////////
	//glPushMatrix();
	//glTranslatef(-0.5,-0.25,0);
	//Draw_Cube(0.5,0.5,1.2);
	//glPopMatrix();

	int count = 0;
	int Posnum;
	Posnum = 3*nDOF;
	//  畫出DOF個Joint的點
	for (int i=0;i<nDOF;i++)
	{	
		glPushMatrix();
		glTranslatef(gl_Robot_POS[0+count],gl_Robot_POS[1+count],gl_Robot_POS[2+count]);
		glPushMatrix();
		glColor3f(0.95,0.5,0);
		glutSolidSphere(0.03,10,10);
		glPopMatrix();
		glPopMatrix();

		if (i==nDOF-1)
		{
			glPushMatrix();
			glTranslatef(gl_Robot_POS[0+count],gl_Robot_POS[1+count],gl_Robot_POS[2+count]);
			glRotatef(gl_Robot_ORI[0],1,0,0);
			glRotatef(gl_Robot_ORI[1],0,1,0);
			glRotatef(gl_Robot_ORI[2],0,0,1);
			//glPushMatrix();
			//Draw_Cute_Axis(0.08);
			//glPopMatrix();
			glPopMatrix();
		}
		count = count+3;
	}
	// 畫出棒棒Link
	count = 0;
	for (int i=0;i<nDOF-1;i++){
		glLineWidth(5.0); 
		glColor3f(0.007, 0.968, 0.564);
		glBegin(GL_LINES);
		glVertex3f( gl_Robot_POS[0+count], 
			gl_Robot_POS[1+count], 
			gl_Robot_POS[2+count]);
		glVertex3f( gl_Robot_POS[3+count], 
			gl_Robot_POS[4+count],
			gl_Robot_POS[5+count]);
		glEnd();
		count = count+3;
	}

	return 0;
}

////------------------------------------------------

//------------ 滑鼠鍵盤事件-----------------
BOOL COpenGLControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	//// TODO: Add your message handler code here and/or call default  
	//int diffX = (int)(point.x - this->m_fLastX);  
	//int diffY = (int)(point.y - this->m_fLastY);  
	//this->m_fLastX = (float)point.x;  
	//this->m_fLastY = (float)point.y;  
	//if ( zDelta == WHEEL_DELTA )                    //Rolls to forward - Zoom in  
	//{  
	//	this->m_fZoom -= (float)0.02f * diffY;  
	//}else if (zDelta == -WHEEL_DELTA) {        //Rolls to back - Zoom out  
	//	this->m_fZoom += (float)0.02f * diffY;  
	//}    //End of if ( zDelta > 0 )  

	this->OnDraw(NULL);  
	return CWnd::OnMouseWheel(nFlags, zDelta, point); 
}

void COpenGLControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	r_mouse_button = true;
	r_mouse_ini_pos = point;

	CWnd::OnRButtonDown(nFlags, point);
}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	float scale_sensitivity = 0.01;
	float trans_sensitivity = 0.02;
	float rot_sensitivity   = 0.01;

	if(r_mouse_button == true && m_mouse_button == false)//只按右鍵 旋轉
	{
		float rotx=(point.x-r_mouse_ini_pos.x)*rot_sensitivity;
		float roty=(point.y-r_mouse_ini_pos.y)*rot_sensitivity;
		rot_m[0]-=rotx;
		rot_m[1]+=roty;		
		r_mouse_ini_pos=point;
	}

	if(r_mouse_button && m_mouse_button)//按右鍵和中鍵 縮放
	{
		float scale_temp =- (point.y-m_mouse_ini_pos.y)*scale_sensitivity;
		scale_m += scale_temp;
		m_mouse_ini_pos = point;
	}

	if(m_mouse_button == true && r_mouse_button == false)//指案中鍵 位移
	{
		float transx=(point.x-m_mouse_ini_pos.x)*trans_sensitivity;
		float transy=(point.y-m_mouse_ini_pos.y)*trans_sensitivity;
		trans_m[0] += transx;
		trans_m[1] += transy;
		m_mouse_ini_pos = point;
	}

	OnDraw(NULL);
	CWnd::OnMouseMove(nFlags, point);
}


void COpenGLControl::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	r_mouse_button = false;

	CWnd::OnRButtonUp(nFlags, point);
}
void COpenGLControl::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	m_mouse_button  = true;
	m_mouse_ini_pos = point;

	CWnd::OnMButtonDown(nFlags, point);
}

void COpenGLControl::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	m_mouse_button=false;

	CWnd::OnMButtonUp(nFlags, point);
}

void COpenGLControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	//// TODO: Add your message handler code here and/or call default
	//l_mouse_button  = true;
	//l_mouse_ini_pos = point;

	////===============轉換視角(視窗轉OpenGL)==============
	//float winPoint[] = {(float)point.x , (float)(ViewPort[3]-point.y) , 0};
	//double objPoint[3] = {0};
	////glReadPixels( int(winPoint[0]), int(winPoint[1]), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winPoint[2] );
	//if(gluUnProject(winPoint[0],winPoint[1],winPoint[2],ModelView,Projection,ViewPort,&objPoint[0],&objPoint[1],&objPoint[2]) != GL_TRUE)
	//	printf("UnProject is ERROR\n");
	//for(int i = 0 ; i < 3 ; i++)
	//	l_mouse_Pos[i] = objPoint[i];

	//L_Button_Pos = point;
	//L_Button_Pos.x=( (long)(L_Button_Pos.x)-300)/5.3/scale_m;
	//L_Button_Pos.y=(-(long)(L_Button_Pos.y)+300)/5.3/scale_m;

	//CWnd::OnLButtonDown(nFlags, point);
}

void COpenGLControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	l_mouse_button = false;

	CWnd::OnLButtonUp(nFlags, point);
}



