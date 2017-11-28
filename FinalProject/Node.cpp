#include "Node.h"
#include <limits>


Node::Node() {
	

}

Node::~Node()
{


}

void Node::attachChildNodes()
{


}

std::vector<Point2D> Node::subDivide(Point2D _upLeft, Point2D _boRight, double &sideLength) {


	sideLength = (_boRight.x - _upLeft.x) / 2;

	Point2D nwMP = Point2D(_upLeft.x + sideLength / 2.0, _upLeft.y + sideLength / 2.0);
	Point2D swMP = Point2D(nwMP.x, nwMP.y + sideLength);
	Point2D neMP = Point2D(nwMP.x + sideLength, nwMP.y);
	Point2D seMP = Point2D(nwMP.x + sideLength,  nwMP.y + sideLength);

	std::vector<Point2D> returnVec;
	returnVec.resize(4);
	returnVec[0] = nwMP;
	returnVec[1] = swMP;
	returnVec[2] = neMP;
	returnVec[3] = seMP;

	return returnVec;
}

std::vector<Point2D> Node::calculateBounding(Point2D _midpoint, double sideLength) {

	Point2D upLeft;
	Point2D boRight;

	upLeft.x = _midpoint.x - sideLength / 2.0;
	upLeft.y = _midpoint.y - sideLength / 2.0;
	boRight.x = _midpoint.x + sideLength / 2.0;
	boRight.y = _midpoint.y + sideLength / 2.0;

	std::vector<Point2D> returnVec;
	returnVec.push_back(upLeft);
	returnVec.push_back(boRight);

	return returnVec;
}




std::shared_ptr<Node> Node::getChildNode()
{

	return std::shared_ptr<Node>();
}


Point2D Node::calculateCOM(std::vector<Particle> localParticles, Point2D& centMass) {
	
	double massTotals = 0;
	for (size_t i = 0; i < localParticles.size(); ++i) {
		massTotals += localParticles[i].mass;
	}

	//#pragma omp for
	for (size_t i = 0; i < localParticles.size(); ++i) {
		centMass.x += (localParticles[i].pos.x)*(localParticles[i].mass) / massTotals;
		centMass.y += (localParticles[i].pos.y)*(localParticles[i].mass) / massTotals;
	}

	return centMass;
}
