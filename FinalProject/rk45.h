#include "std.h"



class integrator {

public:

	//Overload
	integrator(double tol);
	~integrator();

	double fg_rk45(int body_count, double other_masses[], double mass_dist[][3], double self_mass, double r[], double v[], double tol, bool error);
	

private:
	const int body_count = 3;
	double other_masses[3];
	double mass_dist[3][3];
	double tol{0};
};