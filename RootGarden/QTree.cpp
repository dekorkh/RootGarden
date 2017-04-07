#include "QTree.h"
#include "MatterTypes.h"
#include "SceneComponent.h"

QTree::QTree(float inX, float inY, float inLength, QTree *inParent, SceneComponent *inOwner)
	:
	X(inX),
	Y(inY),
	Length(inLength),
	Parent(inParent),
	Owner(inOwner),
	isLeaf(true)
{
	Rectangle = new MatterRectangle();
	Rectangle->SetScale(Vector3f::Constant(Length * 0.5f));
	
	Vector3f NewPosition;
	NewPosition << inX, inY, 0.0f;
	Rectangle->SetPosition(NewPosition);

	Vector4f NewColor;
	float R = static_cast<float>(rand() % 255) / 255.0f;
	float G = static_cast<float>(rand() % 255) / 255.0f;
	float B = static_cast<float>(rand() % 255) / 255.0f;
	NewColor << 1, 1, 1, 0.2f;
	Rectangle->SetColor(NewColor);

	inOwner->AddChild(Rectangle);
}

QTree::~QTree()
{
	// delete any child trees
	if (SE != nullptr)
	{
		delete SE;
		delete SW;
		delete NE;
		delete NW;
	}
}

void QTree::Add(Matter &InNode)
{
	// Does it already have a node?
	if (Node == nullptr && isLeaf)
	{
		Node = &InNode;
		return;
	}
	else
	{
		if (isLeaf)
		{
			// Split
			SW = new QTree(X - Length * 0.25f, Y - Length * 0.25f, Length * 0.5f, this, Owner);
			SE = new QTree(X + Length * 0.25f, Y - Length * 0.25f, Length * 0.5f, this, Owner);
			NW = new QTree(X - Length * 0.25f, Y + Length * 0.25f, Length * 0.5f, this, Owner);
			NE = new QTree(X + Length * 0.25f, Y + Length * 0.25f, Length * 0.5f, this, Owner);
		}

		// Add existing node followed by the InNode to one of the children.
		Matter *Nodes[2] = { Node, &InNode };
		for (int i = 0; i < 2; i++)
		{
			if (Nodes[i] == nullptr)
			{
				continue;
			}
			if (Nodes[i]->Transform.GetPosition().x() < X)
			{
				if (Nodes[i]->Transform.GetPosition().y() < Y)
				{
					SW->Add(*Nodes[i]);
				}
				else
				{
					NW->Add(*Nodes[i]);
				}
			}
			else
			{
				if (Nodes[i]->Transform.GetPosition().y() < Y)
				{
					SE->Add(*Nodes[i]);
				}
				else
				{
					NE->Add(*Nodes[i]);
				}
			}
		}
		Node = nullptr;
		isLeaf = false;
	}
}

void QTree::SearchRegion(Vector3f const &Point, GLfloat const Radius, vector<Matter*> &OutMatter)
{
	// Color the rectangle yellow if it was tested.
	// Color the rectangle green if its contents should be included.
	// Color the rectangle red if its contents should be included but empty.
	
	Vector4f NewColor;
	NewColor << 1, 1, 0, 0.3f;
	Rectangle->SetColor(NewColor);
	Rectangle->Mesh->bDirty_Colors = true;

	// Start with root rectangle...  
	if (Parent == nullptr)
	{
		assert(!isLeaf); //If root there must always be at least one level of nodes before using search region.
	}

	if (!RegionIntersects(Point, Radius))
	{
		return;
	}

	// If isLeaf include.

	if (isLeaf)
	{
		if (Node)
		{
			Vector4f NewColor;
			NewColor << 0, 1, 0, 0.4f;
			Rectangle->SetColor(NewColor);
			Rectangle->Mesh->bDirty_Colors = true;
			OutMatter.insert(OutMatter.end(), Node);
		}
		else
		{
			Vector4f NewColor;
			NewColor << 1, 0, 0, 0.4f;
			Rectangle->SetColor(NewColor);
			Rectangle->Mesh->bDirty_Colors = true;
		}
		return;
	} 
	
	SE->SearchRegion(Point, Radius, OutMatter);
	SW->SearchRegion(Point, Radius, OutMatter);
	NE->SearchRegion(Point, Radius, OutMatter);
	NW->SearchRegion(Point, Radius, OutMatter);

	return;
}

Vector3f QTree::Center() const
{
	Vector3f Center;
	Center << X, Y, 0.0f;
	return Center;
}

Vector3f QTree::SE_Corner() const
{
	Vector3f Corner;
	Corner << (X + Length * 0.5f), (Y - Length * 0.5f), 0.0f;
	return Corner;
}

Vector3f QTree::SW_Corner() const
{
	Vector3f Corner;
	Corner << X - Length * 0.5f, (Y - Length * 0.5f), 0.0f;
	return Corner;
}

Vector3f QTree::NE_Corner() const
{
	Vector3f Corner;
	Corner << (X + Length * 0.5f), (Y + Length * 0.5f), 0.0f;
	return Corner;
}

Vector3f QTree::NW_Corner() const
{
	Vector3f Corner;
	Corner << (X - Length * 0.5f), (Y + Length * 0.5f), 0.0f;
	return Corner;
}

