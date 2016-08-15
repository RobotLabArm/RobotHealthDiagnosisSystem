#ifndef __OpenGLControl_H
#define __OpenGLControl_H
#endif

#pragma once
#include "stdafx.h"
#include <afxwin.h>
#include "global_value.h" 
#include "OpenGL/Include/freeglut.h"    //  opengl 進階版library 包含基本 glu.h 和gl.h

class COpenGLControl : public CWnd
{
public:
	COpenGLControl();
	virtual~COpenGLControl();
public:
	/******************/
	/* Public Members */
	/******************/
	UINT_PTR m_unpTimer;   //  
	bool	 m_bIsMaximized;
public:
	//--------------------使用按鍵的參數--------------------------------
	// View information variables (camera )
	bool r_mouse_button;// 看滑鼠右鍵是否被按住
	bool m_mouse_button;
	bool l_mouse_button;
	CPoint r_mouse_ini_pos; //按右鍵滑鼠座標起始位置
	CPoint m_mouse_ini_pos; //按中建滑鼠座標起始位置
	CPoint l_mouse_ini_pos;
	CPoint L_Button_Pos;
	double l_mouse_Pos[3];
	//--------------------------------------------------------------
	//--------Camera--------------
	static int Dwidth; //視窗的寬度
	static int Dhigh;  //視窗的高度
	GLdouble eye[3];
	GLdouble center[3];
	GLdouble up[3];
	double ModelView [16];
	double Projection[16];
	int    ViewPort[4];
	//--------------------宣告場景轉換的參數-------------------------
	float rot_m[2];
	float trans_m[2];
	float scale_m;
	float move_sensitivity;
	//------------------------------
public:
	/*******************/
	// Window information
	CWnd  *hWnd;    // Handle of parent window
	HDC   hdc;			// Handle of dc for openGl using
	HGLRC hrc;			//  RC for openGL
	int   m_nPixelFormat;  // pixel format
	CRect m_rect;    //  For getting window rect
	CRect m_oldWindow;   //  rect of previous window
	CRect m_originalRect;   // rect of original display window
	//void Create(CRect rect,CWnd* parent);  // create a mfc window
	//CClientDC* dc;   // Client window parameters
	//OpenGLDevice openGLDevice; // crate a mfc window library <windows>
	// Added message classes:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg	void OnDraw(CDC *pDC);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);

	//  window message box 滑鼠鍵盤事件
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);	
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP();

public:
	void openGLInitialize(void);
	void openGLCreate(CRect rect, CWnd *parent);
	void DrawScene(void);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//--------------------------------------------------------------------
	//  畫一堆有的沒的東西Function
public:
	void Draw_Axis(float LINK_LENGTH);   // 畫坐標軸
	void Draw_Cute_Axis (float LINK_LENGTH);
	void Draw_grid();
	void Draw_Atom_Robot_Stick();  // 畫黑金剛棒棒人旗標與棒子
	void Draw_NTU8DOF_Robot_Stick();  // 畫八軸棒棒人旗標與棒子
	static bool glFlagATomStick;
	static bool glFlagNTU8DOFStick;
	static bool glFlagDrawSphere;
	static float gl_NTU8DOF_P[24];
	static float gl_NTU8DOF_Rxyz [3];
	static void Draw_sphere(float r,float x, float y,float z);
	static void Draw_Cube(float Width, float Length, float Heigh);
	static int Draw_Robot_Stick(float gl_Robot_POS[],float gl_Robot_ORI[],int nDOF);
	//------------------------------------------------------------------------
	// 自行使用的控制變數
	static float PosX,PosY,PosZ,SphereRad;
};



//#endif

