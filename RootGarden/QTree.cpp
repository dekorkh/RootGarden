#include "QTree.h"
#include "MatterTypes.h"
#include "SceneComponent.h"

GLfloat QTree::MarkerFadeDist;
Vector3f QTree::MarkerScale;
Vector4f QTree::MarkerColor_ON;
Vector4f QTree::MarkerColor_OFF;

QTree::QTree(float inX, float inY, float inLength, QTree *inParent, SceneComponent *inOwner)
	:
	X(inX),
	Y(inY),
	Length(inLength),
	Parent(inParent),
	Owner(inOwner),
	isLeaf(true),
	bVisualize(false)
{

	if (bVisualize)
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

	
		MarkerColor_ON << 0.0f, 1.0f, 0.0f, 1.0f;
		MarkerColor_OFF << 0.0f, 0.0f, 0.0f, 1.0f;
		MarkerScale = Vector3f::Constant(0.005f);
		MarkerFadeDist = 0.5f;

		Marker_SE_SW = new MatterCircle();
		Marker_SW_NW = new MatterCircle();
		Marker_NW_NE = new MatterCircle();
		Marker_NE_SE = new MatterCircle();

		Marker_SE_SW->SetScale(MarkerScale);
		Marker_SW_NW->SetScale(MarkerScale);
		Marker_NW_NE->SetScale(MarkerScale);
		Marker_NE_SE->SetScale(MarkerScale);

		Marker_SE_SW->SetColors(MarkerColor_OFF, MarkerColor_OFF);
		Marker_SW_NW->SetColors(MarkerColor_OFF, MarkerColor_OFF);
		Marker_NW_NE->SetColors(MarkerColor_OFF, MarkerColor_OFF);
		Marker_NE_SE->SetColors(MarkerColor_OFF, MarkerColor_OFF);

		inOwner->AddChild(Marker_SE_SW);
		inOwner->AddChild(Marker_SW_NW);
		inOwner->AddChild(Marker_NW_NE);
		inOwner->AddChild(Marker_NE_SE);
	}
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

void QTree::SearchRegion(Vector3f const &Point, GLfloat const Radius, vector<Matter*> &OutMatter, int Depth, int CornerIdx)
{
	// Color the rectangle yellow if it was tested.
	// Color the rectangle green if its contents should be included.
	// Color the rectangle red if its contents should be included but empty.
	
	if (bVisualize)
	{
		Vector4f NewColor;
		NewColor << 1, 1, 0, 0.3f;
		Rectangle->SetColor(NewColor);
		Rectangle->Mesh->bDirty_Colors = true;
	}

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
			if (bVisualize)
			{
				Vector4f NewColor;
				NewColor << 0, 1, 0, 0.4f;
				Rectangle->SetColor(NewColor);
				Rectangle->Mesh->bDirty_Colors = true;
			}
			OutMatter.insert(OutMatter.end(), Node);
		}
		else
		{
			if (bVisualize)
			{
				Vector4f NewColor;
				NewColor << 1, 0, 0, 0.4f;
				Rectangle->SetColor(NewColor);
				Rectangle->Mesh->bDirty_Colors = true;
			}
		}
		return;
	} 
	
	SE->SearchRegion(Point, Radius, OutMatter, Depth + 1, 3);
	SW->SearchRegion(Point, Radius, OutMatter, Depth + 1, 2);
	NE->SearchRegion(Point, Radius, OutMatter, Depth + 1, 1);
	NW->SearchRegion(Point, Radius, OutMatter, Depth + 1, 0);

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

	// Closest on each line	
	Vector3f Closest_SE_SW = ClosestPointOnSegment(SE_Corner(), SW_Corner(), Point);
	Vector3f Closest_SW_NW = ClosestPointOnSegment(SW_Corner(), NW_Corner(), Point);
	Vector3f Closest_NW_NE = ClosestPointOnSegment(NW_Corner(), NE_Corner(), Point);
	Vector3f Closest_NE_SE = ClosestPointOnSegment(NE_Corner(), SE_Corner(), Point);

	GLfloat Dist_SE_SW = (Closest_SE_SW - Point).norm();
	GLfloat Dist_SW_NW = (Closest_SW_NW - Point).norm();
	GLfloat Dist_NW_NE = (Closest_NW_NE - Point).norm();
	GLfloat Dist_NE_SE = (Closest_NE_SE - Point).norm();

	if (bVisualize)
	{
		Marker_SE_SW->SetPosition(Closest_SE_SW);
		Marker_SW_NW->SetPosition(Closest_SW_NW);
		Marker_NW_NE->SetPosition(Closest_NW_NE);
		Marker_NE_SE->SetPosition(Closest_NE_SE);

		RefreshMarker(Marker_SE_SW, Dist_SE_SW, Radius);
		RefreshMarker(Marker_SW_NW, Dist_SW_NW, Radius);
		RefreshMarker(Marker_NW_NE, Dist_NW_NE, Radius);
		RefreshMarker(Marker_NE_SE, Dist_NE_SE, Radius);
	}

	if ((Dist_SE_SW < Radius) ||
		(Dist_SW_NW < Radius) ||
		(Dist_NW_NE < Radius) ||
		(Dist_NE_SE < Radius))
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


Vector3f QTree::ClosestPointOnSegment(Vector3f const &LineStart, Vector3f const &LineEnd, Vector3f const &InPoint)
{
	Vector3f AB = LineEnd - LineStart;
	Vector3f AP = InPoint - LineStart;

	// -1.0f to 1.0f amount of either vector projected on the other (order doesn't matter).
	GLfloat Dot_AB_AP = AB.normalized().dot(AP.normalized());

	// Actual projection length is either vector's length * the previous result.
	GLfloat ProjLength = Dot_AB_AP > 0.0f ? Dot_AB_AP * AP.norm() : 0.0f;
	
	if (ProjLength > AB.norm())
	{
		ProjLength = AB.norm();
	}
	
	Vector3f Projection = AB.normalized() * ProjLength;
	Vector3f ClosestPoint = LineStart + Projection;

	return ClosestPoint;
}

void QTree::RefreshMarker(MatterCircle *Marker, GLfloat const Dist, GLfloat const Radius)
{
	GLfloat ScaleMod = Dist / MarkerFadeDist;
	ScaleMod = ScaleMod > 1.0f ? 1.0f : ScaleMod;
	ScaleMod = 1.0f - ScaleMod;
	Marker->SetScale(ScaleMod * MarkerScale);

	if (Dist < Radius)
	{
		Marker->SetColors(MarkerColor_ON, MarkerColor_ON);
	}
	else
	{
		Marker->SetColors(MarkerColor_OFF, MarkerColor_OFF);
	}
}
