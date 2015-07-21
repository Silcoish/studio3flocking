#ifndef FLOCKINGAGENTFACTORY_H
#define FLOCKINGAGENTFACTORY_H

//HACK: lazy include
#include "FlockingAgentSpecification.hpp"
#include "FlockingAgent.hpp"

namespace FA
{
	/**
		Factory for creating flocking agents, hides the mess required to wire everything together
	*/
	class AgentFactory
	{
	public:
		struct Params
		{
			Range mass;
			Range size;
			float collisionRange;
			bool isPrey = true;
			Circle spawnAt,startingVel;
			Range accel;
			SensorArraySpecification spec;
		};
	public:
		AgentFactory(b2World* w) :mPhysWorld(w){}
		~AgentFactory(){}

		//creates and configures a flocking agent from given spec
		//	also adds to scene
		FlockingAgent* Create(const Params& p)const;

		void Destroy(FlockingAgent*)const;

	private:
		b2World* mPhysWorld;
	};
}

#endif//FLOCKINGAGENTFACTORY_H