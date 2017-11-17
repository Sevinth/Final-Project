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


std::shared_ptr<Node> QuadTree::TreeSearch() {


	return rootNode;
	
}

std::vector<std::shared_ptr<Node>> QuadTree::findNodesWParticles(std::shared_ptr<Node> node) {

	std::vector<std::shared_ptr<Node>> returnVetor;

	int totalParticleNumber = node->localParticles.size();
	
	int particleCount = 0;
	
	std::shared_ptr<Node> tempRoot;
	while (particleCount < totalParticleNumber) {

		for (int i = 0; i < 4; ++i) {
			if (node->childNodes[i]->hasChildren == true) {
				if (node->childNodes[i]->localParticles.size() > 1) {

				}

			}
		}


	}



}