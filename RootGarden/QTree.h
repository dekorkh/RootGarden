#include "Eigen/StdVector"
#include "Eigen/Dense"
#include "GL/glew.h"
#include <vector>

#pragma once
class Matter;
class SceneComponent;
class MatterRectangle;

using namespace Eigen;
using namespace std;

class QTree
{
public:
	QTree(float inX, float inY, float inLength, QTree *inParent, SceneComponent *inOwner);
	~QTree();

	float const X;
	float const Y;
	float const Length;

	QTree *Parent;

	QTree *SE;
	QTree *SW;
	QTree *NE;
	QTree *NW;

	Matter *Node;

	bool isLeaf;

	SceneComponent *Owner;

	void Add(Matter &InNode);

	void SearchRegion(Vector3f const &Point, GLfloat const Radius, vector<Matter*> &OutMatter);

	/*
	/	Return true if the specified region intersects with bounds of this node
	*/
	bool RegionIntersects(Vector3f const &Point, GLfloat const Radius) const;

	Matter* GetRightMost(Vector3f const &Point, GLfloat const Radius);
	Matter* GetLeftMost(Vector3f const &Point, GLfloat const Radius);
	Matter* GetTopMost(Vector3f const &Point, GLfloat const Radius);
	Matter* GetBottomMost(Vector3f const &Point, GLfloat const Radius);

	/*
	/ Return the root of the tree
	*/
	QTree* GetRoot();


	Vector3f Center() const;
	Vector3f SE_Corner() const;
	Vector3f SW_Corner() const;
	Vector3f NE_Corner() const;
	Vector3f NW_Corner() const;

	MatterRectangle* Rectangle;
};