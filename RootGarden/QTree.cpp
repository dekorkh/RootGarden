#include "QTree.h"
#include "MatterTypes.h"
#include "SceneComponent.h"

QTree::QTree(float inX, float inY, float inLength, QTree const *inParent, SceneComponent *inOwner)
	:
	X(inX),
	Y(inY),
	Length(inLength),
	Parent(inParent),
	Owner(inOwner),
	isLeaf(true)
{
	MatterRectangle* rect = new MatterRectangle();
	rect->SetScale(Vector3f::Constant(Length * 0.5f));
	
	Vector3f NewPosition;
	NewPosition << inX, inY, 0.0f;
	rect->SetPosition(NewPosition);

	Vector4f NewColor;
	float R = static_cast<float>(rand() % 255) / 255.0f;
	float G = static_cast<float>(rand() % 255) / 255.0f;
	float B = static_cast<float>(rand() % 255) / 255.0f;
	NewColor << R, G, B, 0.25f;
	rect->SetColor(NewColor);

	inOwner->AddChild(rect);
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

void QTree::Add(Matter const &InNode)
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
		Matter const *Nodes[2] = { Node, &InNode };
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