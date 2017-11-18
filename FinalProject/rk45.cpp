#include "rk45.h"


integrator::integrator(double _tol) {
	this->tol = tol;
}

integrator::~integrator() {

}


//definition changed to "particle." IS THIS OK?
Particle  integrator::fg_rk45(std::vector<Particle> other_particles, Particle self_particle, double DT, double tol, bool error) //intended output is a double array with velocity and position
{
	const double g = 6.67408 * pow(10, -11); //gravitational constant G in m^3/(kg s^2)
	const int body_count = other_particles.size(); //calculates the effective number of bodies in system (not including self)
	double dist_mag_sq, a_mag, dt, cT, v_mag4, v_mag5, r_mag4, r_mag5, dV45, dR45;
	double a[3], vold[3], v5[3], v4[3], rold[3], r5[3], r4[3];
	double vkk[3], rkk[3];
	double vk_matrix[3][6] = {};
	double rk_matrix[3][6] = {};
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
	dt = DT/10;//time step (will vary)

	rold[0] = self_particle.pos.x;
	rold[1] = self_particle.pos.y;
	vold[0] = self_particle.vel.vx;
	vold[1] = self_particle.vel.vy;
		

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
				dist_mag_sq = pow(other_particles.at(iter).pos.x + vkk[0], 2) + pow(other_particles.at(iter).pos.y + vkk[1], 2); //magnitude of r squared
//mass_dist is an N by 2 array with the x,y displacement from self_particle to another particle. replace with "other_particles.pos.x and other....pos.y" 
				a_mag = g*other_particles.at(iter).mass / dist_mag_sq; //magnitude of force/mass on current body due to the ith other body
//LOOK AT OTHER_PARTICLES.AT(ITER). IS THERE A BETTER WAY TO MAKE THESE CALLS? CAN I LEAVE THE OTHER VECTORS AS ARRAYS?
				a[0] = a[0] + a_mag*(other_particles.at(iter).pos.x + vkk[0]) / sqrt(dist_mag_sq); // force/mass and direction calculation
				a[1] = a[1] + a_mag*(other_particles.at(iter).pos.y + vkk[1]) / sqrt(dist_mag_sq); // force/mass and direction calculation

			
			}

			//calculates new k values. loops over x,y,z coordinates
			for (int i = 0; i < 3; i++) {
				vk_matrix[i][kn] = dt*a[i];
				rk_matrix[i][kn] = dt*(vold[i] + rkk[i]);
			}
		}
		//end main integrator    =============================

		//udates velocity and position		
		for (int i = 0; i < 2; i++) {
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
			if (DT - cT < dt) dt = DT - cT; // if the new step size is larger than the remaining alloted time, then the step sized is decreased to the remaining alloted time
			for (int i = 0; i < 2; i++) {
				vold[i] = v5[i];
				rold[i] = r5[i];
			}
		}
		else
		{
			dt = dt*0.84*pow(tol / dR45, 0.25);
		}


	} while (cT < DT);
	
	//IS THERE ANY WAY TO SHORTEN THIS? 
	self_particle.pos.x = rold[0];
	self_particle.pos.y = rold[1];
	self_particle.vel.vx = vold[0];
	self_particle.vel.vy = vold[0];

	return self_particle;
}

