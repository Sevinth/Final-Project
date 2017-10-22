#pragma once
#include "std.h"

class Node
{
public:
	Node(Point2D, Point2D, std::vector<Particle>);
	~Node();
	
	void attachChildNodes();
	std::shared_ptr<Node> getChildNode();
	

	Point2D upLeft;
	Point2D boRight;

	double width;
	double height;
	
	bool hasChildren{ false };
	Point2D centMass{0.0,0.0};
	std::vector<std::shared_ptr<Node>> childNodes;
	std::vector<Particle> localParticles;
};

