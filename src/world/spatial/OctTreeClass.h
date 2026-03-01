#ifndef OCT_TREE_CLASS_H
#define OCT_TREE_CLASS_H

class OctTree 
{
private:
	OctTree();
	~OctTree();
	bool devided = 0;
	OctTree* data[2][2][2] = { nullptr };
	bool type = false;
public:
	void Subdevide();
};
#endif