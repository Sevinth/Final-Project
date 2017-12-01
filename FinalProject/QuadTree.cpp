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

	std::vector<std::shared_ptr<Node>> returnVetor;

	int totalParticleNumber = node->localParticles.size();
	
	int particleCount = 0;
	
	std::shared_ptr<Node> tempRoot;

}


void QuadTree::BuildTree(Point2D _midPoint, double _sideLength, std::vector<Particle> particleList, int _rank, std::shared_ptr<Node> _GlobalParent, std::shared_ptr<Node> thisNode) {

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
		
		std::vector<Point2D> newMidPoints;
		newMidPoints.resize(4);
		double newSide = 0;
		newMidPoints = thisNode->subDivide(thisNode->upLeft, thisNode->boRight, newSide);

		 std::shared_ptr<Node> nwNode = std::make_shared<Node>();
		 BuildTree(newMidPoints[0], newSide, thisNode->localParticles, thisNode->rank + 1, thisNode->GlobalParent, nwNode);
		 std::shared_ptr<Node> swNode = std::make_shared<Node>();;
		 BuildTree(newMidPoints[1], newSide, thisNode->localParticles, thisNode->rank + 1, thisNode->GlobalParent, swNode);
		 std::shared_ptr<Node> neNode = std::make_shared<Node>();;
		 BuildTree(newMidPoints[2], newSide, thisNode->localParticles, thisNode->rank + 1, thisNode->GlobalParent, neNode);
		 std::shared_ptr<Node> seNode = std::make_shared<Node>();;
		 BuildTree(newMidPoints[3], newSide, thisNode->localParticles, thisNode->rank + 1, thisNode->GlobalParent, seNode);


		//Add nodes to the child node vector
		thisNode->LocalChildren.push_back(neNode);
		thisNode->LocalChildren.push_back(seNode);
		thisNode->LocalChildren.push_back(nwNode);
		thisNode->LocalChildren.push_back(swNode);



		for (int i = 0; i <thisNode-> LocalChildren.size(); i++) {
			thisNode->GlobalParent->GlobalChildren.push_back(thisNode->LocalChildren[i]);
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

		std::vector<Point2D> newMidPoints;
		newMidPoints.resize(4);
		newMidPoints = root->subDivide(root->upLeft, root->boRight, sideLength);


		std::shared_ptr<Node> nwNode = std::make_shared<Node>();;
		BuildTree(newMidPoints[0], sideLength, root->localParticles, root->rank + 1, root, nwNode);
		std::shared_ptr<Node> swNode = std::make_shared<Node>();;
		BuildTree(newMidPoints[1], sideLength, root->localParticles, root->rank + 1, root, swNode);
		std::shared_ptr<Node> neNode = std::make_shared<Node>();;
		BuildTree(newMidPoints[2], sideLength, root->localParticles, root->rank + 1, root, neNode);
		std::shared_ptr<Node> seNode = std::make_shared<Node>();;
		BuildTree(newMidPoints[3], sideLength, root->localParticles, root->rank + 1, root, seNode);

		//Add nodes to the child node vector
		root->LocalChildren.push_back(neNode);
		root->LocalChildren.push_back(seNode);
		root->LocalChildren.push_back(nwNode);
		root->LocalChildren.push_back(swNode);



		for (int i = 0; i < root->LocalChildren.size(); i++) {
			root->GlobalChildren.push_back(root->LocalChildren[i]);
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


void QuadTree::UpdateTreeStructure(std::shared_ptr<Node> node) {

	
	for (int i = 0; i < rootNode->GlobalChildren.size(); i++) {
		
		if (rootNode->GlobalChildren[i] == node->LocalChildren[i]) {
			rootNode->GlobalChildren[i].reset;
			node->LocalChildren[i].reset;
			node->LocalChildren.erase[i];
			rootNode->GlobalChildren.erase[i];
		}


	}

	//TODO: NEED TO BUILD TREE FROM THIS NODE DOWN
	
}


void QuadTree::NodeListManager() {




}