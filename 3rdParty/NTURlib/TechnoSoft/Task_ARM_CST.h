///////////////////////////////////////////////////////////////////////////////
// 
// Beckhoff Automation GmbH
// 
// TwinCAT IO HeaderFile
// 
///////////////////////////////////////////////////////////////////////////////
// 
// D:\¤jµ´©Û\TCAT\Task_ARM_CST.h

#define	TASK_ARM_CST_PORTNUMBER	301

#define	TASK_ARM_CST_INPUTSIZE	66
#define	TASK_ARM_CST_OUTPUTSIZE	54

#pragma pack(push, 1)

typedef struct
{
	unsigned short	Status_Word_01;
	unsigned short	Status_Word_02;
	unsigned short	Status_Word_03;
	unsigned short	Status_Word_04;
	unsigned short	Status_Word_05;
	unsigned short	Status_Word_06;
	long	Position_Read_01;
	long	Position_Read_02;
	long	Position_Read_03;
	long	Position_Read_04;
	long	Position_Read_05;
	long	Position_Read_06;
	short	Torque_Read_01;
	short	Torque_Read_02;
	short	Torque_Read_03;
	short	Torque_Read_04;
	short	Torque_Read_05;
	short	Torque_Read_06;
	char	Mode_Read_01;
	char	Mode_Read_02;
	char	Mode_Read_03;
	char	Mode_Read_04;
	char	Mode_Read_05;
	char	Mode_Read_06;
	unsigned short	Motion_Err_Reg_01;
	unsigned short	Motion_Err_Reg_02;
	unsigned short	Motion_Err_Reg_03;
	unsigned short	Motion_Err_Reg_04;
	unsigned short	Motion_Err_Reg_05;
	unsigned short	Motion_Err_Reg_06;
	long	Velocity_Read_01;
	long	Velocity_Read_02;
	long	Velocity_Read_03;
	long	Velocity_Read_04;
	long	Velocity_Read_05;
	long	Velocity_Read_06;
} Task_ARM_CST_Inputs, *PTask_ARM_CST_Inputs;

typedef struct
{
	unsigned short	Control_Word_01;
	unsigned short	Control_Word_02;
	unsigned short	Control_Word_03;
	unsigned short	Control_Word_04;
	unsigned short	Control_Word_05;
	unsigned short	Control_Word_06;
	char	Mode_Write_01;
	char	Mode_Write_02;
	char	Mode_Write_03;
	char	Mode_Write_04;
	char	Mode_Write_05;
	char	Mode_Write_06;
	short	Torque_Set_01;
	short	Torque_Set_02;
	short	Torque_Set_03;
	short	Torque_Set_04;
	short	Torque_Set_05;
	short	Torque_Set_06;
	long	Position_Set_01;
	long	Position_Set_02;
	long	Position_Set_03;
	long	Position_Set_04;
	long	Position_Set_05;
	long	Position_Set_06;
	long	Velocity_Set_01;
	long	Velocity_Set_02;
	long	Velocity_Set_03;
	long	Velocity_Set_04;
	long	Velocity_Set_05;
	long	Velocity_Set_06;
} Task_ARM_CST_Outputs, *PTask_ARM_CST_Outputs;

#pragma pack(pop)
