按下 forcesensoer botton(開完跑個一秒關掉)
按下 running botton

Traj(set point -> go)  -> Joint Car(save) -> Diagnosis botton

----------------------------------Diagnosis Trajectory---------------------------------------

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