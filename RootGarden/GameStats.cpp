#include "GameStats.h"
#include <fstream>

GameStats* GameStats::pGameStats = nullptr;

GameStats::GameStats()
{
	pGameStats = this;
}

GameStats::~GameStats()
{
}

GameStats* GameStats::GetGameStats()
{
	if (pGameStats == nullptr)
	{
		pGameStats = new GameStats();
	}
	return pGameStats;
}

void GameStats::AddFrame()
{
	Frames.insert(Frames.end(), new GameStatsFrame());
	if (Frames.size() == 1)
	{
		pLastFrame = Frames[0]; //If first frame, last frame is first frame.
	}
	else
	{
		pLastFrame = pThisFrame;
	}
	pThisFrame = Frames[Frames.size() - 1];
}

int GameStats::NumFrames()
{
	return Frames.size();
}

void GameStats::WriteToFile(string FilePath)
{
	ofstream* stream = new ofstream(FilePath);
	
	Frames[0]->Index->WriteName(stream);
	Frames[0]->Ms_Total->WriteName(stream);
	Frames[0]->Ms_Draw->WriteName(stream);
	Frames[0]->Ms_SetUniforms->WriteName(stream);
	Frames[0]->Ms_Tick->WriteName(stream);
	Frames[0]->Ms_TickAnimation->WriteName(stream);
	Frames[0]->Ms_DynamicCastMatter->WriteName(stream);
	Frames[0]->Ms_BuildParameters->WriteName(stream);
	Frames[0]->Ms_ComputeModelMatrix->WriteName(stream);
	Frames[0]->Ms_GenerateMesh->WriteName(stream);
	Frames[0]->Ms_GenerateMesh_Positions->WriteName(stream);
	Frames[0]->Ms_GenerateMesh_Colors->WriteName(stream);
	Frames[0]->Ms_GenerateMesh_Indices->WriteName(stream);
	Frames[0]->Ms_Test->WriteName(stream);
	
	*stream << "\n";

	for (auto& Frame : Frames)
	{
		Frame->Index->WriteValue(stream);
		Frame->Ms_Total->WriteValue(stream);
		Frame->Ms_Draw->WriteValue(stream);
		Frame->Ms_SetUniforms->WriteValue(stream);
		Frame->Ms_Tick->WriteValue(stream);
		Frame->Ms_TickAnimation->WriteValue(stream);
		Frame->Ms_DynamicCastMatter->WriteValue(stream);
		Frame->Ms_BuildParameters->WriteValue(stream);
		Frame->Ms_ComputeModelMatrix->WriteValue(stream);
		Frame->Ms_GenerateMesh->WriteValue(stream);
		Frame->Ms_GenerateMesh_Positions->WriteValue(stream);
		Frame->Ms_GenerateMesh_Colors->WriteValue(stream);
		Frame->Ms_GenerateMesh_Indices->WriteValue(stream);
		Frame->Ms_Test->WriteValue(stream);
		*stream << "\n";
	}
	
	stream->close();

	delete stream;
}

GameStatsFrame::GameStatsFrame()
{
	Index = new GameStatsStat<int>("Index", 0);
	Ms_Total = new GameStatsStat<float>("Ms_Total", 0.0f);
	Ms_Draw = new GameStatsStat<float>("Ms_Draw", 0.0f);
	Ms_SetUniforms = new GameStatsStat<float>("Ms_SetUniforms", 0.0f);
	Ms_Tick = new GameStatsStat<float>("Ms_Tick", 0.0f);
	Ms_TickAnimation = new GameStatsStat<float>("Ms_TickAnimation", 0.0f);
	Ms_DynamicCastMatter = new GameStatsStat<float>("Ms_DynamicCastMatter", 0.0f);
	Ms_BuildParameters = new GameStatsStat<float>("Ms_BuildParameters", 0.0f);
	Ms_ComputeModelMatrix = new GameStatsStat<float>("Ms_ComputeModelMatrix", 0.0f);
	Ms_GenerateMesh = new GameStatsStat<float>("Ms_GenerateMesh", 0.0f);
	Ms_GenerateMesh_Positions = new GameStatsStat<float>("Ms_GenerateMesh_Positions", 0.0f);
	Ms_GenerateMesh_Colors = new GameStatsStat<float>("Ms_GenerateMesh_Colors", 0.0f);
	Ms_GenerateMesh_Indices = new GameStatsStat<float>("Ms_GenerateMesh_Indices", 0.0f);
	Ms_Test = new GameStatsStat<float>("Ms_Test", 0.0f);
}

GameStatsFrame::~GameStatsFrame()
{
}