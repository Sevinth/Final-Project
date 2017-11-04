#pragma once
#include "std.h"

class Node
{
public:
	Node(Point2D, Point2D, std::vector<Particle>, int _rank = 0);
	Node(Point2D, Point2D);
	~Node();
	
	void attachChildNodes();
	std::shared_ptr<Node> getChildNode();
	

	Point2D upLeft;
	Point2D boRight;

	double width;
	double height;
	double area;

	int rank{ 0 };
	bool hasChildren{ false };
	bool hasParticle{ false };
	Point2D centMass{0.0,0.0};
	std::vector<std::shared_ptr<Node>> childNodes;
	std::vector<Particle> localParticles;
};

