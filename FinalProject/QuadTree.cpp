#include "QuadTree.h"

QuadTree::QuadTree(Point2D tl, Point2D br, std::vector<Particle> particles)
{
	
	rootNode = std::make_shared<Node>();

	MakeRootNode(br, tl, 0, rootNode, particles);

}

QuadTree::QuadTree(Point2D tl, Point2D br) {

	rootNode = std::shared_ptr<Node>();

}

QuadTree::~QuadTree()
{
	

}


std::shared_ptr<Node> QuadTree::TreeSearch() {


	return rootNode;
	
}

std::vector<std::shared_ptr<Node>> QuadTree::findNodesWParticles(std::shared_ptr<Node> node) {

	std::vector<std::shared_ptr<Node>> returnVector;

	int totalParticleNumber = node->localParticles.size();
	
	int particleCount = 0;
	
	std::shared_ptr<Node> tempRoot;

	return returnVector;
}


void QuadTree::BuildTree(Point2D _midPoint, double _sideLength, std::vector<Particle> particleList, int _rank, std::shared_ptr<Node> _GlobalParent, std::shared_ptr<Node> thisNode, std::shared_ptr<Node> locParent) {

	std::ofstream outFile;
	outFile.open("Data.txt", std::ios::app);


	Point2D nwVert;
	Point2D swVert;
	Point2D neVert;
	Point2D seVert;

	std::vector<Point2D> bounds = thisNode->calculateBounding(_midPoint, _sideLength);
	thisNode->upLeft = bounds[0];
	thisNode->boRight = bounds[1];
	thisNode->sideLenth = _sideLength;
	thisNode->rank = _rank;
	thisNode->GlobalParent = _GlobalParent;

	nwVert = thisNode->upLeft;
	seVert = thisNode->boRight;
	neVert.x = thisNode->boRight.x;
	neVert.y = thisNode->upLeft.y;
	swVert.x = thisNode->upLeft.x;
	swVert.y = thisNode->boRight.y;


	std::cout << "Rank: " << thisNode->rank << ",";
	std::cout << _midPoint.x << " " << _midPoint.y << std::endl;
	outFile << nwVert.x << " " << nwVert.y << " " << swVert.x << " " << swVert.y << " " << neVert.x << " " << neVert.y << " " << seVert.x << " " << seVert.y << " " << _midPoint.x << " " << _midPoint.y << std::endl;
	outFile.close();
	for (size_t i = 0; i < particleList.size(); ++i) {
		if (particleList[i].pos.x > thisNode->upLeft.x && particleList[i].pos.y > thisNode->upLeft.y &&
			particleList[i].pos.x < thisNode->boRight.x && particleList[i].pos.y < thisNode->boRight.y)
		{
			thisNode->localParticles.push_back(particleList[i]);
		}
	}

	if (thisNode->localParticles.size() > 1) {
		
		double sideLength;

		thisNode->calculateCOM(thisNode->localParticles, thisNode->centMass);

		std::shared_ptr<Node> nwNode = std::make_shared<Node>();
		std::shared_ptr<Node> swNode = std::make_shared<Node>();
		std::shared_ptr<Node> neNode = std::make_shared<Node>();
		std::shared_ptr<Node> seNode = std::make_shared<Node>();

		QuadTreeSubDivide(thisNode, nwNode, swNode, neNode, seNode, sideLength,locParent);

		//Add nodes to the child node vector
		thisNode->LocalChildren.push_back(neNode);
		thisNode->LocalChildren.push_back(seNode);
		thisNode->LocalChildren.push_back(nwNode);
		thisNode->LocalChildren.push_back(swNode);



		for (int i = 0; i <thisNode-> LocalChildren.size(); i++) {
			thisNode->GlobalParent->GlobalChildren.push_back(thisNode->LocalChildren[i]);
			thisNode->GlobalParent->GlobalChildrenRank.push_back(thisNode->rank + 1);
			thisNode->GlobalChildren.push_back(thisNode->LocalChildren[i]);

		}

		thisNode->hasChildren = true;
		thisNode->hasParticle = true;
	}
	else {
		double massTotals = 0;
		for (size_t i = 0; i < thisNode->localParticles.size(); ++i) {
			massTotals += thisNode->localParticles[i].mass;
		}
		if (massTotals > 0) {
			thisNode->hasParticle = true;
			this->NodesContainingParticles.push_back(thisNode);
			thisNode->calculateCOM(particleList, thisNode->centMass);
		}
		else
		{
			thisNode->hasChildren = false;
		}
	}

}


