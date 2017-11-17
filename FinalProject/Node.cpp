#include "Node.h"
#include <limits>

Node::Node(Point2D _midPoint, double _sideLength, std::vector<Particle> particleList, int _rank) {

	std::vector<Point2D> bounds = calculateBounding(_midPoint, _sideLength);
	this->upLeft = bounds[0];
	this->boRight = bounds[1];
	this->sideLenth = _sideLength;

	if (particleList.size() > 1) {
		std::vector<Point2D> newMidPoints;
		newMidPoints.resize(4);
		double newSide = 0;
		newMidPoints = subDivide(upLeft, boRight, newSide);

		Node nwNode(newMidPoints[0], newSide, localParticles, this->rank + 1);
		Node swNode(newMidPoints[1], newSide, localParticles, this->rank + 1);
		Node neNode(newMidPoints[2], newSide, localParticles, this->rank + 1);
		Node seNode(newMidPoints[3], newSide, localParticles, this->rank + 1);


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
			calculateCOM(particleList, centMass);
		}
		else
		{

			this->hasChildren = false;
		}
	}

}

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
		double sideLength;
	
    	calculateCOM(localParticles, centMass);

		std::vector<Point2D> newMidPoints;
		newMidPoints.resize(4);
		newMidPoints = subDivide(this->upLeft, this->boRight, sideLength);

		Node nwNode(newMidPoints[0], sideLength, localParticles, this->rank + 1);
		Node swNode(newMidPoints[1], sideLength, localParticles, this->rank + 1);
		Node neNode(newMidPoints[2], sideLength, localParticles, this->rank + 1);
		Node seNode(newMidPoints[3], sideLength, localParticles, this->rank + 1);


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
			calculateCOM(particleList, centMass);
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

std::vector<Point2D> Node::subDivide(Point2D _upLeft, Point2D _boRight, double &sideLength) {


	sideLength = (_boRight.x - _upLeft.x) / 2;

	Point2D nwMP = Point2D(_upLeft.x + sideLength / 2.0, _upLeft.y + sideLength / 2.0);
	Point2D swMP = Point2D(_upLeft.x + sideLength / 2.0, _upLeft.y + sideLength);
	Point2D neMP = Point2D(_upLeft.x + sideLength, _upLeft.y + sideLength / 2.0);
	Point2D seMP = Point2D(_upLeft.x + sideLength, _upLeft.y + sideLength);

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