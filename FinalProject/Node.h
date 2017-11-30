#pragma once
#include "std.h"
#include <map>

class Node
{
public:
	Node();
	~Node();
	
	void attachChildNodes();
	std::shared_ptr<Node> getChildNode();
	Point2D calculateCOM(std::vector<Particle>, Point2D&);
	std::vector<Point2D> subDivide(Point2D, Point2D, double&);
	std::vector<Point2D> calculateBounding(Point2D, double);


	Point2D upLeft;
	Point2D boRight;

	double width;
	double height;
	double area;
	double sideLenth;

	int rank{ 0 };
	bool hasChildren{ false };
	bool hasParticle{ false };
	Point2D centMass{0.0,0.0};
	std::vector<std::shared_ptr<Node>> LocalChildren;
	std::shared_ptr<Node> GlobalParent{nullptr};
	std::shared_ptr<Node> LocalParent{ nullptr };
	std::vector<std::shared_ptr<Node>> GlobalChildren;
	std::vector<int> GlobalChildrenRank;
	std::vector<Particle> localParticles;
};

