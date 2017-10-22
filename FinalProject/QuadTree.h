#pragma once
#include "std.h"
#include "Node.h"

class QuadTree
{
public:
	QuadTree();
	~QuadTree();
	QuadTree(Point2D tl, Point2D br);

	//Define Bounding box by upper left and lower right points
	Point2D topLeft;
	Point2D botRight;

	std::shared_ptr<Node> rootNode;

};

