
#include <iostream>
#include "QuadTree.h"
#include <random>
#include <fstream>
#include <vector>
#include <memory>
#include "rk45.h"
#include <string>


int main() {
	std::ofstream outFile;
	const int N = 3; // number of particles
	std::vector<Particle> all_Particles; // list of all particles
	std::vector<Particle> all_Part_New; // list of all particles 
	std::vector<Particle> other_Particles; //list of all particles minus main particle
	all_Particles.resize(N);
	all_Part_New.resize(N);
	other_Particles.resize(N - 1);
	Particle main_particle; //particle whose dynamics are under consideration
	const double DT = 0.01; //time step of rk45
	double tolerance = 1; // error tolerance for rk45
	const bool yes = true; //useless variable right now
	integrator rk(tolerance);

	outFile.open("particle_position_test.txt");

	const double pi = atan(1)*4.0;
	const double delta_angle = 2 * pi / N;
	const double start_radius = 1.5*pow(10, 9);
	const double start_velocity = 3.0*pow(10, 1);

	const double TT = 5400.0; // total time
	double current_time = 0.0;
	int iter;

	std::cout << std::endl;
	all_Particles[1].pos.x = 0.0;
	all_Particles[1].pos.y = 0.0;
	all_Particles[1].vel.vx = 0.0;
	all_Particles[1].mass = 5.97*pow(10, 24); // mass of earth in kg
	all_Particles[0].pos.x = 6783.0e3;
	all_Particles[0].pos.y = 0.0;
	all_Particles[0].vel.vx = 0.0;
	all_Particles[0].vel.vy = 7.67e3;
	all_Particles[0].mass = 420000.0; // mass of iss in kg
	all_Particles[2].pos.x = 385000.0e3;
	all_Particles[2].pos.y = 0.0;
	all_Particles[2].vel.vx = 0.0;
	all_Particles[2].vel.vy = 1.022e3;
	all_Particles[2].mass = 7.35*pow(10, 22); // mass of moon in kg


											  //for-loop establishes initial conditions
											  //for (int i = 0; i < N;i++) { 
											  //all_Particles[i].pos.x = cos(i*delta_angle)*start_radius;
											  //all_Particles[i].pos.y = sin(i*delta_angle)*start_radius;
											  //all_Particles[i].vel.vx = sin(i*delta_angle)*start_velocity;
											  //all_Particles[i].vel.vy = -cos(i*delta_angle)*start_velocity;
											  //all_Particles[i].mass = (0.7 + i / 10.0) * 2 * pow(10, 30); // mass of sun in kg
											  //}

	std::cout << "Particle 1 Pos: " << all_Particles[0].pos.x << "  " << all_Particles[0].pos.y;
	std::cout << " Particle 1 Vel: " << all_Particles[0].vel.vx << "  " << all_Particles[0].vel.vy;
	std::cout << std::endl;
	std::cout << "Particle Masses: " << all_Particles[0].mass << " " << all_Particles[1].mass << " " << all_Particles[2].mass;
	std::cout << std::endl;
	// main integration
	while (current_time<TT) {
		// for loop runs fg_rk45 for each particle in system
		for (int i = 0; i < N; i++) {
			main_particle = all_Particles[i];
			iter = 0;
			// for loop creates the other_Particles vector
			for (int j = 0; j < N; j++) {
				if (j != i) {
					other_Particles[iter] = all_Particles[j];
					iter = iter + 1;
				}
			}
			all_Part_New[i] = rk.fg_rk45(other_Particles, main_particle, DT, tolerance, yes);
		}
		//for loop updates the new positions and velocities for each particle
		for (int i = 0; i < N; i++) {
			// add a matrix to store a time history of particles
			all_Particles[i] = all_Part_New[i];
		}
		outFile << "0  " << all_Particles[0].pos.x << "  " << all_Particles[0].pos.y << "  1  " << all_Particles[1].pos.x << "  " << all_Particles[1].pos.y << " 2  " << all_Particles[2].pos.x << "  " << all_Particles[2].pos.y << std::endl;;
		//outFile << "1  " << all_Particles[1].vel.vx << "  " << all_Particles[1].vel.vy << std::endl;;
		current_time = current_time + DT;
	}


	// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

	//const double MAX_GRID_X = 10;
	//const double MAX_GRID_Y = 10;
	//const double MIN_GRID_X = 0;
	//const double MIN_GRID_Y = 0;

	//std::ofstream pointFile;
	//pointFile.open("PointData.txt");

	//std::random_device r;
	//std::default_random_engine engine(r());
	////Works fine for a square grid, if grid is rectangular need to create a new distribution
	//std::uniform_real_distribution<double> x_dist(0, 10);
	//std::uniform_real_distribution<double> y_dist(0, 10);
	//std::mt19937_64 e2;
	////Setup initial quad tree.
	//QuadTree center(Point2D(0, 0), Point2D(0, 0));
	//std::vector<std::shared_ptr<Node>> nodeVec;

	//nodeVec.resize(10);

	//std::vector<Particle> myParticles;
	//myParticles.resize(100);
	//std::cout << "Particles Created: " << std::endl;
	//for (size_t i = 0; i < myParticles.size(); ++i) {
	//
	//	myParticles[i].pos.x = x_dist(engine);
	//	myParticles[i].pos.y = y_dist(engine);

	//	pointFile << myParticles[i].pos.x << "," << myParticles[i].pos.y << std::endl;
	//}


	//Point2D upperLeftGridPoint(MIN_GRID_X, MIN_GRID_Y);
	//Point2D bottomRightGridPoint(MAX_GRID_X, MAX_GRID_Y);

	//QuadTree myTree(upperLeftGridPoint, bottomRightGridPoint, myParticles);

	//std::cout << "Total Children: " << myTree.rootNode->GlobalChildren.size() <<std::endl;
	//int nPoints = 0;
	//for (int i = 0; i < myTree.rootNode->GlobalChildren.size(); i++) {
	//	if (myTree.rootNode->GlobalChildren[i]->hasParticle == true) {
	//		nPoints++;
	//	}
	//}


	//std::cout << "Number of children, maybe: " << nPoints << std::endl;

#ifdef _WIN32
	system("PAUSE");
#endif


	return EXIT_SUCCESS;
}

