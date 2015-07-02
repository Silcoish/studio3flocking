#include "SFML/Graphics.hpp"
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

using namespace std;
using namespace qgf;
using namespace kf;
using namespace sf;

int main()
{
	// This sets the working directory to where the executable is.
	initDirectory();

	// Start the KF log system, tell it to log to the console and a file.
	Log::getDefault().addCout();
	Log::getDefault().addFile("base.log");
	kf_log("This is the base qgf2d project");

	//start the app
	FA::App::Instance().Init();

	//add some place holder agents, preds and obs to demonstrate functionality

#pragma region prey
	FA::AgentFactory::Params p;

	p.spawnAt.SetRadius(Range(0,30));
	p.accel = Range(500, 1000);
	p.startingVel.SetRadius(Range(-2, 2));

	p.size = Range(1, 1);
	p.mass = Range(4, 4);
	p.spec.SetAvoidance(FA::SensorSpecification(25, 25, 180, 180, 80, 80));
	p.spec.SetGrouping(FA::SensorSpecification(50, 50, 120, 120, 50, 50));
	p.spec.SetHeading(FA::SensorSpecification(30, 30, 90, 90, 50, 50));
	p.spec.SetSpeed(FA::SensorSpecification(40, 40, 70, 70, 20, 20));
	p.spec.SetFlee(FA::SensorSpecification(40, 40, 180, 180, 220, 220));
	
	for (int i = 0; i < 500; ++i)
	{
		FA::App::Instance().GetAgentFactory()->Create(p);
	}
#pragma endregion

#pragma region pred
	p.isPrey = false;
	p.spec.SetChase(FA::SensorSpecification(240, 240, 90, 90, 1000, 1000));
	
	for (int i = 0; i < 3; i++)
	{
		FA::App::Instance().GetAgentFactory()->Create(p);
	}

#pragma endregion

#pragma region rect_obs
	FA::ObstacleFactory::Params op;

	op.size = kf::Vector2f(20, 20);
	op.pos = kf::Vector2f(120, 0);
	op.rotation = 25 * DEG2RAD;
	op.type = FA::Obstacle::Type::Rect;

	FA::App::Instance().GetObstacleFactory()->Create(op);

#pragma endregion

#pragma region circ_obs
	op.pos = kf::Vector2f(230, 0);
	op.type = FA::Obstacle::Type::Circ;
	FA::App::Instance().GetObstacleFactory()->Create(op);

#pragma endregion

	//let the simulation go
	FA::App::Instance().Run();

	return 0;
}
