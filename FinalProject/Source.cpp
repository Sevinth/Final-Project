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
	std::uniform_real_distribution<double> x_dist(0, 10);
	std::uniform_real_distribution<double> y_dist(0, 10);
	std::mt19937_64 e2;
	//Setup initial quad tree.
	QuadTree center(Point2D(0, 0), Point2D(0, 0));
	std::vector<std::shared_ptr<Node>> nodeVec;

	nodeVec.resize(10);

	std::vector<Particle> myParticles;
	myParticles.resize(100);
	std::cout << "Particles Created: " << std::endl;
	for (size_t i = 0; i < myParticles.size(); ++i) {
	
		myParticles[i].pos.x = x_dist(engine);
		myParticles[i].pos.y = y_dist(engine);

		pointFile << myParticles[i].pos.x << "," << myParticles[i].pos.y << std::endl;
	}


	Point2D upperLeftGridPoint(MIN_GRID_X, MIN_GRID_Y);
	Point2D bottomRightGridPoint(MAX_GRID_X, MAX_GRID_Y);

	QuadTree myTree(upperLeftGridPoint, bottomRightGridPoint, myParticles);

	std::cout << "Total Children: " << myTree.rootNode->GlobalChildren.size() <<std::endl;
	int nPoints = 0;
	for (int i = 0; i < myTree.rootNode->GlobalChildren.size(); i++) {
		if (myTree.rootNode->GlobalChildren[i]->hasParticle == true) {
			nPoints++;
		}
	}


	std::cout << "Number of children, maybe: " << nPoints << std::endl;

#ifdef _WIN32
	system("PAUSE");
#endif


	return EXIT_SUCCESS;
}

