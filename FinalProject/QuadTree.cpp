#include "QuadTree.h"

QuadTree::QuadTree(Point2D tl, Point2D br, std::vector<Particle> particles)
{
	Node node(tl, br, particles);
	rootNode = std::make_shared<Node>(node);

}

QuadTree::QuadTree(Point2D tl, Point2D br) {

	Node node(tl, br);
	rootNode = std::make_shared<Node>(node);


}

QuadTree::~QuadTree()
{

}
