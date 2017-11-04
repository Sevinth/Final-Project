#include "Node.h"
#include <limits>



Node::Node(Point2D _upLeft, Point2D _boRight, std::vector<Particle> particleList, int _rank)
{
	if (_boRight.x < DBL_EPSILON || _boRight.y < DBL_EPSILON) {
		return;
	}
	this->rank = _rank;
	this->upLeft = _upLeft;
	this->boRight = _boRight;

	/*std::cout << "This Nodes dimensions: " << std::endl;
	std::cout << "Upper Left: " << upLeft.x << "," << upLeft.y << std::endl;
	std::cout << "Bottom Right: " << boRight.x << "," << boRight.y << std::endl;*/

	this->width = boRight.x - upLeft.x;
	this->height = boRight.y - upLeft.y;
	if (width != height) return;
	this->area = width*height;
	std::ofstream OutFile;

	localParticles.resize(1);

	//OutFile << "NW Upper: " << nwULDim.x << " " << nwULDim.y << std::endl;
	//OutFile << "NW Bottom: " << nwBRDim.x << " " << nwBRDim.y << std::endl;

	//OutFile << "SW Upper: " << swULDim.x << " " << swULDim.y << std::endl;
	//OutFile << "SW Bottom: " << swBRDim.x << " " << swBRDim.y << std::endl;

	//OutFile << "NE Upper: " << neULDim.x << " " << neULDim.y << std::endl;
	//OutFile << "NE Bottom: " << neBRDim.x << " " << neBRDim.y << std::endl;

	//OutFile << "SE Upper: " << swULDim.x << " " << swULDim.y << std::endl;
	//OutFile << "SE Bottom: " << swBRDim.x << " " << swBRDim.y << std::endl;
	//OutFile << " " << std::endl;

	//Determine if any of the particles passed in are inside the bounding box of this node
	//If they are, we deal with them.  If not, ignore them.
	for (size_t i = 0; i < particleList.size(); ++i) {
		if (particleList[i].pos.x > upLeft.x && particleList[i].pos.y > upLeft.y &&
			particleList[i].pos.x < boRight.x && particleList[i].pos.y < boRight.y)
		{
			this->localParticles.push_back(particleList[i]);
		}
	}

	//Only subdivide if there is more than one particle in the node
	if (localParticles.size() > 1 && this->area > 0.0) {

		//Create 4 subdivisions in this node of equation width & height
		//Example Values: Initial Gird upLeft.x = 0, upLeft.y = 0, boRight.x = 10, boRight.y = 10;
		// nwNode:  upLeft.x = 0, upLeft.y = 0; boRight.x = 5.0, boRight.y = 5.0
		//swNode: upLeft.x = 0, upLeft.y = 5.0; boRight.x = 5.0, boRight.y = 10.0
		//neNode: upLeft.x = 5, upLeft.y = 0; boRight.x = 10, boRight.y = 5
		//seNode: upLeft.x = 5, upLeft.y = 5, boRight.x = 10, boRight.y = 10
		//OutFile.open("Subdivision_Test.txt", std::ios::app);
		////Calculate subdivision points
		//OutFile << "Rank: " << rank << ", Area: " << area << std::endl;

		Point2D nwULDim(upLeft.x, upLeft.y);
		Point2D nwBRDim(boRight.x / 2.0, boRight.y / 2.0);

		Point2D swULDim(upLeft.x, boRight.y / 2.0);
		Point2D swBRDim(boRight.x / 2.0, boRight.y);

		Point2D neULDim(boRight.x / 2.0, upLeft.y);
		Point2D neBRDim(boRight.x, boRight.y / 2.0);

		Point2D seULDim = nwBRDim;
		Point2D seBRDim = boRight;


		double massTotals = 0;
		//Need reductions here
		//#pragma omp parallel 

		//#pragma omp for
		for (size_t i = 0; i < localParticles.size(); ++i) {
			massTotals += localParticles[i].mass;
		}

		//#pragma omp for
		for (size_t i = 0; i < localParticles.size(); ++i) {
			centMass.x += (localParticles[i].pos.x)*(localParticles[i].mass) / massTotals;
			centMass.y += (localParticles[i].pos.y)*(localParticles[i].mass) / massTotals;
		}
	
	Node nwNode(nwULDim, nwBRDim, localParticles, this->rank + 1);
	Node swNode(swULDim, swBRDim, localParticles, this->rank + 1);
	Node neNode(neULDim, neBRDim, localParticles, this->rank + 1);
	Node seNode(seULDim, seBRDim, localParticles, this->rank + 1);

	//Add nodes to the child node vector
	childNodes.push_back(std::make_shared<Node>(neNode));
	childNodes.push_back(std::make_shared<Node>(seNode));
	childNodes.push_back(std::make_shared<Node>(nwNode));
	childNodes.push_back(std::make_shared<Node>(swNode));
	this->hasChildren = true;
	this->hasParticle = true;
}
	else {
		
		double massTotals = 0;
		for (size_t i = 0; i < localParticles.size(); ++i) {
			massTotals += localParticles[i].mass;
		}
		if (massTotals > 0) {
			this->hasParticle = true;
			for (size_t i = 0; i < localParticles.size(); ++i) {
				centMass.x += (localParticles[i].pos.x)*(localParticles[i].mass) / massTotals;
				centMass.y += (localParticles[i].pos.y)*(localParticles[i].mass) / massTotals;
			}
		}
		else
		{
			
			this->hasChildren = false;
		}
	}

}

Node::Node(Point2D _upLeft, Point2D _boRight)
{
	this->upLeft = _upLeft;
	this->boRight = _boRight;
}


Node::~Node()
{


}

void Node::attachChildNodes()
{


}

std::shared_ptr<Node> Node::getChildNode()
{



	return std::shared_ptr<Node>();
}
