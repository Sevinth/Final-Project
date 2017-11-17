#include "rk45.h"


integrator::integrator(double _tol) {
	this->tol = tol;
}

integrator::~integrator() {

}


//the following within comments is scratch:
std::vector<Particle> other_particles; //contains information of everybody EXECPT the one whose dynamics are being calculated
std::vector<Particle> self_particle; //
//end of scratch

double  integrator::fg_rk45(std::vector<Particle> other_particles, std::vector<Particle> self_particle, double DT, double tol, bool error) //intended output is a double array with velocity and position
{
	const double g = 6.67408 * pow(10, -11); //gravitational constant G in m^3/(kg s^2)
	const int body_count = other_masses.size(); //calculates the effective number of bodies in system (not including self)
	double dist_mag_sq, a_mag, dt, cT, DT, v_mag4, v_mag5, r_mag4, r_mag5, dV45, dR45;
	double a[3], vold[3], v5[3], v4[3], rold[3], r5[3], r4[3];
	double vkk[3], rkk[3];
	double vk_matrix[3][6] = {};
	double rk_matrix[3][6] = {};

	std::vector< vector<double> >  mass_dist(body_count, vector<int>(3));//establishes an N by 3 vector 

	const double k_ratio[6][6] =
	{
		{},
		{ 0.25 },
		{ 3 / 32, 9 / 32 },
		{ 1932 / 2197, -7200 / 2197, 7296 / 2197 },
		{ 439 / 216, -8, 3680 / 513, -845 / 4104 },
		{ -8 / 27, 2, -3544 / 2565, 1859 / 4104, -11 / 40 },
	};

	cT = 0; //current time
	DT = 1; //total time in seconds
	dt = 0.1;//time step (will vary)

	 //loop sets "old" position and velocity values
	for (int i = 0; i < 3; i++) {
		rold[i] = r[i];
		vold[i] = v[i];
	}

	//loops from t=0 to t=TT;
	do {
		//main integrator ; loops over the six values of k
		for (int kn = 0; kn < 6; kn++) {
			//loop resets the acceleration and k values.
			for (int i = 0; i < 3; i++) {
				a[i] = 0;
				vkk[i] = 0;
				rkk[i] = 0;
			}

			//loop creates k-sum values (loops x,y,z)
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 6; j++) {//(loops k1, k2,...k6)
					vkk[i] = vkk[i] + k_ratio[kn][j] * vk_matrix[i][j];
					rkk[i] = rkk[i] + k_ratio[kn][j] * rk_matrix[i][j];
				}
			}

			//loops over number of other bodies; calculates acceleration
			for (int iter = 0; iter < body_count; iter++) {
				dist_mag_sq = pow(mass_dist[iter][0] + vkk[0], 2) + pow(mass_dist[iter][1] + vkk[1], 2) + pow(mass_dist[iter][2] + vkk[2], 2); //magnitude of r squared

				a_mag = g*other_masses[iter] / dist_mag_sq; //magnitude of force/mass on current body due to the ith other body
				for (int i = 0; i < 3; i++) {//loops over x,y,z coordinates
					a[i] = a[i] + a_mag*(mass_dist[iter][i] + vkk[i]) / sqrt(dist_mag_sq); // force/mass and direction calculation
				}
			}

			//calculates new k values. loops over x,y,z coordinates
			for (int i = 0; i < 3; i++) {
				vk_matrix[i][kn] = dt*a[i];
				rk_matrix[i][kn] = dt*(v[i] + rkk[i]);
			}
		}
		//end main integrator    =============================

		//udates velocity and position		
		for (int i = 0; i < 3; i++) {
			v5[i] = vold[i] + (16 / 135)*vk_matrix[i][1] + (6656 / 12825)*vk_matrix[i][3] + (28561 / 56430)*vk_matrix[i][4] - (9 / 50)*vk_matrix[i][5] + (2 / 55)*vk_matrix[i][6];
			v4[i] = vold[i] + (25 / 216)*vk_matrix[i][1] + (1408 / 2565)*vk_matrix[i][3] + (2197 / 4101)*vk_matrix[i][4] - (1 / 5)*vk_matrix[i][5];
			r5[i] = rold[i] + (16 / 135)*rk_matrix[i][1] + (6656 / 12825)*rk_matrix[i][3] + (28561 / 56430)*rk_matrix[i][4] - (9 / 50)*rk_matrix[i][5] + (2 / 55)*rk_matrix[i][6];
			r4[i] = rold[i] + (25 / 216)*rk_matrix[i][1] + (1408 / 2565)*rk_matrix[i][3] + (2197 / 4101)*rk_matrix[i][4] - (1 / 5)*rk_matrix[i][5];
		}
		//calculates magnitude of velocity
		v_mag5 = sqrt(pow(v5[0], 2) + pow(v5[1], 2) + pow(v5[2], 2));
		v_mag4 = sqrt(pow(v4[0], 2) + pow(v4[1], 2) + pow(v4[2], 2));
		r_mag5 = sqrt(pow(r5[0], 2) + pow(r5[1], 2) + pow(r5[2], 2));
		r_mag4 = sqrt(pow(r4[0], 2) + pow(r4[1], 2) + pow(r4[2], 2));

		dV45 = (1 / dt)*abs(v_mag5 - v_mag4);
		dR45 = (1 / dt)*abs(r_mag5 - r_mag4);

		//if dR45 meets tolerance standards: time is updated; else the time step is lowered. the iteration is repeated. 
		if (dR45 < tol)
		{
			cT = cT + dt;
			dt = dt*0.84*pow(tol / dR45, 0.25);
			for (int i = 0; i < 3; i++) {
				vold[i] = v5[i];
				rold[i] = r5[i];
			}
		}
		else
		{
			dt = dt*0.84*pow(tol / dR45, 0.25);
		}


	} while (cT < DT);

	return 0;
}

