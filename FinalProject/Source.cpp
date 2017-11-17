#include <mpi.h>
#include "QuadTree.h"
#include <random>
#include <vector>
#include <memory>
#include "rk45.h"


int main() {


	const double MAX_GRID_X = 10;
	const double MAX_GRID_Y = 10;
	const double MIN_GRID_X = 0;
	const double MIN_GRID_Y = 0;

	std::ofstream pointFile;
	pointFile.open("PointData.txt");

	std::random_device r;
	std::default_random_engine engine(r());
	//Works fine for a square grid, if grid is rectangular need to create a new distribution
	std::uniform_real_distribution<double> dist(5, 10);
	std::mt19937_64 e2;
	//Setup initial quad tree.
	QuadTree center(Point2D(0, 0), Point2D(0, 0));
	std::vector<std::shared_ptr<Node>> nodeVec;

	nodeVec.resize(10);

	std::vector<Particle> myParticles;
	myParticles.resize(10);
	std::cout << "Particles Created: " << std::endl;
	for (size_t i = 0; i < myParticles.size(); ++i) {
	
		myParticles[i].pos.x = dist(engine);
		myParticles[i].pos.y = dist(engine);

		pointFile << myParticles[i].pos.x << "," << myParticles[i].pos.y << std::endl;
	}


	Point2D upperLeftGridPoint(MIN_GRID_X, MIN_GRID_Y);
	Point2D bottomRightGridPoint(MAX_GRID_X, MAX_GRID_Y);


	Node myNode(upperLeftGridPoint, bottomRightGridPoint, myParticles);

	for (int i = 0; i < myNode.childNodes.size(); ++i) {
		std::cout << "Children? " << myNode.childNodes[i]->hasChildren << std::endl;
		std::cout << "Particle? " << myNode.childNodes[i]->hasParticle << std::endl;
		std::cout << "Number of Particles: " << myNode.localParticles.size() << std::endl;
		std::cout << "Rank: " << myNode.childNodes[i]->rank << std::endl;
		std::cout << "Center of Mass: " << myNode.childNodes[i]->centMass.x << " " << myNode.childNodes[i]->centMass.y << std::endl;
	}


#ifdef _WIN32
	system("PAUSE");
#endif



	integrator myIntegrator(0.1);


	return EXIT_SUCCESS;
}

