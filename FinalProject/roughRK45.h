#include "rk45.h"


integrator::integrator(double _tol) {
	this->tol = tol;
}

integrator::~integrator() {

}


Particle  integrator::fg_rk45(std::vector<Particle> other_particles, Particle self_particle, double DT, double tol, bool error) //intended output is a double array with velocity and position
{
	const double G = 6.67408 * pow(10, -11); //gravitational constant G in m^3/(kg s^2)
	const int body_count = other_particles.size(); //calculates the effective number of bodies in system (not including self)
	double dist_mag, dist_mag_cu, a_mag, dt, cT, v_mag4, v_mag5, r_mag4, r_mag5, dV45, dR45, ERR;
	double x_displacement, y_displacement;
	double a[3], vold[3], v5[3], v4[3], rold[3], r5[3], r4[3];
	double vk_add[3], rk_add[3];
	double vk_matrix[3][6] = {};
	double rk_matrix[3][6] = {};
	const double k_ratio[6][6] =
	{
		{},
		{ 0.25 },
		{ 3.0 / 32.0, 9.0 / 32.0 },
		{ 1932.0 / 2197.0, -7200.0 / 2197.0, 7296.0 / 2197.0 },
		{ 439.0 / 216.0, -8.0, 3680.0 / 513.0, -845.0 / 4104.0 },
		{ -8.0 / 27.0, 2.0, -3544.0 / 2565.0, 1859.0 / 4104.0, -11.0 / 40.0 },
	};

	cT = 0; //current time
	dt = DT/100.0;//time step (will vary)

	rold[0] = self_particle.pos.x;
	rold[1] = self_particle.pos.y;
	vold[0] = self_particle.vel.vx;
	vold[1] = self_particle.vel.vy;
	rold[2] = 0.0;
	vold[2] = 0.0;
	v5[2] = 0.0;
	v4[2] = 0.0;

				for (int i = 0; i < 3; i++) {
				a[i] = 0.0;
				vk_add[i] = 0.0;
				rk_add[i] = 0.0;
			}
	
	

	//loops from t=0 to t=DT;
	do {
		//std::cout << "cT=" << cT << " dt=" << dt;
		//std::cout << std::endl;
		//main integrator ; loops over the six values of kn to build k1, k2, k3, k4, k5, and k6
			//loop resets the acceleration and k values.


			//loop creates k-sum values (loops x,y,z)
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 6; j++) {//(loops k1, k2,...k6)
					vk_add[i] = vk_add[i] + k_ratio[kn][j] * vk_matrix[i][j];
					rk_add[i] = rk_add[i] + k_ratio[kn][j] * rk_matrix[i][j];
				}
			}
			std::cout << " -------acceleration calculation---------";
			std::cout << std::endl;
			//loops over number of other bodies; calculates acceleration
			for (int iter = 0; iter < body_count; iter++) {
				x_displacement = other_particles[iter].pos.x - self_particle.pos.x + rk_add[0];
				y_displacement = other_particles[iter].pos.y - self_particle.pos.y + rk_add[1];

				dist_mag = sqrt(pow(x_displacement, 2) + pow(y_displacement, 2)); //magnitude of r
				dist_mag_cu = pow(dist_mag,3); //magnitude of r

				a[0] =a[0]+(x_displacement/ dist_mag_cu)* G*other_particles[iter].mass; //force/mass on current body due to the ith other body
				a[1] = a[1] + (y_displacement/ dist_mag_cu)* G*other_particles[iter].mass; //force/mass on current body due to the ith other body
				std::cout << "kn=" << kn << " body=" << iter << " x_disp=" << x_displacement << " y_disp=" << y_displacement;
				std::cout << " r=" << dist_mag << " r cubed=" << dist_mag_cu << " ax=" << a[0] << " ay=" << a[1];
				std::cout << std::endl;
			
			}
			std::cout << " --------k matrix updater--------";
			std::cout << std::endl;
			//calculates new k values. loops over x,y,z coordinates
			for (int i = 0; i < 2; i++) {
				vk_matrix[i][kn] = dt*a[i];
				rk_matrix[i][kn] = dt*(vold[i] + rk_add[i]);
				std::cout << "kn=" << kn << " dimension=" << i << " vk_m=" << dt*a[i] << " rk_m=" << dt*(vold[i] + vk_add[i]);
				std::cout << std::endl;
			}
		
		//end main integrator    =============================
		std::cout << "----";
		std::cout << std::endl;
		std::cout << "--!!--------velocity and postion updaters--------!!--";
		std::cout << std::endl;
		std::cout << "----";
		std::cout << std::endl;

		//udates velocity and position		
		for (int i = 0; i < 2; i++) {
			v5[i] = vold[i] + (16.0 / 135.0)*vk_matrix[i][1] + (6656.0 / 12825.0)*vk_matrix[i][3] + (28561.0 / 56430.0)*vk_matrix[i][4] - (9.0 / 50.0)*vk_matrix[i][5] + (2.0 / 55.0)*vk_matrix[i][6];
			r5[i] = rold[i] + (16.0 / 135.0)*rk_matrix[i][1] + (6656.0 / 12825.0)*rk_matrix[i][3] + (28561.0 / 56430.0)*rk_matrix[i][4] - (9.0 / 50.0)*rk_matrix[i][5] + (2.0 / 55.0)*rk_matrix[i][6];
			v4[i] = vold[i] + (25.0 / 216.0)*vk_matrix[i][1] + (1408.0 / 2565.0)*vk_matrix[i][3] + (2197.0 / 4101.0)*vk_matrix[i][4] - (1.0 / 5.0)*vk_matrix[i][5];
			r4[i] = rold[i] + (25.0 / 216.0)*rk_matrix[i][1] + (1408.0 / 2565.0)*rk_matrix[i][3] + (2197.0 / 4101.0)*rk_matrix[i][4] - (1.0 / 5.0)*rk_matrix[i][5];
		}
		//calculates magnitude of velocity
		v_mag5 = sqrt(pow(v5[0], 2) + pow(v5[1], 2));// +pow(v5[2], 2));
		v_mag4 = sqrt(pow(v4[0], 2) + pow(v4[1], 2));// + pow(v4[2], 2));
		r_mag5 = sqrt(pow(r5[0], 2) + pow(r5[1], 2));// + pow(r5[2], 2));
		r_mag4 = sqrt(pow(r4[0], 2) + pow(r4[1], 2));// + pow(r4[2], 2));


		dV45 = (1.0 / dt)*abs(v_mag5 - v_mag4);
		dR45 = (1.0 / dt)*abs(r_mag5 - r_mag4);
		
		std::cout << "v5x=" << v5[0] << " v5y=" << v5[1] << " v_mag5=" << v_mag5<< " v_mag4=" << v_mag4<<" ERROR="<<dV45;
		std::cout << std::endl;
		std::cout << "v4x=" << v4[0] << " v4y=" << v4[1] << " r_mag5=" << r_mag5 << " r_mag4=" << r_mag4 << " ERROR="<<dR45;
		std::cout << std::endl;
		if (dV45 >= dR45) {
			ERR = dV45;
		}
		else if (dV45 < dR45) {
			ERR = dR45;
		} 

		//if dR45 meets tolerance standards: time is updated; else the time step is lowered. the iteration is repeated. 
		if (ERR <= tol)
		{
			cT = cT + dt;
			dt = dt*0.8400*pow((tol / dR45), 0.25);
			//if (DT - cT < dt) {
				//dt = DT - cT; // if the new step size is larger than the remaining alloted time, then the step sized is decreased to the remaining alloted time
			//std::cout << "passed; t=" << cT << " setting dt=" << dt << " DT=" << DT;
			//std::cout << std::endl;
			//}
			for (int i = 0; i < 2; i++) {
				vold[i] = v5[i];
				rold[i] = r5[i];
			}
		}
		else if(ERR>tol)
		{
			dt = dt*0.8400*pow((tol / dR45), 0.25);
			//std::cout << "Error Exceeded at t=" << cT << " setting dt=" << dt;
			//std::cout << std::endl; 
		}
		else {// when ERR=0
			cT = cT + dt;
		}
		//std::cout << "end of loop. to begining. t=" << cT << " dt=" << dt;
		//std::cout << std::endl;

	} while (cT < DT);
	
	//IS THERE ANY WAY TO SHORTEN THIS? 
	self_particle.pos.x = r5[0];
	self_particle.pos.y = r5[1];
	self_particle.vel.vx = v5[0];
	self_particle.vel.vy = v5[1];

	//self_particle.pos.x = a[0];
	//self_particle.pos.y = a[1];
	//self_particle.vel.vx = a[0];
	//self_particle.vel.vy = a[1];

	return self_particle;
}

