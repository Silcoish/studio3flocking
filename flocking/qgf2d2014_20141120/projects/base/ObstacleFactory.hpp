#ifndef OBSTACLEFACTORY_H
#define OBSTACLEFACTORY_H

//lazy include
#include "Obstacle.hpp"

namespace FA
{
	/**
		Factory for creating obstacles from settings. Automatically adds to current scene
	*/
	class ObstacleFactory
	{
	public:
		struct Params
		{
			Obstacle::Type type;
			kf::Vector2f pos, size;
			float rotation = 0.0f;
		};
	public:
		ObstacleFactory(b2World* w) :mPhysWorld(w){}
		~ObstacleFactory(){}

		Obstacle* Create(Params& p);

		void Destroy(Obstacle* o);

	private:
		b2World* mPhysWorld;
	};
}

#endif//OBSTACLEFACTORY_H