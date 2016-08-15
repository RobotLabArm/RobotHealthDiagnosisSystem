#ifndef _Trajectoryplanning_
#define _Trajectoryplanning_


#include <iostream>
#include <Eigen/Eigen/Dense>
#include <vector>
#include <map>
using namespace std;
//using namespace Eigen;
//namespace eigenTraj{

	class TrajectoryPlanning
	{
	public:
		// 212
		Eigen::MatrixXd Splines212(Eigen::VectorXd q0, Eigen::VectorXd q0d,Eigen::VectorXd qf,  Eigen::VectorXd qfd,  Eigen::MatrixXd qm, double t_total, int select);
		Eigen::MatrixXd Splines212(Eigen::VectorXd q0, Eigen::VectorXd q0d,Eigen::VectorXd qf,  Eigen::VectorXd qfd, double t_total, int select);
		//// 434
		Eigen::MatrixXd Splines434( Eigen::VectorXd q0,  Eigen::VectorXd q0d,  Eigen::VectorXd q0dd,  Eigen::VectorXd qf,  Eigen::VectorXd qfd,  Eigen::VectorXd qfdd,  Eigen::MatrixXd qm, double t_total, int select);
		Eigen::MatrixXd Splines434( Eigen::VectorXd q0,  Eigen::VectorXd q0d,  Eigen::VectorXd q0dd,  Eigen::VectorXd qf,  Eigen::VectorXd qfd,  Eigen::VectorXd qfdd, double t_total, int select);
		//// 535
		Eigen::MatrixXd Splines535( Eigen::VectorXd q0,  Eigen::VectorXd q0d,  Eigen::VectorXd q0dd,  Eigen::VectorXd q0ddd,  Eigen::VectorXd qf,  Eigen::VectorXd qfd,  Eigen::VectorXd qfdd,  Eigen::VectorXd qfddd,  Eigen::MatrixXd qm, double t_total, int select);
		Eigen::MatrixXd Splines535( Eigen::VectorXd q0,  Eigen::VectorXd q0d,  Eigen::VectorXd q0dd,  Eigen::VectorXd q0ddd,  Eigen::VectorXd qf,  Eigen::VectorXd qfd,  Eigen::VectorXd qfdd,  Eigen::VectorXd qfddd, double t_total, int select);
		//// Cubic
		Eigen::MatrixXd CubicPolynomials( Eigen::VectorXd q0,  Eigen::VectorXd q0d,  Eigen::VectorXd qf,  Eigen::VectorXd qfd,  Eigen::MatrixXd qm, double t_total, int select);
		Eigen::MatrixXd CubicPolynomials( Eigen::VectorXd q0,  Eigen::VectorXd q0d,  Eigen::VectorXd qf,  Eigen::VectorXd qfd, double t_total, int select);

	};

//};

#endif