void QuadTree::MakeRootNode(Point2D _boRight, Point2D _upLeft, int _rank, std::shared_ptr<Node> root, std::vector<Particle> particleList) {

	if (_boRight.x < DBL_EPSILON || _boRight.y < DBL_EPSILON) {
		return;
	}

	root->rank = _rank;
	root->upLeft = _upLeft;
	root->boRight = _boRight;

	/*std::cout << "This Nodes dimensions: " << std::endl;
	std::cout << "Upper Left: " << upLeft.x << "," << upLeft.y << std::endl;
	std::cout << "Bottom Right: " << boRight.x << "," << boRight.y << std::endl;*/

	root->width = root->boRight.x - root->upLeft.x;
	root->height = root->boRight.y - root->upLeft.y;
	if (root->width !=root->height) return;
	root->area = root->width*root->height;
	std::ofstream OutFile;

	root->localParticles.resize(1);

	//Determine if any of the particles passed in are inside the bounding box of this node
	//If they are, we deal with them.  If not, ignore them.
	for (size_t i = 0; i < particleList.size(); ++i) {
		if (particleList[i].pos.x > root->upLeft.x && particleList[i].pos.y > root->upLeft.y &&
			particleList[i].pos.x <root-> boRight.x && particleList[i].pos.y < root->boRight.y)
		{
			root->localParticles.push_back(particleList[i]);
		}
	}

	//Only subdivide if there is more than one particle in the node
	if (root->localParticles.size() > 1 && root->area > 0.0) {
		double sideLength;

		root->calculateCOM(root->localParticles, root->centMass);
		std::shared_ptr<Node> nwNode = std::make_shared<Node>();
		std::shared_ptr<Node> swNode = std::make_shared<Node>();
		std::shared_ptr<Node> neNode = std::make_shared<Node>();
		std::shared_ptr<Node> seNode = std::make_shared<Node>();

		QuadTreeSubDivide(root, nwNode, swNode, neNode, seNode, sideLength, root);

		//Add nodes to the child node vector
		root->LocalChildren.push_back(neNode);
		root->LocalChildren.push_back(seNode);
		root->LocalChildren.push_back(nwNode);
		root->LocalChildren.push_back(swNode);


		for (int i = 0; i < root->LocalChildren.size(); i++) {

			root->GlobalChildren.push_back(root->LocalChildren[i]);
			root->GlobalChildrenRank.push_back(root->rank + 1);
			
		}



		root->hasChildren = true;
		root->hasParticle = true;
	}
	else {

		double massTotals = 0;
		for (size_t i = 0; i < root->localParticles.size(); ++i) {
			massTotals += root->localParticles[i].mass;
		}
		if (massTotals > 0) {
			root->hasParticle = true;
			root->calculateCOM(particleList, root->centMass);
		}
		else
		{

			root->hasChildren = false;
		}
	}
}



void QuadTree::passToAdjacent(Particle p, std::shared_ptr<Node> nodeParent, std::shared_ptr<Node> cNode) {

	if (p.pos.x > nodeParent->upLeft.x && p.pos.y > nodeParent->upLeft.y &&
		p.pos.x < nodeParent->boRight.x && p.pos.y < nodeParent->boRight.y)
	{
		
		for (int i = 0; i < nodeParent->LocalChildren.size(); i++) {

			if (p.pos.x > nodeParent->LocalChildren[i]->upLeft.x &&
				p.pos.y > nodeParent->LocalChildren[i]->upLeft.y &&
				p.pos.x < nodeParent->LocalChildren[i]->boRight.x &&
				p.pos.y < nodeParent->LocalChildren[i]->boRight.y) {

				//Particle is inside this child node
				nodeParent->LocalChildren[i]->localParticles.push_back(p);
				if (nodeParent->LocalChildren[i]->localParticles.size() > 1) {
						
				}
				
			}

		}

	}

}

void QuadTree:: RemoveChildren(std::shared_ptr<Node> node) {
	
	for (int i = 0; i < node->LocalChildren.size(); i++) {
		node->LocalChildren[i].reset();
	}
	
	node->LocalChildren.clear();

}


void QuadTree::QuadTreeSubDivide(std::shared_ptr<Node> node,
	std::shared_ptr<Node> nwNode, std::shared_ptr<Node> swNode, 
	std::shared_ptr<Node> neNode, std::shared_ptr<Node> seNode, double& sideLength,std::shared_ptr<Node> locParent) {
	std::vector<Point2D> newMidPoints;

	newMidPoints.resize(4);
	newMidPoints = node->subDivide(node->upLeft, node->boRight, sideLength);

	BuildTree(newMidPoints[0], sideLength, node->localParticles,node->rank + 1, node, nwNode, locParent);
	BuildTree(newMidPoints[1], sideLength, node->localParticles, node->rank + 1, node, swNode, locParent);
	BuildTree(newMidPoints[2], sideLength, node->localParticles, node->rank + 1, node, neNode, locParent);
	BuildTree(newMidPoints[3], sideLength, node->localParticles, node->rank + 1, node, seNode, locParent);

}


//Update tree structure below the argument node
void QuadTree::UpdateTreeStructure(std::shared_ptr<Node> node) {


	std::shared_ptr<Node> nwNode = std::make_shared<Node>();
	std::shared_ptr<Node> swNode = std::make_shared<Node>();
	std::shared_ptr<Node> neNode = std::make_shared<Node>();
	std::shared_ptr<Node> seNode = std::make_shared<Node>();
	
	
	std::vector<Point2D> newMidPoints;
	newMidPoints.resize(4);
	double sideLength = 0;

	newMidPoints = node->subDivide(node->upLeft, node->boRight, sideLength);

	QuadTreeSubDivide(node, nwNode, swNode, neNode, seNode, sideLength, node);

}

//Recursively search tree for nodes whose COM is > R_MAX
Point2D QuadTree::QuadTreeIterate(std::shared_ptr<Node> node, Particle p) {

	double dist = checkDistance(node->centMass, p.pos);

	if (dist > R_MAX) {
		
		return node->centMass;

	}
	else {
		if(node->LocalChildren[0]->hasChildren == true)
			QuadTreeIterate(node->LocalChildren[0], p);
		if (node->LocalChildren[1]->hasChildren == true)
			QuadTreeIterate(node->LocalChildren[1], p);
		if (node->LocalChildren[2]->hasChildren == true)
			QuadTreeIterate(node->LocalChildren[2], p);
		if (node->LocalChildren[3]->hasChildren == true)
			QuadTreeIterate(node->LocalChildren[3], p);
	}
}


double QuadTree::checkDistance(Point2D x, Point2D y) {

	return std::sqrt(std::pow(y.x - x.x, 2) + std::pow(y.y - x.y, 2));

}