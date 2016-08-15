/**************************************************************************************************
Copyright, 2013-2015, Robotics Lab., Dept. of M.E., National Taiwan University
File Name: TwinCAT_COM.h

Author: Kenneth Yi-wen Chao, Dora
Version: 0.85
Date: 2013/07/13 by Slongz

Functions:
     請詳見 Class-TwinCAT_COM

Classes: TwinCAT_COM

Description:
     本函式庫包含了對EtherCAT.iPOS馬達控制器進行控制/資料存取的函式
	 透過EtherCAT和TwinCAT進行連結並利用TwinCAT® I/O 和EtherCAT.iPOS進行資料的傳輸或存取

Note: 
	 修改建議:日後需要進行大量修改或次批次的傳值取值時,可先建立指標陣列將相關I/O的關係於建構子指定
	 可使敘述更整潔
	 Class下方有預列出可能可以增加的Func名稱
***************************************************************************************************/


#include <stdio.h>
#include <tchar.h>
#include "windows.h"
#include <SDKDDKVer.h>
#include <iostream>
#include "./IO Api/TCatIoApi.h" 	// header file shipped with TwinCAT® I/O 

#ifndef TASK_ARM_CST_H
#define TASK_ARM_CST_H
#include "TASK_ARM_CST.h"  //I/O定義檔,由TwinCAT匯出Header而得 
#endif    

using namespace std;
#pragma once

class TwinCAT_COM
{
public:
	
	TwinCAT_COM(void);
	~TwinCAT_COM(void);

	PTask_ARM_CST_Outputs pTARM1msOut; //Task_RLeg.h內的資料結構
	PTask_ARM_CST_Inputs pTARM1msIn; 

	//Funcs of Motor Control and Data acquisition

	void EtherCATInit_iPOS(void);				// TwinCAT初始化
	void EtherCATHoming_iPOS(void);				// RobotArm homing

	/****************************************
	******CST & CSP & CSV一次只能開一個******
	*****************************************/
	void EtherCATSetCST_iPOS(void);			    // Set RobotArm 為CST Mode or CSP Mode or CSV Mode  
 
	/**************Setting**************/                                                                      
	void EtherCATReadEncoder_iPOS(long* buf);	// 讀Robot Arm上六個軸的Encoder值
	void EtherCATReadTorque_iPOS(short* buf);	// 讀Robot Arm上六個軸的扭矩值(千分之Rated torque)
	void EtherCATReadVelocity_iPOS(long* buf);  // 讀Robot Arm上六個軸的Velocity值

	/****************Read****************/
	void EtherCATSetTorque_iPOS(long* buf);     // 設定Robot Arm要追的Torque Command (CST)
	void EtherCATSetEncoder_iPOS(long* buf);	// 設定Robot Arm要追的Encoder Command (CSP)
	void EtherCATSetVelocity_iPOS(long* buf);   // 設定Robot Arm要追的Velocity Command (CSV)

	void EtherCATEmergentStop(void);			// 緊急停止-Disable任何模式的控制
	unsigned short* pCtrlWords[6];				// Robot Arm Ctrlword 的指標陣列

	void EtherCATEmergentStop_iPOS(void);

	//Error Handling/Check
	bool BitCheck(unsigned short statword,bool value, int bitnumb);    // 檢查特定bit位址之二近位值
												
	//iPOS
	bool ReadyToSwitchOnCheck_iPOS(unsigned short statword);
	bool SwitchedOnCheck_iPOS(unsigned short statword);
	unsigned short iPOS_CSTErrorCheck(unsigned short statword);
	bool HomingCheck_iPOS(unsigned short statword);
};


