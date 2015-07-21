#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <omp.h>
#include "cmath"
#include "kf/kf_log.h"
#include "qgf2d/system.h"
#include "qgf2d/anim.h"
#include <Box2D\Box2D.h>

#include "FlockingAgentFactory.hpp"
#include "ObstacleFactory.hpp"
#include "FlockingAgentSpecification.hpp"
#include "App.hpp"
#include "CSVParser.h"

#include "Stats.h";

using namespace std;
using namespace qgf;
using namespace kf;
using namespace sf;

int main()
{
	int threads = omp_get_max_threads() * 4;
	omp_set_num_threads(threads);
	// This sets the working directory to where the executable is.
	initDirectory();

	// Start the KF log system, tell it to log to the console and a file.
	Log::getDefault().addCout();
	Log::getDefault().addFile("base.log");
	kf_log("This is the base qgf2d project");

	//start the app
	FA::App::Instance().Init();

	//sf::Music music;
	//music.openFromFile("resources/epicmusic.wav");
	//music.play();

#pragma region Parser
	{
		CSVParser parser;
		if (!parser.LoadFromFile("resources/csv/FlockingData.csv"))
			std::cout << "Failed to load file to parse" << std::endl;

		//add some place holder agents, preds and obs to demonstrate functionality

#pragma region birds
		FA::AgentFactory::Params p;

		int x = 0;
		for (int i = 0; i < parser.mRows.size(); i++)
		{
			//std::cout << (parser.GetDataString(i, x));
			int howMany = parser.GetDataFloat(i, x);
			if (parser.GetDataString(i, x).compare("TRUE") == 0)
				p.isPrey = true;
			else
				p.isPrey = false;

			p.spawnAt.SetRadius(Range(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x)));
			p.accel = Range(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x));
			p.startingVel.SetRadius(Range(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x)));

			p.size = Range(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x));
			p.mass = Range(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x));
			
			p.spec.SetAvoidance(FA::SensorSpecification(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x)));
			p.spec.SetGrouping(FA::SensorSpecification(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x)));
			p.spec.SetHeading(FA::SensorSpecification(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x)));
			p.spec.SetSpeed(FA::SensorSpecification(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x)));
			p.spec.SetFlee(FA::SensorSpecification(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x)));
			p.spec.SetChase(FA::SensorSpecification(parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x), parser.GetDataFloat(i, x)));

			p.collisionRange = parser.GetDataFloat(i, x);

			for (int h = 0; h < howMany; h++)
				FA::App::Instance().GetAgentFactory()->Create(p);

			x = 0;
		}
#pragma endregion

		parser.Unload();

		parser.LoadFromFile("resources/csv/ObstacleData.csv");


#pragma region walls
		FA::ObstacleFactory::Params op;

		x = 0;
		for (int j = 0; j < parser.mRows.size(); j++)
		{
			if (parser.GetDataString(j, x).compare("SQUARE") != 0)
				op.type = FA::Obstacle::Type::Rect;
			else
				op.type = FA::Obstacle::Type::Circ;

			op.size = kf::Vector2f(parser.GetDataFloat(j, x), parser.GetDataFloat(j, x));
			op.pos = kf::Vector2f(parser.GetDataFloat(j, x), parser.GetDataFloat(j, x));
			op.rotation = parser.GetDataFloat(j, x);

			FA::App::Instance().GetObstacleFactory()->Create(op);

			x = 0;
		}
#pragma endregion

	}
#pragma endregion

	//let the simulation go
	FA::App::Instance().Run();

	//Save stats then exit
	Stats::getInstance().SaveCSV();

	return 0;
}
