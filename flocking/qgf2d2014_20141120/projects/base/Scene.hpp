#ifndef FA_SCENE_H
#define FA_SCENE_H

#include <vector>

//HACK lazy include
#include "FlockingAgent.hpp"
#include "Obstacle.hpp"
#include "Heatmap.h"

#include "Macros.hpp"

//pre-dec
namespace sf{ class RenderWindow; }

namespace FA
{
	/**
		Manager that holds and updates flocking agents and obstacles
	*/
	class Scene
	{
	public:
		Scene();
		~Scene(); 

		void Update(float dt);

		void Render(sf::RenderWindow& rw);

		void AddAgent(FlockingAgent* fa){ mAgents.push_back(fa); }
		
		//terrible
		void EraseAgent(FlockingAgent* fa)
		{ 
			auto loc = std::find(mAgents.begin(), mAgents.end(), fa);
			if (loc != mAgents.end())
				mAgents.erase(loc); 
		}	//the horror
		
		
		void AddObstacle(Obstacle* o){ mObstacles.push_back(o); }
		
		//terrible
		void EraseObstacle(Obstacle* o)
		{
			auto loc = std::find(mObstacles.begin(), mObstacles.end(), o);
			if (loc != mObstacles.end())
				mObstacles.erase(loc);
		}	//the horror

		//allows app to clean up
		void AgentListCopy(std::vector<FlockingAgent*>& out) { out = mAgents; }
		void ClearAgentList(){ mAgents.clear(); }
		void ObstacleListCopy(std::vector<Obstacle*>& out) { out = mObstacles; }
		void ClearObstacleList(){ mObstacles.clear(); }

		void UpdateHeatmap(Heatmap* heatmap);

	protected:
		std::vector<FlockingAgent*> mAgents;
		std::vector<Obstacle*> mObstacles;
	};
}

#endif//FA_SCENE_H