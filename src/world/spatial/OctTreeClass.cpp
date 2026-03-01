#include "OctTreeClass.h"

OctTree::OctTree()
	: devided(false), type(0) {

}


OctTree::~OctTree() {


}

void OctTree::Subdevide() {
	devided = true;
	data[0][0][0] = nullptr;
	data[0][0][1] = nullptr;
	data[0][1][0] = nullptr;
	data[0][1][1] = nullptr;

	data[1][0][0] = nullptr;
	data[1][0][1] = nullptr;
	data[1][1][0] = nullptr;
	data[1][1][1] = nullptr;

}