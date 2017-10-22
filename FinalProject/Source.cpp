#include <mpi.h>
#include "QuadTree.h"
#include <random>
#include <vector>
#include <memory>


int main() {

	std::random_device r;
	std::default_random_engine engine(r());
	std::uniform_real_distribution<double> dist(0, 10);
	std::mt19937_64 e2;
	//Setup initial quad tree.
	QuadTree center(Point2D(0, 0), Point2D(0, 0));
	std::vector<std::shared_ptr<Node>> nodeVec;

	nodeVec.resize(10);

	std::vector<Particle> myParticles;
	myParticles.resize(2);

	myParticles[0].pos.x = 2.0;
	myParticles[0].pos.y = 4.0;
	myParticles[1].pos.x = 5.1;
	myParticles[1].pos.y = 7.5;
	myParticles[0].mass = 5.0;

	Point2D upperLeftGridPoint(0.0, 0.0);
	Point2D bottomRightGridPoint(10.0, 10.0);

	Node myNode(upperLeftGridPoint, bottomRightGridPoint, myParticles);

	std::cout << myNode.centMass.x << " " << myNode.centMass.y << std::endl;
	std::cout << "Children?" << myNode.hasChildren << std::endl;

	for (int i = 0; i < myNode.childNodes.size(); ++i) {
		std::cout << "Children? " << myNode.childNodes[i]->hasChildren << std::endl;
		std::cout << "Particle? " << myNode.childNodes[i]->hasParticle << std::endl;
		std::cout << "Center of Mass: " << myNode.childNodes[i]->centMass.x << " " << myNode.childNodes[i]->centMass.y << std::endl;
	}


#ifdef _WIN32
	system("PAUSE");
#endif

	return EXIT_SUCCESS;
}

