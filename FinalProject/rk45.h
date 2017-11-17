#include "std.h"



class integrator {

public:

	//Overload
	integrator(double tol);
	~integrator();

	Particle fg_rk45(std::vector<Particle> other_particles, Particle self_particle, double DT, double tol, bool error);
	

private:
	const int body_count = 3;
	double other_masses[3];
	double mass_dist[3][3];
	double tol{0};
};