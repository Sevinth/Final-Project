#include "Node.h"




Node::Node(Point2D _upLeft, Point2D _boRight, std::vector<Particle> particleList)
{
	
	this->upLeft = _upLeft;
	this->boRight = _boRight;
	std::cout << "This Nodes dimensions: " << std::endl;
	std::cout << "Upper Left: " << upLeft.x << "," << upLeft.y << std::endl;
	std::cout << "Bottom Right: " << boRight.x << "," << boRight.y << std::endl;

	for (size_t i = 0; i < particleList.size(); ++i) {
		if (particleList[i].pos.x > upLeft.x && particleList[i].pos.y > upLeft.y &&
			particleList[i].pos.x < boRight.x && particleList[i].pos.y < boRight.y)
			{
			this->localParticles.push_back(particleList[i]);
		}
	}

	
	if (localParticles.size() > 1) {

	double massTotals = 0;
//Need reductions here
//#pragma omp parallel 
	{
//#pragma omp for
		for (size_t i = 0; i < localParticles.size(); ++i) {
			massTotals += localParticles[i].mass;
		}
		
//#pragma omp for
		for (size_t i = 0; i < localParticles.size(); ++i) {
			centMass.x += (localParticles[i].pos.x)*(localParticles[i].mass) / massTotals;
			centMass.y += (localParticles[i].pos.y)*(localParticles[i].mass) / massTotals;
		}
	}

				Node neNode(Point2D(upLeft.x, upLeft.y), Point2D(boRight.x / 2.0, boRight.y / 2.0), localParticles);
				Node seNode(Point2D(upLeft.x, boRight.y / 2), Point2D(boRight.x / 2.0, boRight.y), localParticles);
				Node nwNode(Point2D(boRight.x / 2, upLeft.y), Point2D(boRight.x, boRight.y / 2.0), localParticles);
				Node swNode(Point2D(boRight.x / 2.0, boRight.y / 2.0), Point2D(boRight.x, boRight.y), localParticles);

				childNodes.push_back(std::make_shared<Node>(neNode));
				childNodes.push_back(std::make_shared<Node>(seNode));
				childNodes.push_back(std::make_shared<Node>(nwNode));
				childNodes.push_back(std::make_shared<Node>(swNode));
				this->hasChildren = true;
							
	}
	else {
		
		double massTotals = 0;
		for (size_t i = 0; i < localParticles.size(); ++i) {
			massTotals += localParticles[i].mass;
		}
		if (massTotals > 0) {
			this->hasChildren = true;
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
