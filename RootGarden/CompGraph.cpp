#include "CompGraph.h"
#include "Animation_Lerp2Vec3.h"
#include "MatterPlot.h"
#include "MatterLine.h"


CompGraph::CompGraph() :
	X_Neg(-1.6f),
	X_Pos(1.6f),
	Y_Neg(-1.0f),
	Y_Pos(1.0f)
{
	UnitScale << 0.1f, 0.05f, 1.0f;

	Plot = new MatterPlot();
	Plot->SetUnitScale(UnitScale);
	AddChild(Plot);
	
	vector<GLfloat> Positions;
	vector<GLfloat> Positions2;
	
	GLfloat l_trunk = 0.7f;
	GLfloat r_whole = 0.6f;

	for (GLuint i = 0; i < 100; ++i)
	{
		GLfloat x = (static_cast<GLfloat>(i) / 99) * 10.0f;	
		// The Y!
		GLfloat y = x * x * 0.2f;
		
		// v_trunk = v_whole - v_tip

		// we can derive r_tip if we express it as a ratio of l_whole and l_trunk (l_whole will give us the answer really)
		
		//	v_tip = *v_whole* - v_trunk - we actually will know v_trunk so ultimately we just need v_tip
			
		//------------------------------------------------------------------------------------------------------------------------------------------------
		//  v_trunk = v_whole - v_tip																											2 unknowns  
		//	v_trunk = l_whole / 3.0 * Pi * r_whole ^ 2 - l_tip / 3.0 * Pi * r_tip ^ 2															2 unknowns
		//																	r_tip = ( r_whole * ( l_tip / l_whole ) )
		//	v_trunk = l_whole / 3.0 * Pi * r_whole ^ 2 - l_tip / 3.0 * Pi * ( r_whole * ( l_tip / l_whole ) ) ^ 2								2 unknowns
		//            l_whole = ( l_trunk + l_tip )
		//	v_trunk = ( l_trunk + l_tip ) / 3.0 * Pi * r_whole ^ 2 - l_tip / 3.0 * Pi * ( r_whole * ( l_tip / ( l_trunk + l_tip ) ) ) ^ 2		1 unknowns
		//	multiply both sides by ( l_trunk + l_tip )
		//	v_trunk * ( l_trunk + l_tip ) = ( l_trunk + l_tip ) ^ 2 / 3.0 * Pi * r_whole ^ 2 - l_tip / 3.0 * Pi * ( r_whole * l_tip ) ^ 2
		//  divide both sides by 3.0 * Pi
		//	v_trunk * ( l_trunk + l_tip ) / (3.0 * Pi) = 
		//------------------------------------------------------------------------------------------------------------------------------------------------
		
		
		//  v_trunk = v_whole - v_tip							2 unknowns  
		
		//	l_tip = l_whole * ( 1.0 - l_trunk / l_whole )		2 unknowns
		//	l_tip = l_whole * ( 1.0 - r_trunk / r_whole )		2 unknowns


		//	l_tip = l_whole - l_trunk							2 unknowns
		//	l_whole = l_tip + l_trunk							2 unknowns
		//	v_tip = v_whole - v_trunk							2 unknowns
		//	r_tip = r_whole * (l_tip / l_whole)					3 unknowns
		//	r_tip = r_whole * (l_tip / ( l_tip + l_trunk ))		2 unknowns

		
		// If we start with a cone of trunk's length and begin to increase its length at some point the volume of the trunk will be the volume we are given

		GLfloat r_base_whole_sqrd = 1.0f * 1.0f;
		
		// Volume of a cone of a fixed base radius (1.0) as a function of its length
		GLfloat v = x / 3.0f * static_cast<float>(M_PI) * r_base_whole_sqrd;

		Positions.insert(Positions.end(), { x, y, 0.0f });
		Positions2.insert(Positions2.end(), { x, v, 0.0f });
	}


	AddData(Positions);
	AddData(Positions2);
}

CompGraph::~CompGraph()
{
	delete Plot;
}

void CompGraph::Build()
{
	SceneComponent::Build();
}

void CompGraph::AddData(vector<GLfloat> &ContigousXYPositions, Vector4f const &Color)
{
	MatterLine *Data = new MatterLine();
	DataArray.insert(DataArray.end(), Data);
	Data->SetColor(Color);

	vector<GLfloat> ScaledPositions;

	for (size_t i = 0; i < ContigousXYPositions.size(); ++i)
	{
		size_t axis = i % 3;
		GLfloat scaledValue = 0.0f;
		switch (axis)
		{
		case 0: scaledValue = ContigousXYPositions[i] * UnitScale.x(); break;
		case 1: scaledValue = ContigousXYPositions[i] * UnitScale.y(); break;
		case 2: scaledValue = ContigousXYPositions[i] * UnitScale.z(); break;
		}
		
		ScaledPositions.insert(ScaledPositions.end(), scaledValue);
	}

	Data->SetPositions(ScaledPositions);
	AddChild(Data);
}