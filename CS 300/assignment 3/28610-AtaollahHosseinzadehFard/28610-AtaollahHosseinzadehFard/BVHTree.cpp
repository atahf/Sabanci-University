#include "BVHTree.h"

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}
std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}

/*
	part after this message is written by 
	Ataollah Hosseinzadeh Fard with ID 28610
*/

//default constructor
BVHTree::BVHTree() {
	this->root = NULL;
}

//destructor
BVHTree::~BVHTree() {
	deleteAllNodes(root);
	map.clear();
}

//adds a leaf to tree with defind constraits
void BVHTree::addBVHMember(AABB objectArea, std::string name) {
	if (root == NULL) {
		BVHTreeNode* newNode = new BVHTreeNode(objectArea, name, true);

		root = newNode;
		map[name] = newNode;
	}
	else if(root->isLeaf) {
		BVHTreeNode* oldRoot = root;
		BVHTreeNode* newBranch = new BVHTreeNode(oldRoot->aabb + objectArea, "branch", false);
		BVHTreeNode* newNode = new BVHTreeNode(objectArea, name, true);

		newBranch->parent = NULL;
		newBranch->rightChild = oldRoot;
		newBranch->leftChild = newNode;
		newNode->parent = newBranch;
		oldRoot->parent = newBranch;

		root = newBranch;
		map[name] = newNode;
	}
	else {
		BVHTreeNode* existingLeaf = root;
		BVHTreeNode* newBranch;
		BVHTreeNode* newNode = new BVHTreeNode(objectArea, name, true);
		while(!existingLeaf->isLeaf) {
			int increaseInRightTreeSize = AABB::unionArea(newNode->aabb, existingLeaf->rightChild->aabb) - existingLeaf->rightChild->aabb.getArea();
			int increaseInLeftTreeSize = AABB::unionArea(newNode->aabb, existingLeaf->leftChild->aabb) - existingLeaf->leftChild->aabb.getArea();
			if(increaseInRightTreeSize < increaseInLeftTreeSize)
				existingLeaf = existingLeaf->rightChild;
			else
				existingLeaf = existingLeaf->leftChild;
		}
		AABB newMasterArea = existingLeaf->aabb + objectArea;
		newBranch = new BVHTreeNode(newMasterArea, "branch", false);
		newBranch->parent = existingLeaf->parent;
		if(existingLeaf->parent->leftChild == existingLeaf)
			existingLeaf->parent->leftChild = newBranch;
		else
			existingLeaf->parent->rightChild = newBranch;
		existingLeaf->parent = newBranch;
		newNode->parent = newBranch;
		newBranch->leftChild = newNode;
		newBranch->rightChild = existingLeaf;
		BVHTreeNode* ptr = newBranch;
		while(ptr != NULL) {
			int minX = AABB::getMin(ptr->aabb.minX, objectArea.minX);
			int maxX = AABB::getMax(ptr->aabb.maxX, objectArea.maxX);
			int minY = AABB::getMin(ptr->aabb.minY, objectArea.minY);
			int maxY = AABB::getMax(ptr->aabb.maxY, objectArea.maxY);
			AABB newArea(minX, minY, maxX, maxY);
			ptr->aabb = newArea;
			ptr = ptr->parent;
		}
		map[name] = newNode;
	}
}

//moves a leaf to given AABB location with defined constraits
void BVHTree::moveBVHMember(std::string name, AABB newLocation) {
	if(root == NULL)					//tree is empty
		return;
	if(map.find(name) == map.end())		//name does not exist in tree
		return;
	BVHTreeNode* toMove = map[name];
	if(toMove == root) {
		root->aabb = newLocation;
		return;
	}
	AABB parentArea = toMove->parent->aabb;
	if(parentArea + newLocation != parentArea) {
		removeBVHMember(name);
		addBVHMember(newLocation, name);
		fixAreas(root);
	}
	else {
		toMove->aabb = newLocation;
	}
}

//removes a leaf and rearrenges to meet the defined constraits
void BVHTree::removeBVHMember(std::string name) {
	if(root == NULL)					//tree is empty
		return;
	if(map.find(name) == map.end())		//name does not exist in tree
		return;
	BVHTreeNode* toRemove = map[name];
	if(toRemove == root) {
		delete root;
		root = NULL;
	}
	else if (toRemove->parent == root) {
		BVHTreeNode* sibling;
		if(toRemove->parent->leftChild == toRemove)
			sibling = toRemove->parent->rightChild;
		else
			sibling = toRemove->parent->leftChild;
		sibling->parent = NULL;
		delete toRemove;
		toRemove = NULL;
	}
	else {
		BVHTreeNode* masterBranch = toRemove->parent->parent;
		BVHTreeNode* parent = toRemove->parent;
		BVHTreeNode* sibling;
		if(toRemove->parent->leftChild == toRemove)
			sibling = toRemove->parent->rightChild;
		else
			sibling = toRemove->parent->leftChild;
		
		if(masterBranch->leftChild == parent)
			masterBranch->leftChild = sibling;
		else
			masterBranch->rightChild = sibling;
		sibling->parent = masterBranch;
		delete toRemove;
		toRemove = NULL;
	}
	map.erase(name);
}

//calls private function of getCollides and returns its output
std::vector<std::string> BVHTree::getCollidingObjects(AABB object) {
	std::vector<std::string> res;
	getCollides(res, root, object);
	return res;
}

//private function that finds all the leaf names of leaf that collide with the given AABB location
void BVHTree::getCollides(std::vector<std::string> &v, BVHTreeNode* ptr, const AABB object) const {
    if(ptr != NULL) {
		if(ptr->aabb.collide(object)) {
			if(!ptr->isLeaf){
				getCollides(v, ptr->leftChild, object);
				getCollides(v, ptr->rightChild, object);
			}
			else 
				v.push_back(ptr->name);
		}
	}
}

//private function that deallocates all nodes in tree in recursive way and will be used in destructor
void BVHTree::deleteAllNodes(BVHTreeNode* ptr) {
	if(ptr != NULL) {
		deleteAllNodes(ptr->leftChild);
		deleteAllNodes(ptr->rightChild);
		delete ptr;
	}
}

//private function that fixes the AABBs of branch nodes in tree according to their children and this will be used in moveBVHMember
void BVHTree::fixAreas(BVHTreeNode* ptr) {
	if(!ptr->isLeaf) {
		fixAreas(ptr->leftChild);
		fixAreas(ptr->rightChild);
		ptr->aabb = ptr->leftChild->aabb + ptr->rightChild->aabb;
	}
}