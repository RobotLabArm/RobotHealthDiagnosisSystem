#ifndef __PROJECTPLAY_H
#define __PROJECTPLAY_H

#pragma  once
#include "global_value.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <Eigen/Eigen/Dense>
// ------------  robot library ----------
#include "roboop/robot.h"
#include "roboop/controller.h"
#include "roboop/control_select.h"
#include "roboop/trajectory.h"
#include "roboop/utils.h"
#include "roboop/quaternion.h"
#include "roboop/dynamics_sim.h"
#include "roboop/IOfile.h"
// --------  Controller ----------
#include "serial_port.h"
#include "TwinCAT_COM.h"
// -------  Trajectory Planning -------------
#include "NTURlib/TrajecotryPlanning/TrajectoryPlanning.h"
// -------  ForceSensor -------------
#include "NTUSensors/ForceSensor/ForceSensor.h"
////////////////////////////////////////////
#include "CMail.h"

// Global value
extern Robot robot_atom;   //  Robot Model 
extern Robot robot_NTU8DOF;
class ProjectPlay
{
public:
	ProjectPlay();
	~ProjectPlay();
	// Safety Function
	void Atom_Reset();
	static bool gAtom_RestFinish;
	static int MotorControlSwitch;
	void Project_SaveFile();
	void Project_test();
	// Dynamics test fcn
	void AtomDynControl();
	double *Atom_P;
	void project_InitAtom();
	void project_AtomStep(int num);
	void project_AtomOffset(int num);
	void project_ATomThreadClose();
	static void project_AtomCartStep(int num);
	//  NTU 8 DOF robot
	void ProjectPlay_InitNTU8DOF();
public:
	// ---  Controller ---
	void ControllerConnect();
	void ControllerDisconnect();
	//extern 	SerialPort Sp1,Sp2,Sp3,Sp4,Sp5,Sp6;
public:
	//--- dynamics Test ---
	void Atom_dynamics_Test();
	void Atom_torqueCtl();
	void Atom_Impedance();
	// --- Trajectory ---
public:
	ReturnMatrix robot_jTray(ColumnVector q0, ColumnVector qf, ColumnVector time, int Selec);
	ReturnMatrix robot_straight(ColumnVector q0,ColumnVector qf,Robot &_robot, float times,int Selec);
	void foo(Robot foo_robot,map<float,ColumnVector>& m1, Matrix T0,Matrix Tf,ColumnVector q0fix);
	void Atom_TrajectoryGO();
	void Atom_Setpoint(float thetaAll[6],float times,int types);
	void Atom_TrajClear();

public:
	void FileIO(vector<vector<float>> OutData , const string & filename_);
	void FileIO(vector<float> OutData , const string & filename_);
public:   
	// Gripper
	void Grip_Command(int a);  //  a = 1 (close), 0 (open)
	void GripperConnectOrDisConnect(int types);   // types = 1 (connect), 0 (disconnect)
public:	
	// ForceSensor
	void project_InitForceSensor();
	void project_ForceSensorThreadClose();
	// Monitoring
	static bool Monitoring(const vector<float>& _tempFeedbackTorque, const vector<float>& _tempFeedbackForce, float &output);
	static bool Monitoring(const vector<float>& _tempFeedbackq, const vector<float>& _tempFeedbackqd, const vector<float>& _tempFeedbackTorque, const vector<float>& _tempFeedbackForce, float &output);
	static float Monitoring_std;
};

void loadTxt(const string& filename, Eigen::MatrixXf& mat);
#endif
