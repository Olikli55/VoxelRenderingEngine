#include "OctTreeClass.h"


OctTree::OctTree(short level)
	:devided(0), type(0), level(level){
}

OctTree::~OctTree()
{	
	if (data[0][0][0]) { delete data[0][0][0]; }
	if (data[0][0][1]) { delete data[0][0][1]; }
	if (data[0][1][0]) { delete data[0][1][0]; }
	if (data[0][1][1]) { delete data[0][1][1]; }

	if (data[1][0][0]) { delete data[1][0][0]; }
	if (data[1][0][1]) { delete data[1][0][1]; }
	if (data[1][1][0]) { delete data[1][1][0]; }
	if (data[1][1][1]) { delete data[1][1][1]; }

	data[0][0][0] = nullptr;
	data[0][0][1] = nullptr;
	data[0][1][0] = nullptr;
	data[0][1][1] = nullptr;

	data[1][0][0] = nullptr;
	data[1][0][1] = nullptr;
	data[1][1][0] = nullptr;
	data[1][1][1] = nullptr;
}



void OctTree::subDevide() {
	devided = 1;
	//faster than for loop;
	data[0][0][0] = new OctTree(level + 1);
	data[0][0][1] = new OctTree(level + 1);
	data[0][1][0] = new OctTree(level + 1);
	data[0][1][1] = new OctTree(level + 1);

	data[1][0][0] = new OctTree(level + 1);
	data[1][0][1] = new OctTree(level + 1);
	data[1][1][0] = new OctTree(level + 1);
	data[1][1][1] = new OctTree(level + 1);


}



void OctTree::eraseChildrens() {

	//deletes the memory in the stack, delete aautomaticli calls the deconstructor
	if (data[0][0][0]) { delete data[0][0][0]; }
	if (data[0][0][1]) { delete data[0][0][1]; }
	if (data[0][1][0]) { delete data[0][1][0]; }
	if (data[0][1][1]) { delete data[0][1][1]; }

	if (data[1][0][0]) { delete data[1][0][0]; }
	if (data[1][0][1]) { delete data[1][0][1]; }
	if (data[1][1][0]) { delete data[1][1][0]; }
	if (data[1][1][1]) { delete data[1][1][1]; }



	//delete dangleging pointers
	data[0][0][0] = nullptr;
	data[0][0][1] = nullptr;
	data[0][1][0] = nullptr;
	data[0][1][1] = nullptr;

	data[1][0][0] = nullptr;
	data[1][0][1] = nullptr;
	data[1][1][0] = nullptr;
	data[1][1][1] = nullptr;
}



void OctTree::subDevideToMax() {
	if (level == 16) { return; }
	subDevide();
	data[0][0][0]->subDevideToMax();
	data[0][0][1]->subDevideToMax();
	data[0][1][0]->subDevideToMax();
	data[0][1][1]->subDevideToMax();

	data[1][0][0]->subDevideToMax();
	data[1][0][1]->subDevideToMax();
	data[1][1][0]->subDevideToMax();
	data[1][1][1]->subDevideToMax();

}
