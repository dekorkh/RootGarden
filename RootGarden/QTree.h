#pragma once
class Matter;
class SceneComponent;

class QTree
{
public:
	QTree(float inX, float inY, float inLength, QTree const *inParent, SceneComponent *inOwner);
	~QTree();

	float const X;
	float const Y;
	float const Length;

	QTree const *Parent;

	QTree *SE;
	QTree *SW;
	QTree *NE;
	QTree *NW;

	Matter const *Node;

	bool isLeaf;

	SceneComponent *Owner;

	void Add(Matter const &InNode);
};