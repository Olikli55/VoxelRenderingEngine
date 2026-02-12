#ifndef OCT_TREE_CLASS
#define OCT_TREE_CLASS


class OctTree
{
public:
	OctTree(short level);
	~OctTree();
	void subDevide();
	void eraseChildrens();
	void subDevideToMax();
	OctTree* data[2][2][2] = { nullptr };
	bool devided = 0;
	bool type = 0;
	short level = 0;


private:
	
};

#endif