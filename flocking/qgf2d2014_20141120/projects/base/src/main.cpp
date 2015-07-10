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

	CSVParser parser;
	if (!parser.LoadFromFile("resources/csv/FlockingData.csv"))
		std::cout << "Failed to load file to parse" << std::endl;

	//add some place holder agents, preds and obs to demonstrate functionality

#pragma region prey
	FA::AgentFactory::Params p;

	int x = 1;
	for (int i = 0; i < parser.mRows.size(); i++)
	{
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

		FA::App::Instance().GetAgentFactory()->Create(p);

		x = 1;
	}
	
#pragma endregion

#pragma region pred
	p.isPrey = false;
	
	for (int i = 0; i < 3; i++)
	{
		FA::App::Instance().GetAgentFactory()->Create(p);
	}

#pragma endregion

#pragma region walls
	FA::ObstacleFactory::Params op;
	op.size = kf::Vector2f(400, 2);
	op.pos = kf::Vector2f(0, -200);
	op.rotation = 0;
	op.type = FA::Obstacle::Type::Rect;

	FA::App::Instance().GetObstacleFactory()->Create(op);

	op.pos = kf::Vector2f(0, 200);

	FA::App::Instance().GetObstacleFactory()->Create(op);

	op.size = kf::Vector2f(2, 400);
	op.pos = kf::Vector2f(-200, 0);

	FA::App::Instance().GetObstacleFactory()->Create(op);

	op.pos = kf::Vector2f(200, 0);

	FA::App::Instance().GetObstacleFactory()->Create(op);
#pragma endregion

	//let the simulation go
	FA::App::Instance().Run();

	return 0;
}
