#pragma once

#include <vector>
#include <map>
#include <iostream>

#define TICKS_TO_MS(ticks) static_cast<float>(static_cast<double>(ticks))

using namespace std;

class GameStatsFrame; // Forward declaration

class GameStatsStatBase
{
public:
	GameStatsStatBase() {};
	~GameStatsStatBase() {};
};

template <typename T>
class GameStatsStat : public GameStatsStatBase
{
public:
	GameStatsStat(string InName, T InValue)
	{
		Name = InName;
		Value = InValue;
	}

	string Name;
	T Value;
	GameStatsStat* pParent = nullptr;
	vector<GameStatsStat*> pChildren;


	void Set(T InValue)
	{
		Value = InValue;
	}

	void Increment(T InValue)
	{
		Value += InValue;
	}

	void WriteValue(ofstream* stream)
	{
		*stream << Value << ",";
	}
	
	void WriteName(ofstream* stream)
	{
		*stream << Name.c_str() << ",";
	}
};

class GameStatsFrame
{
public:
	GameStatsFrame();
	~GameStatsFrame();

	GameStatsStat<int>* Index;
	GameStatsStat<float>* Ms_Total;
	GameStatsStat<float>* Ms_Draw;
	GameStatsStat<float>* Ms_SetUniforms;
	GameStatsStat<float>* Ms_Tick;
	GameStatsStat<float>* Ms_TickAnimation;
	GameStatsStat<float>* Ms_DynamicCastMatter;
	GameStatsStat<float>* Ms_BuildParameters;
	GameStatsStat<float>* Ms_ComputeModelMatrix;
	GameStatsStat<float>* Ms_GenerateMesh;
	GameStatsStat<float>* Ms_GenerateMesh_Positions;
	GameStatsStat<float>* Ms_GenerateMesh_Normals;
	GameStatsStat<float>* Ms_GenerateMesh_Colors;
	GameStatsStat<float>* Ms_GenerateMesh_Indices;
	GameStatsStat<float>* Ms_Swap;

	GameStatsStat<int>* OcclusionQueries_Tested;
	GameStatsStat<int>* OcclusionQueries_Collected;
	GameStatsStat<int>* OcclusionSamples_Passed;

	GameStatsStat<float>* Ms_Test; //For quick any testing
	GameStatsStat<float>* Ms_Test2; //For quick any testing
};

class GameStats
{
public:
	GameStats();
	~GameStats();

	// Singleton accessor
	static GameStats* GetGameStats();

	/* AddFrame
	/ Starts a new frame.
	*/
	void AddFrame();

	/* WriteToFile
	/ Writes the stats to file.
	*/
	void WriteToFile(string FilePath);

	GameStatsFrame* pThisFrame;
	GameStatsFrame* pLastFrame;

	/* NumFrames
	/ Number of frames created so far.
	*/
	int NumFrames();

private:
	static GameStats* pGameStats;

	vector<GameStatsFrame*> Frames;
};