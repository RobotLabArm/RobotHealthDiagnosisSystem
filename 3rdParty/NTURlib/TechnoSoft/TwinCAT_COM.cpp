/**************************************************************************************************
Copyright, 2013-2016, Robotics Lab., Dept. of M.E., National Taiwan University
File Name: TwinCAT_COM.h

Author: Kenneth Yi-wen Chao, Dora
Version: 0.85
Date: 2016/01/05 

Functions:
     請詳見 Class-TwinCAT_COM

Classes: TwinCAT_COM

Description:
     本函式庫包含了對iPOS360x SY-CAT馬達控制器進行控制/資料存取的函式
	 透過EtherCAT和TwinCAT進行連結並利用TwinCAT® I/O 和iPOS360x SY-CAT進行資料的傳輸或存取

Note: 
	 修改建議:日後需要進行大量修改或次批次的傳值取值時,可先建立指標陣列將相關I/O的關係於建構子指定
	 可使敘述更整潔
	 Class下方有預列出可能可以增加的Func名稱
***************************************************************************************************/
#include "TwinCAT_COM.h"
#include <stdio.h>
#include <tchar.h>



TwinCAT_COM::TwinCAT_COM(void)
{
	/******************************************************************
	input: void
	output: void

	Note:
	// Class constructor  初始化 + Robot Arm Homing + Set CST Mode
	******************************************************************/
	EtherCATInit_iPOS(); // Control_Word = 6 ---> 7

	//還沒有Homing，以後再寫
	//EtherCATHoming_iPOS(); // Control_Word =  15 ---> 31 ---> 15
	EtherCATSetCST_iPOS(); // Control_Word =  15 ---> 10

	//Number by axis
	*(pCtrlWords+0) = &pTARM1msOut->Control_Word_01;
	*(pCtrlWords+1) = &pTARM1msOut->Control_Word_02;
	*(pCtrlWords+2) = &pTARM1msOut->Control_Word_03;
	*(pCtrlWords+3) = &pTARM1msOut->Control_Word_04;
	*(pCtrlWords+4) = &pTARM1msOut->Control_Word_05;
	*(pCtrlWords+5) = &pTARM1msOut->Control_Word_06;
}
void TwinCAT_COM::EtherCATInit_iPOS(void)
{
	/******************************************************************
	input: void
	output: void

	Note:
	// TwinCAT I/O 初始化
	   由6個獨立的while loop組成,按序列式依序進行初始化的動作
	   條件判斷式詳見iPOS360x SY-CAT user manual
	******************************************************************/
	int checklimit=10; //初始化嘗試連接次數 上限值
	if ( TCatIoOpen() == 0 ) 
	{ 
		// get the process image pointer 
		if ( TCatIoGetOutputPtr(TASK_ARM_CST_PORTNUMBER, (void**)&pTARM1msOut,sizeof(Task_ARM_CST_Outputs) ) == 0 && 
			TCatIoGetInputPtr( TASK_ARM_CST_PORTNUMBER, (void**)&pTARM1msIn, sizeof(Task_ARM_CST_Inputs) ) == 0 
			) 
		{ 
			int countercheck=0;
			long nError;
			long nError2;
			/*******************Initial 1st Axis*********************/
			while(1)
			{
				pTARM1msOut->Control_Word_01=6;  // Shutdown & Ready to Switch on = 6
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );  //test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(200);
				if (nError == 0 && nError2 ==0)
				{
					if( ReadyToSwitchOnCheck_iPOS(pTARM1msIn->Status_Word_01))
					{
						cout<<"Idle Mode Activated: Device R01 Shoutdown"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R01 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			countercheck = 0;
			nError = 0;
			nError2 = 0;
			while(1)
			{
				pTARM1msOut->Control_Word_01=7;// Switch on = 7
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );//test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(10);
				if (nError == 0 && nError2 ==0)
				{
					if( SwitchedOnCheck_iPOS(pTARM1msIn->Status_Word_01))
					{
						cout<<"Idle Mode Activated: Device R01 switched on"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R01 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			/*******************Initial 2nd Axis*********************/
			while(1)
			{
				pTARM1msOut->Control_Word_02=6;  // Shutdown & Ready to Switch on = 6
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );  //test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(200);
				if (nError == 0 && nError2 ==0)
				{
					if( ReadyToSwitchOnCheck_iPOS(pTARM1msIn->Status_Word_02))
					{
						cout<<"Idle Mode Activated: Device R02 Shoutdown"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R02 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			countercheck = 0;
			nError = 0;
			nError2 = 0;
			while(1)
			{
				pTARM1msOut->Control_Word_02=7;//  Switch on = 7
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );//test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(10);
				if (nError == 0 && nError2 ==0)
				{
					if( SwitchedOnCheck_iPOS(pTARM1msIn->Status_Word_02))
					{
						cout<<"Idle Mode Activated: Device R02 switched on"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R02 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			/*******************Initial 3rd Axis*********************/
			while(1)
			{
				pTARM1msOut->Control_Word_03=6;  // Shutdown & Ready to Switch on = 6
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );  //test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(200);
				if (nError == 0 && nError2 ==0)
				{
					if( ReadyToSwitchOnCheck_iPOS(pTARM1msIn->Status_Word_03))
					{
						cout<<"Idle Mode Activated: Device R03 Shoutdown"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R03 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			countercheck = 0;
			nError = 0;
			nError2 = 0;
			while(1)
			{
				pTARM1msOut->Control_Word_03=7;// Switch on = 7
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );//test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(10);
				if (nError == 0 && nError2 ==0)
				{
					if( SwitchedOnCheck_iPOS(pTARM1msIn->Status_Word_03))
					{
						cout<<"Idle Mode Activated: Device R03 switched on"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R03 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			/*******************Initial 4th Axis*********************/
			while(1)
			{
				pTARM1msOut->Control_Word_04=6;  // Shutdown & Ready to Switch on = 6
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );  //test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(200);
				if (nError == 0 && nError2 ==0)
				{
					if( ReadyToSwitchOnCheck_iPOS(pTARM1msIn->Status_Word_04))
					{
						cout<<"Idle Mode Activated: Device R04 Shoutdown"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R04 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			countercheck = 0;
			nError = 0;
			nError2 = 0;
			while(1)
			{
				pTARM1msOut->Control_Word_04=7;// Switch on = 7
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );//test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(10);
				if (nError == 0 && nError2 ==0)
				{
					if( SwitchedOnCheck_iPOS(pTARM1msIn->Status_Word_04))
					{
						cout<<"Idle Mode Activated: Device R04 switched on"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R04 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			/*******************Initial 5th Axis*********************/
			while(1)
			{
				pTARM1msOut->Control_Word_05=6;  // Shutdown & Ready to Switch on = 6
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );  //test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(200);
				if (nError == 0 && nError2 ==0)
				{
					if( ReadyToSwitchOnCheck_iPOS(pTARM1msIn->Status_Word_05))
					{
						cout<<"Idle Mode Activated: Device R05 Shoutdown"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R05 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			countercheck = 0;
			nError = 0;
			nError2 = 0;
			while(1)
			{
				pTARM1msOut->Control_Word_05=7;// Switch on = 7
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );//test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(10);
				if (nError == 0 && nError2 ==0)
				{
					if( SwitchedOnCheck_iPOS(pTARM1msIn->Status_Word_05))
					{
						cout<<"Idle Mode Activated: Device R05 switched on"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R05 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			/*******************Initial 6th Axis*********************/
			while(1)
			{
				pTARM1msOut->Control_Word_06=6;  // Shutdown & Ready to Switch on = 6
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );  //test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(200);
				if (nError == 0 && nError2 ==0)
				{
					if( ReadyToSwitchOnCheck_iPOS(pTARM1msIn->Status_Word_06))
					{
						cout<<"Idle Mode Activated: Device R06 Shoutdown"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R06 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
			countercheck = 0;
			nError = 0;
			nError2 = 0;
			while(1)
			{
				pTARM1msOut->Control_Word_06=7;// Switch on = 7
				nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );//test TCAT IO
				nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
				Sleep(10);
				if (nError == 0 && nError2 ==0)
				{
					if( SwitchedOnCheck_iPOS(pTARM1msIn->Status_Word_06))
					{
						cout<<"Idle Mode Activated: Device R06 switched on"<<endl;
						break;
					}
				}
				else
				{
					countercheck++;
					Sleep(20);
					if(countercheck>checklimit)
					{
						cout<<"R06 EtherCAT Error!"<<endl;
						system("pause");
					}
				}
			}
		} 
	}
	else
	{
		cout<<"Warning!TwinCAT is not opened"<<endl;
		system("pause");
	}
}
void TwinCAT_COM::EtherCATHoming_iPOS(void)//要改多軸
{
	/******************************************************************
	input: void
	output: void

	Note:
	// 透過TwinCAT I/O 進行Homing動作
	   組成方式為六組依序(由腳踝開始)的homing function
	   //Copy
				func ....
	   //Copy
	   請先確認數位電有開啟
	   條件判斷式詳見iPOS360x SY-CAT user manual的6.4章 Homing example
	******************************************************************/
	//pTARM1msOut->Control_Word_01=15; // Switch on + Enable poperation = 15
	//TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	//Sleep(50);

	//pTARM1msOut->LowSpeedForZero01=50; // Homing speed during search for zero.
	//TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	//Sleep(50);

	//pTARM1msOut->HighSpeedForSwitch01=100; // Homing speed during search for switch.
	//TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	//Sleep(50);

	//pTARM1msOut->HomingAcceleration01=64;  // Homing acceleration.
	//TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	//Sleep(50);

	//pTARM1msOut->HomeOffset01=0; // Home offset value, decide desired init pos.
	//TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	//Sleep(50);

	///*****************************************************************************************************************
	//Using method 21:
	//If home input is high, the initial direction of movement will be positive, or negative if home input is low.
	//Reverse (with slow speed) after home input low-high transition.
	//The motor will stop right after home switch high-low transition.
	//******************************************************************************************************************/
	//pTARM1msOut->HomingMethod01=21;
	//TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	//Sleep(50);

	//pTARM1msOut->Mode_Write_01=6; // Homing Mode = 6
	//TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	//Sleep(50);
	//cout<<"01 HomingMode Set"<<endl;

	//cout<<"Press Anykey to Start 01 Homing"<<endl; 
	//cout<<"Start 01 Homing ..."<<endl;
	//Sleep(50);
	//system("pause");	
	//pTARM1msOut->Control_Word_01=31; // Start the homing = 31
	//TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	//Sleep(50);

	//while(1)
	//{
	//	TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );
	//	if((pTARM1msIn->Status_Word_01>>12)%2 == 1) // When Status Word's bit13=0, bit12=1, bit10=1, means homing is completed.
	//	{
	//		cout<<"01 Homing Posi Attained"<<endl;			
	//		break;
	//	}
	//}

	//pTARM1msOut->Control_Word_01=15; // Switch on + Enable poperation = 15
	//TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	//Sleep(50);
	//system("pause");

	//cout<<"ARM Homing Finished"<<endl;
	//cout<<endl;
}

void TwinCAT_COM::EtherCATSetCST_iPOS(void)
{
	/*****************************************************************************************
	input: void
	output: void

	Note:
	// 透過TwinCAT I/O 進行Set CST Mode動作
	   組成方式為六組依序(由腳踝開始)的homing function
	   //Copy
				func ....
	   //Copy
	   模式啟動流程詳見iPOS360x SY-CAT user manual的14章 Cyclic synchronous torque mode
	******************************************************************************************/

	/*******************Set 1st Axis*********************/
	pTARM1msOut->Control_Word_01=15; // Switch on + Enable operation = 15
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);
	
	pTARM1msOut->Mode_Write_01=8;  // Cyclic sync Torque Mode = 10 & Cyclic sync Position Mode = 8
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);

	TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(50);
	if(	pTARM1msIn->Mode_Read_01==8 )
		cout<<"R01 CST set succeed"<<endl;
	else
		cout<<"R01 CST set failed"<<endl;
	cout<<endl;

	/*******************Set 2nd Axis*********************/
	pTARM1msOut->Control_Word_02=15; // Switch on + Enable operation = 15
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);
	
	pTARM1msOut->Mode_Write_02=8;  // Cyclic sync Torque Mode = 10 & Cyclic sync Position Mode = 8
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);

	TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(50);
	if(	pTARM1msIn->Mode_Read_02==8 )
		cout<<"R02 CST set succeed"<<endl;
	else
		cout<<"R02 CST set failed"<<endl;
	cout<<endl;

	/*******************Set 3rd Axis*********************/
	pTARM1msOut->Control_Word_03=15; // Switch on + Enable operation = 15
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);
	
	pTARM1msOut->Mode_Write_03=8;  // Cyclic sync Torque Mode = 10 & Cyclic sync Position Mode = 8
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);

	TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(50);
	if(	pTARM1msIn->Mode_Read_03==8 )
		cout<<"R03 CST set succeed"<<endl;
	else
		cout<<"R03 CST set failed"<<endl;
	cout<<endl;

	/*******************Set 4th Axis*********************/
	pTARM1msOut->Control_Word_04=15; // Switch on + Enable operation = 15
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);
	
	pTARM1msOut->Mode_Write_04=8;  // Cyclic sync Torque Mode = 10 & Cyclic sync Position Mode = 8
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);

	TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(50);
	if(	pTARM1msIn->Mode_Read_04==8 )
		cout<<"R04 CST set succeed"<<endl;
	else
		cout<<"R04 CST set failed"<<endl;
	cout<<endl;

	/*******************Set 5th Axis*********************/
	pTARM1msOut->Control_Word_05=15; // Switch on + Enable operation = 15
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);
	
	pTARM1msOut->Mode_Write_05=8;  // Cyclic sync Torque Mode = 10 & Cyclic sync Position Mode = 8
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);

	TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(50);
	if(	pTARM1msIn->Mode_Read_05==8 )
		cout<<"R05 CST set succeed"<<endl;
	else
		cout<<"R05 CST set failed"<<endl;
	cout<<endl;

	/*******************Set 6th Axis*********************/
	pTARM1msOut->Control_Word_06=15; // Switch on + Enable operation = 15
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);
	
	pTARM1msOut->Mode_Write_06=8;  // Cyclic sync Torque Mode = 10 & Cyclic sync Position Mode = 8
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(500);

	TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(50);
	if(	pTARM1msIn->Mode_Read_06==8 )
		cout<<"R06 CST set succeed"<<endl;
	else
		cout<<"R06 CST set failed"<<endl;
	cout<<endl;
}
void TwinCAT_COM::EtherCATEmergentStop_iPOS(void)
{
	/******************************************************************
	input: void
	output: void

	Note:
	// 透過TwinCAT I/O 進行Mode disalbe的動作
	   函式執行後機器手臂整個鬆開
	******************************************************************/
	long nError;
	long nError2;
	static int iii=0; 
	if ( ((nError = TCatIoInputUpdate( TASK_ARM_CST_PORTNUMBER )) == 0 )&& ((nError2 = TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER )) == 0 )) 
	{ 
		pTARM1msOut->Control_Word_01=6; // Shutdown & Ready to Switch on = 6
		pTARM1msOut->Control_Word_02=6; // Shutdown & Ready to Switch on = 6
		pTARM1msOut->Control_Word_03=6; // Shutdown & Ready to Switch on = 6
		pTARM1msOut->Control_Word_04=6; // Shutdown & Ready to Switch on = 6
		pTARM1msOut->Control_Word_05=6; // Shutdown & Ready to Switch on = 6
		pTARM1msOut->Control_Word_06=6; // Shutdown & Ready to Switch on = 6
		TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
		cout<<"Emergent Stop!!!"<<endl;
	}
	else 
		printf( "TCatInputUpdate(%d) %d failed with 0x%x !\n",
		TASK_ARM_CST_PORTNUMBER, iii++, nError );
}
void TwinCAT_COM::EtherCATSetTorque_iPOS(long* buf)
{
	/******************************************************************************************************************************
	input: long* buf
	output: void

	Note:
	// 將指標陣列儲存的數值透過TwinCAT I/O 進行指定Target Encoder動作
	   須將TwinCAT I/O Output 部分進行update才會開始動作

	   Current[IU] = (65520*Current[A]) / (2*IPeak)
	   where Ipeak is the peak current supported by the drive and current[IU] is the command value for Target torque (Torque_Set).
	   ****************************************************************************************************************************/
	long AxisEnable[6]={1,1,1,1,1,1};
	
	// Number by axis
	pTARM1msOut->Torque_Set_01=*(buf+0)*AxisEnable[0];
	pTARM1msOut->Torque_Set_02=*(buf+1)*AxisEnable[1];
	pTARM1msOut->Torque_Set_03=*(buf+2)*AxisEnable[2];
	pTARM1msOut->Torque_Set_04=*(buf+3)*AxisEnable[3];
	pTARM1msOut->Torque_Set_05=*(buf+4)*AxisEnable[4];
	pTARM1msOut->Torque_Set_06=*(buf+5)*AxisEnable[5];
}
void TwinCAT_COM::EtherCATReadTorque_iPOS(short* buf)
{
	/******************************************************************
	input: long* buf
	output: void

	Note:
	// 透過TwinCAT I/O 進行讀取Torque(千分之rated torque)動作
	   須將TwinCAT I/O Input 部分進行update才會開始動作	     
	******************************************************************/
	*(buf+0)=pTARM1msIn->Torque_Read_01;
	*(buf+1)=pTARM1msIn->Torque_Read_02;
	*(buf+2)=pTARM1msIn->Torque_Read_03;
	*(buf+3)=pTARM1msIn->Torque_Read_04;
	*(buf+4)=pTARM1msIn->Torque_Read_05;
	*(buf+5)=pTARM1msIn->Torque_Read_06;
}
void TwinCAT_COM::EtherCATSetEncoder_iPOS(long* buf)
{
	/******************************************************************
	input: long* buf
	output: void

	Note:
	// 將指標陣列儲存的數值透過TwinCAT I/O 進行指定Target Encoder動作
	   須將TwinCAT I/O Output 部分進行update才會開始動作

	   Position[rad] = (2*pi*Position[IU]) / (4*Encoder_lines)
	******************************************************************/
	long AxisEnable[6]={1,1,1,1,1,1};
	
	// Number by axis
	pTARM1msOut->Position_Set_01=*(buf+0)*AxisEnable[0];
	pTARM1msOut->Position_Set_02=*(buf+1)*AxisEnable[1];
	pTARM1msOut->Position_Set_03=*(buf+2)*AxisEnable[2];
	pTARM1msOut->Position_Set_04=*(buf+3)*AxisEnable[3];
	pTARM1msOut->Position_Set_05=*(buf+4)*AxisEnable[4];
	pTARM1msOut->Position_Set_06=*(buf+5)*AxisEnable[5];
}
void TwinCAT_COM::EtherCATReadEncoder_iPOS(long* buf)
{
	/******************************************************************
	input: long* buf
	output: void

	Note:
	// 透過TwinCAT I/O 進行讀取Encoder動作
	   須將TwinCAT I/O Input 部分進行update才會開始動作
	******************************************************************/
	*(buf+0)=pTARM1msIn->Position_Read_01;
	*(buf+1)=pTARM1msIn->Position_Read_02;
	*(buf+2)=pTARM1msIn->Position_Read_03;
	*(buf+3)=pTARM1msIn->Position_Read_04;
	*(buf+4)=pTARM1msIn->Position_Read_05;
	*(buf+5)=pTARM1msIn->Position_Read_06;
}
void TwinCAT_COM::EtherCATSetVelocity_iPOS(long* buf)
{
	/******************************************************************
	input: long* buf
	output: void

	Note:
	// 將指標陣列儲存的數值透過TwinCAT I/O 進行指定Target Encoder動作
	   須將TwinCAT I/O Output 部分進行update才會開始動作

	   Velocity[rad] = (2*pi*Velocity[IU]) / (4*Encoder_lines*0.001*65536)
	******************************************************************/
	long AxisEnable[6]={1,1,1,1,1,1};
	
	// Number by axis
	pTARM1msOut->Velocity_Set_01=*(buf+0)*AxisEnable[0];
	pTARM1msOut->Velocity_Set_02=*(buf+1)*AxisEnable[1];
	pTARM1msOut->Velocity_Set_03=*(buf+2)*AxisEnable[2];
	pTARM1msOut->Velocity_Set_04=*(buf+3)*AxisEnable[3];
	pTARM1msOut->Velocity_Set_05=*(buf+4)*AxisEnable[4];
	pTARM1msOut->Velocity_Set_06=*(buf+5)*AxisEnable[5];
}
void TwinCAT_COM::EtherCATReadVelocity_iPOS(long* buf)
{
	/******************************************************************
	input: long* buf
	output: void

	Note:
	// 透過TwinCAT I/O 進行讀取Velocity動作
	   須將TwinCAT I/O Input 部分進行update才會開始動作
	******************************************************************/
	*(buf+0)=pTARM1msIn->Velocity_Read_01;
	*(buf+1)=pTARM1msIn->Velocity_Read_02;
	*(buf+2)=pTARM1msIn->Velocity_Read_03;
	*(buf+3)=pTARM1msIn->Velocity_Read_04;
	*(buf+4)=pTARM1msIn->Velocity_Read_05;
	*(buf+5)=pTARM1msIn->Velocity_Read_06;
}
bool TwinCAT_COM::BitCheck(unsigned short statword,bool value, int bitnumb) //bitnumb count from 0
{
	/******************************************************************
	input: unsigned short statword,bool value, int bitnumb
	output: ture/false

	Note:
	// 進行特定位址之二近位數值大小判斷	     
	******************************************************************/
	if(   (statword>>bitnumb)%2==value        )
		return true;
	else
		return false;
}
bool TwinCAT_COM::ReadyToSwitchOnCheck_iPOS(unsigned short statword)
{
	/******************************************************************
	input: unsigned short statword
	output: ture/false

	Note:
	// 配合EtherCATInit使用
	   進行Statusword Disabled與否的狀態判讀
	   若是結果為disalbed(true),則可進行Initialization
	   狀態判別詳見Epos3 Firmware specifications & Application note
	******************************************************************/
	if(    BitCheck(statword,false,3)  &&  BitCheck(statword,false,2)  &&  BitCheck(statword,false,1)  &&  BitCheck(statword,true,0)  && 	
		   BitCheck(statword,false,6)  &&  BitCheck(statword,true,5)   
	 )
		return true;//Ready to Switch On    Drive parameters may be changed Drive function is disabled
	else
		return false;
}
bool TwinCAT_COM::SwitchedOnCheck_iPOS(unsigned short statword)
{
	/******************************************************************
	input: unsigned short statword
	output: ture/false

	Note:
	// 配合EtherCATInit使用
	   進行Statusword Disabled與否的狀態判讀
	   若是結果為disalbed(true),則可進行Initialization
	   狀態判別詳見Epos3 Firmware specifications & Application note
	******************************************************************/
	if(    BitCheck(statword,false,3)  &&  BitCheck(statword,false,2)  &&  BitCheck(statword,true,1)  &&  BitCheck(statword,true,0)  && 	
		   BitCheck(statword,false,6)  &&  BitCheck(statword,true,5)  
	 )
	 	return true;//Switched On    Drive function is disabled
	else
		return false;
}

bool TwinCAT_COM::HomingCheck_iPOS(unsigned short statword)
{
	/******************************************************************
	input: unsigned short statword
	output: ture/false

	Note:
	// 配合EtherCATInit使用
	   進行Statusword Disabled與否的狀態判讀
	   若是結果為disalbed(true),則可進行Initialization
	   狀態判別詳見Epos3 Firmware specifications & Application note
	******************************************************************/
	if(    BitCheck(statword,false,13)  &&  BitCheck(statword,true,12)  &&  BitCheck(statword,true,10)     
	 )
		return true;//Ready to Switch On    Drive parameters may be changed Drive function is disabled
	else
		return false;
}
unsigned short TwinCAT_COM::iPOS_CSTErrorCheck(unsigned short statword)
{
	/******************************************************************
	input: unsigned short statword
	output: ture/false

	Note:
	// 進行Statusword ERROR的狀態(i.e.是否有error)與種類判讀
	   狀態判別詳見iPOS 
	   //無CST專用ERROR 只有一般ERROR
	******************************************************************/
	if(    BitCheck(statword,true,3))//Having error
	 	return pTARM1msIn->Motion_Err_Reg_01;
	else
		return 0;//no error
}
TwinCAT_COM::~TwinCAT_COM(void)
{
	/******************************************************************
	input: void
	output: void

	Note:
	// Class destructor  關閉TwinCAT I/O
	******************************************************************/
	pTARM1msOut->Control_Word_01=6; // Shutdown & Ready to Switch on = 6
	pTARM1msOut->Control_Word_02=6; // Shutdown & Ready to Switch on = 6
	pTARM1msOut->Control_Word_03=6; // Shutdown & Ready to Switch on = 6
	pTARM1msOut->Control_Word_04=6; // Shutdown & Ready to Switch on = 6
	pTARM1msOut->Control_Word_05=6; // Shutdown & Ready to Switch on = 6
	pTARM1msOut->Control_Word_06=6; // Shutdown & Ready to Switch on = 6
	TCatIoOutputUpdate( TASK_ARM_CST_PORTNUMBER );
	Sleep(20);

	TCatIoClose(); 
}