bool QTree::RegionIntersects(Vector3f const &Point, GLfloat const Radius) const
{
	// Either the point is inside rectangle or one of its lines has a point in the circle.
	// Is point inside rectangle?
	bool bInXBounds = (Point.x() > SW_Corner().x() && Point.x() < SE_Corner().x());
	bool bInYBounds = (Point.y() > SW_Corner().y() && Point.y() < NW_Corner().y());
	if (bInXBounds && bInYBounds)
	{
		return true;
	}

	// Otherwise for each side check if the shortest distance to it is within radius.

	// SIDES
	Vector3f SE_SW = (SW_Corner() - SE_Corner());
	Vector3f SW_NW = (NW_Corner() - SW_Corner());
	Vector3f NW_NE = (NE_Corner() - NW_Corner());
	Vector3f NE_SE = (SE_Corner() - NE_Corner());

	// Corner to point
	Vector3f SE_P = (Point - SE_Corner());
	Vector3f SW_P = (Point - SW_Corner());
	Vector3f NW_P = (Point - NW_Corner());
	Vector3f NE_P = (Point - NE_Corner());

	// Closest on each line
	GLfloat Dot_SE_SW = SE_P.normalized().dot(SE_SW.normalized());
	Vector3f Closest_SE_SW = SE_Corner() + Length * Dot_SE_SW * SE_SW.normalized();

	GLfloat Dot_SW_NW = SW_P.normalized().dot(SW_NW.normalized());
	Vector3f Closest_SW_NW = SW_Corner() + Length * Dot_SW_NW * SW_NW.normalized();

	GLfloat Dot_NW_NE = NW_P.normalized().dot(NW_NE.normalized());
	Vector3f Closest_NW_NE = NW_Corner() + Length * Dot_NW_NE * NW_NE.normalized();

	GLfloat Dot_NE_SE = NE_P.normalized().dot(NE_SE.normalized());
	Vector3f Closest_NE_SE = NE_Corner() + Length * Dot_NE_SE * NE_SE.normalized();

	// Distances from each side
	if (Dot_SE_SW > 0 && ((Closest_SE_SW - Point).norm() < Radius))
	{
		return true;
	}
	if (Dot_SW_NW > 0 && ((Closest_SW_NW - Point).norm() < Radius))
	{
		return true;
	}
	if (Dot_NW_NE > 0 && ((Closest_NW_NE - Point).norm() < Radius))
	{
		return true;
	}
	if (Dot_NE_SE > 0 && ((Closest_NE_SE - Point).norm() < Radius))
	{
		return true;
	}
	return false;
}

Matter* QTree::GetRightMost(Vector3f const &Point, GLfloat const Radius)
{
	vector<Matter*> FoundMatter;
	
	GetRoot()->SearchRegion(Point, 0.3f, FoundMatter);
	
	auto sortFunc_X = [](Matter *A, Matter *B) -> bool
	{
		return A->Transform.GetPosition().x() < B->Transform.GetPosition().x();
	};

	std::sort(FoundMatter.begin(), FoundMatter.end(), sortFunc_X);
	for (auto &it : FoundMatter)
	{
		if (it->Transform.GetPosition().x() - Point.x() > 0.0f)
		{
			return it;
		}
	}

	return nullptr;
}

Matter* QTree::GetLeftMost(Vector3f const &Point, GLfloat const Radius)
{
	vector<Matter*> FoundMatter;

	GetRoot()->SearchRegion(Point, 0.3f, FoundMatter);

	auto sortFunc_X = [](Matter *A, Matter *B) -> bool
	{
		return A->Transform.GetPosition().x() > B->Transform.GetPosition().x();
	};

	std::sort(FoundMatter.begin(), FoundMatter.end(), sortFunc_X);
	for (auto &it : FoundMatter)
	{
		if (it->Transform.GetPosition().x() - Point.x() < 0.0f)
		{
			return it;
		}
	}

	return nullptr;
}

Matter* QTree::GetTopMost(Vector3f const &Point, GLfloat const Radius)
{
	vector<Matter*> FoundMatter;

	GetRoot()->SearchRegion(Point, 0.3f, FoundMatter);

	auto sortFunc_Y = [](Matter *A, Matter *B) -> bool
	{
		return A->Transform.GetPosition().y() < B->Transform.GetPosition().y();
	};

	std::sort(FoundMatter.begin(), FoundMatter.end(), sortFunc_Y);
	for (auto &it : FoundMatter)
	{
		if (it->Transform.GetPosition().y() - Point.y() > 0.0f)
		{
			return it;
		}
	}

	return nullptr;
}

Matter* QTree::GetBottomMost(Vector3f const &Point, GLfloat const Radius)
{
	vector<Matter*> FoundMatter;

	GetRoot()->SearchRegion(Point, 0.3f, FoundMatter);

	auto sortFunc_Y = [](Matter *A, Matter *B) -> bool
	{
		return A->Transform.GetPosition().y() > B->Transform.GetPosition().y();
	};

	std::sort(FoundMatter.begin(), FoundMatter.end(), sortFunc_Y);
	for (auto &it : FoundMatter)
	{
		if (it->Transform.GetPosition().y() - Point.y() < 0.0f)
		{
			return it;
		}
	}

	return nullptr;
}


QTree* QTree::GetRoot()
{
	if (Parent == nullptr)
	{
		return this;
	}
	else
	{
		return Parent->GetRoot();
	}
}


