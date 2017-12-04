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
	double a[3], vold[3], v5[3], v4[3], rold[3], r5[3], r4[3],vk3[3],rk3[3];
	double vk_add[3], rk_add[3],vk1[3],vk2[3],vk4[3],vk5[3],vk6[3], rk1[3], rk2[3], rk4[3], rk5[3], rk6[3];


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



	//loops from t=0 to t=DT;
	do {
		a[0] = 0;  a[1] = 0;
		for (int body = 0; body < body_count; body++) {//loops over number of bodies
			x_displacement = other_particles[body].pos.x - self_particle.pos.x;
			y_displacement = other_particles[body].pos.y - self_particle.pos.y;
			dist_mag = sqrt(pow(x_displacement, 2) + pow(y_displacement, 2)); //magnitude of r
			dist_mag_cu = pow(dist_mag, 3); //r cubed
			a[0] = a[0] + (x_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
			a[1] = a[1] + (y_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
		}
		vk1[0] = a[0] * dt; vk1[1] = a[1] * dt;
		rk1[0] = vold[0] * dt; rk1[1] = vold[1] * dt;
		//std::cout << std::endl;
		//std::cout << " ax= " << a[0] << " ay=" << a[1]<<" vk1="<<vk1[0]<<" vk="<<vk1[1];
		//std::cout << " rk1= " << rk1[0] << " ry=" << rk1[1];
		//std::cout << std::endl;


		//start of k2 sequence
		vk_add[0] = vk1[0]/4.0; rk_add[0] = rk1[0] / 4.0;
		vk_add[1] = vk1[1] / 4.0; rk_add[1] = rk1[1] / 4.0;
		a[0] = 0; a[1] = 0;
		for (int body = 0; body < body_count; body++) {//loops over number of bodies
			x_displacement = other_particles[body].pos.x - (self_particle.pos.x + rk_add[0]);
			y_displacement = other_particles[body].pos.y - (self_particle.pos.y + rk_add[1]);
			dist_mag = sqrt(pow(x_displacement, 2) + pow(y_displacement, 2)); //magnitude of r
			dist_mag_cu = pow(dist_mag, 3); //r cubed
			a[0] = a[0] + (x_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
			a[1] = a[1] + (y_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
		}
		vk2[0] = a[0] * dt; vk2[1] = a[1] * dt;
		rk2[0] = (vold[0] + vk_add[0])*dt; rk2[1] = (vold[1] + vk_add[1])*dt;

		//std::cout << " ax= " << a[0] << " ay=" << a[1] << " vk2=" << vk2[0] << " vk2=" << vk2[1];
		//std::cout << " rk2= " << rk2[0] << " ry=" << rk2[1];
		//std::cout << std::endl;

		//start of k3 sequence
		vk_add[0] = 3.0 / 32.0 + vk1[0] + 9.0 / 32.0*vk2[0];
		rk_add[0] = 3.0 / 32.0 + rk1[0] + 9.0 / 32.0*rk2[0];
		vk_add[1] = 3.0 / 32.0 + vk1[1] + 9.0 / 32.0*vk2[1];
		rk_add[1] = 3.0 / 32.0 + rk1[1] + 9.0 / 32.0*rk2[1];
		a[0] = 0; a[1] = 0;
		for (int body = 0; body < body_count; body++) {//loops over number of bodies
			x_displacement = other_particles[body].pos.x - (self_particle.pos.x + rk_add[0]);
			y_displacement = other_particles[body].pos.y - (self_particle.pos.y + rk_add[1]);
			dist_mag = sqrt(pow(x_displacement, 2) + pow(y_displacement, 2)); //magnitude of r
			dist_mag_cu = pow(dist_mag, 3); //r cubed
			a[0] = a[0] + (x_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
			a[1] = a[1] + (y_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
		}
		vk3[0] = a[0] * dt; vk3[1] = a[1] * dt;
		rk3[0] = (vold[0] + vk_add[0])*dt; rk3[1] = (vold[1] + vk_add[0])*dt;
		//std::cout << " ax= " << a[0] << " ay=" << a[1] << " vk3=" << vk3[0] << " vk3=" << vk3[1];
		//std::cout << " rkx3= " << rk3[0] << " rky3=" << rk3[1];
		//std::cout << std::endl;


		//start of k4 sequence
		vk_add[0] = 1932.0 / 2197.0*vk1[0] - 7200.0 / 2197.0*vk2[0] + 7296.0 / 2197.0*vk3[0];
		rk_add[0] = 1932.0 / 2197.0*rk1[0] - 7200.0 / 2197.0*rk2[0] + 7296.0 / 2197.0*rk3[0];
		vk_add[1] = 1932.0 / 2197.0*vk1[1] - 7200.0 / 2197.0*vk2[1] + 7296.0 / 2197.0*vk3[1];
		rk_add[1] = 1932.0 / 2197.0*rk1[1] - 7200.0 / 2197.0*rk2[1] + 7296.0 / 2197.0*rk3[1];
		a[0] = 0; a[1] = 0;
		for (int body = 0; body < body_count; body++) {//loops over number of bodies
			x_displacement = other_particles[body].pos.x - (self_particle.pos.x + rk_add[0]);
			y_displacement = other_particles[body].pos.y - (self_particle.pos.y + rk_add[1]);
			dist_mag = sqrt(pow(x_displacement, 2) + pow(y_displacement, 2)); //magnitude of r
			dist_mag_cu = pow(dist_mag, 3); //r cubed
			a[0] = a[0] + (x_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
			a[1] = a[1] + (y_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
		}
		vk4[0] = a[0] * dt; vk4[1] = a[1] * dt;
		rk4[0] = (vold[0] + vk_add[0])*dt; rk4[1] = (vold[1] + vk_add[0])*dt;

		//std::cout << " ax= " << a[0] << " ay=" << a[1] << " vk4=" << vk4[0] << " vk4=" << vk4[1];
		//std::cout << " rkx4= " << rk4[0] << " rky4=" << rk4[1];
		//std::cout << std::endl;

		//start of k5 sequence
		vk_add[0] = 439.0 / 216.0*vk1[0] - 8.0*vk2[0] +3680.0 / 513.0*vk3[0] - 845.0 / 4104.0*vk4[0];
		rk_add[0] = 439.0 / 216.0*rk1[0] - 8.0*rk2[0] +3680.0 / 513.0*rk3[0] - 845.0 / 4104.0*rk4[0];
		vk_add[1] = 439.0 / 216.0*vk1[1] - 8.0*vk2[1] + 3680.0 / 513.0*vk3[1] - 845.0 / 4104.0*vk4[1];
		rk_add[1] = 439.0 / 216.0*rk1[1] - 8.0*rk2[1] + 3680.0 / 513.0*rk3[1] - 845.0 / 4104.0*rk4[1];
		a[0] = 0; a[1] = 0;
		for (int body = 0; body < body_count; body++) {//loops over number of bodies
			x_displacement = other_particles[body].pos.x - (self_particle.pos.x + rk_add[0]);
			y_displacement = other_particles[body].pos.y - (self_particle.pos.y + rk_add[1]);
			dist_mag = sqrt(pow(x_displacement, 2) + pow(y_displacement, 2)); //magnitude of r
			dist_mag_cu = pow(dist_mag, 3); //r cubed
			a[0] = a[0] + (x_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
			a[1] = a[1] + (y_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
		}
		vk5[0] = a[0] * dt; vk5[1] = a[1] * dt;
		rk5[0] = (vold[0] + vk_add[0])*dt; rk5[1] = (vold[1] + vk_add[1])*dt;

		//std::cout << " ax= " << a[0] << " ay=" << a[1] << " vk5=" << vk5[0] << " vk5=" << vk5[1];
		//std::cout << " rkx5= " << rk5[0] << " rky5=" << rk5[1];
		//std::cout << std::endl;

		//start of k6 sequence
		vk_add[0] = (8.0 / 27.0*vk1[0] + 2.0*vk2[0] - 3544.0 / 2565.0*vk3[0] + 1859.0 / 4104.0*vk4[0] - 11.0 / 40.0*vk5[0]);
		rk_add[0] = (8.0 / 27.0*rk1[0] + 2.0*rk2[0] - 3544.0 / 2565.0*rk3[0] + 1859.0 / 4104.0*rk4[0] - 11.0 / 40.0*rk5[0]);
		vk_add[1] = (8.0 / 27.0*vk1[1] + 2.0*vk2[1] - 3544.0 / 2565.0*vk3[1] + 1859.0 / 4104.0*vk4[1] - 11.0 / 40.0*vk5[1]);
		rk_add[1] = (8.0 / 27.0*rk1[1] + 2.0*rk2[1] - 3544.0 / 2565.0*rk3[1] + 1859.0 / 4104.0*rk4[1] - 11.0 / 40.0*rk5[1]);
		a[0] = 0; a[1] = 0;
		for (int body = 0; body < body_count; body++) {//loops over number of bodies
			x_displacement = other_particles[body].pos.x - (self_particle.pos.x + rk_add[0]);
			y_displacement = other_particles[body].pos.y - (self_particle.pos.y + rk_add[1]);
			dist_mag = sqrt(pow(x_displacement, 2) + pow(y_displacement, 2)); //magnitude of r
			dist_mag_cu = pow(dist_mag, 3); //r cubed
			a[0] = a[0] + (x_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
			a[1] = a[1] + (y_displacement / dist_mag_cu)* G*other_particles[body].mass; //force/mass on current body due to the ith other body
		}
		vk6[0] = a[0] * dt; vk6[1] = a[1] * dt;
		rk6[0] = (vold[0] + vk_add[0])*dt; rk6[1] = (vold[1] + vk_add[1])*dt;
		
		//std::cout << " ax= " << a[0] << " ay=" << a[1] << " vk6=" << vk6[0] << " vk6=" << vk6[1];
		//std::cout << " rkx6= " << rk6[0] << " rky6=" << rk6[1];
		//std::cout << std::endl;

		//updates velocity and position		
		for (int i = 0; i < 2; i++) {
			v5[i] = vold[i] + (16.0/135.0)*vk1[i] + (6656.0/12825.0)*vk3[i] + (28561.0/56430.0)*vk4[i] - (9.0/50.0)*vk5[i] + (2.0/55.0)*vk6[i];
			r5[i] = rold[i] + (16.0/135.0)*rk1[i] + (6656.0/12825.0)*rk3[i] + (28561.0/56430.0)*rk4[i] - (9.0/50.0)*rk5[i] + (2.0/55.0)*rk6[i];

			v4[i] = vold[i] + (25.0 / 216.0)*vk1[i] + (1408.0 / 2565.0)*vk3[i] + (2197.0 / 4101.0)*vk4[i] - (1.0 / 5.0)*vk5[i];
			r4[i] = rold[i] + (25.0 / 216.0)*rk1[i] + (1408.0 / 2565.0)*rk3[i] + (2197.0 / 4101.0)*rk4[i] - (1.0 / 5.0)*rk5[i];

		}
		//calculates magnitude of velocity
		v_mag5 = sqrt(pow(v5[0], 2) + pow(v5[1], 2));// +pow(v5[2], 2));
		v_mag4 = sqrt(pow(v4[0], 2) + pow(v4[1], 2));// + pow(v4[2], 2));
		r_mag5 = sqrt(pow(r5[0], 2) + pow(r5[1], 2));// + pow(r5[2], 2));
		r_mag4 = sqrt(pow(r4[0], 2) + pow(r4[1], 2));// + pow(r4[2], 2));


		dV45 = abs(v_mag5 - v_mag4);
		dR45 = abs(r_mag5 - r_mag4);
		ERR = dR45;
		//std::cout << " vx5= " << v5[0] << " vy5=" << v5[1] << " vx4= " << v4[0] << " vy4=" << v4[1];
		//std::cout << " V ERROR= " << dV45 << " R ERROR" << dR45;
		//std::cout << std::endl;



		//if dR45 meets tolerance standards: time is updated; else the time step is lowered. the iteration is repeated. 
		if (ERR <= dt*tol)
		{
			cT = cT + dt;
			dt = dt*0.8400*pow((dt*tol / dR45), 0.25);
			//if (DT - cT < dt) {
			//dt = DT - cT; // if the new step size is larger than the remaining alloted time, then the step sized is decreased to the remaining alloted time
			//std::cout << "passed; t=" << cT << " setting dt=" << dt << " DT=" << DT;
			//std::cout << std::endl;
			//}
			for (int i = 0; i < 2; i++) {
				vold[i] = v4[i];
				rold[i] = r4[i];
			}
		}
		else if (ERR>dt*tol)
		{
			dt = dt*0.8400*pow((dt*tol / dR45), 0.25);
			//std::cout << "Error Exceeded at t=" << cT << " setting dt=" << dt;
			//std::cout << std::endl; 
		}
		else {// when ERR=0
			cT = cT + dt;
		}
		//std::cout << "end of loop. to begining. t=" << cT << " dt=" << dt;
		//std::cout << std::endl;

	} while (cT < DT);
	
	self_particle.pos.x = r5[0];
	self_particle.pos.y = r5[1];
	self_particle.vel.vx = v5[0];
	self_particle.vel.vy = v5[1];



	return self_particle;
}

