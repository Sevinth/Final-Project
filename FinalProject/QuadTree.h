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
	//std::vector<std::shared_ptr<Node>> findNodesWParticles(std::shared_ptr<Node> node);
	std::shared_ptr<Node> rootNode;
	void BuildTree(Point2D _midPoint, double _sideLength, std::vector<Particle> particleList, int _rank, std::shared_ptr<Node> _GlobalParent, std::shared_ptr<Node> tNode);
	void MakeRootNode(Point2D _boRight, Point2D _upLeft, int _rank, std::shared_ptr<Node> root, std::vector<Particle> particleList);


};

