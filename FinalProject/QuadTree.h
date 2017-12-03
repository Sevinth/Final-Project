#pragma once
#include "std.h"
#include "Node.h"
#define R_MAX 5

class QuadTree
{
public:
	QuadTree(Point2D, Point2D, std::vector<Particle>);
	QuadTree(Point2D, Point2D);
	~QuadTree();

	//Define Bounding box by upper left and lower right points
	Point2D topLeft;
	Point2D botRight;

	//Searh Tree
	//What parameters do we need to be able to search for?
	std::shared_ptr<Node> TreeSearch();
	std::vector<std::shared_ptr<Node>> findNodesWParticles(std::shared_ptr<Node> node);
	std::shared_ptr<Node> rootNode;
	std::vector<std::shared_ptr<Node>> NodesContainingParticles;

	void passToAdjacent(Particle, std::shared_ptr<Node>, std::shared_ptr<Node>);
	void RemoveChildren(std::shared_ptr<Node>);
	void BuildTree(Point2D _midPoint, double _sideLength, std::vector<Particle> particleList, int _rank, std::shared_ptr<Node> _GlobalParent, std::shared_ptr<Node> tNode, std::shared_ptr<Node> locParent);
	void MakeRootNode(Point2D _boRight, Point2D _upLeft, int _rank, std::shared_ptr<Node> root, std::vector<Particle> particleList);
	void QuadTreeSubDivide(std::shared_ptr<Node> node,
		std::shared_ptr<Node> nwNode, std::shared_ptr<Node> swNode,
		std::shared_ptr<Node> neNode, std::shared_ptr<Node> seNode, double&, std::shared_ptr<Node> locParent);
	void UpdateTreeStructure(std::shared_ptr<Node> node);
	Point2D QuadTreeIterate(std::shared_ptr<Node> node, Particle p);
	double checkDistance(Point2D, Point2D);

};

