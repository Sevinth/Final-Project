#pragma once
#include "std.h"
#include "Node.h"

class QuadTree
{
public:
	QuadTree(Point2D, Point2D, std::vector<Particle>);
	QuadTree(Point2D, Point2D);
	~QuadTree();


	//Define Bounding box by upper left and lower right points
	Point2D topLeft;
	Point2D botRight;

	std::shared_ptr<Node> TreeSearch();

	std::shared_ptr<Node> rootNode;

};

