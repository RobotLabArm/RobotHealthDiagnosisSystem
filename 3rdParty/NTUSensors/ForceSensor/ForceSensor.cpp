#include "ForceSensor.h"
#include "force.h"

void ForceSensor::getForceTorqe(double* output)
{
	static double FM[6], FM2[6];	
	static double X[6] = {1, 1, 1, 1, 1, 1};
	static double P[6] = {1, 1, 1, 1, 1, 1};
	static double Q[6] = {1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4};
	static double R[6] = {1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4};
	static double KGain[6];
	static double F[6];
	static int A[6];

	  //瘋狂的傳送最快大概500 point
	  Forcemean(FM, FM2, 6, 6);
	  
	  for (int i = 0; i < 6; i++){
	  A[i] = (FM[i]+0.005)*100;
	  F[i] = (double)A[i]/100;
	  }

	  /*Klman Filter*/
	  for (int i = 0; i < 6; i++)
	  {
		  P[i] = P[i]+Q[i];
		  KGain[i] = P[i]/(P[i]+R[i]);
		  X[i] = X[i] + KGain[i]*(F[i]-X[i]);
		  P[i] = (1-KGain[i])*P[i];
	  }

	  for(int i = 0; i < 6; i++)
	  output[i]=X[i];
}





