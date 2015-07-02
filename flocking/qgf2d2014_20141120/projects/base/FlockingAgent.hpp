#ifndef FLOCKINGAGENT_H
#define FLOCKINGAGENT_H

//HACK: only include the things you actually need
#include <iostream>
#include <omp.h>
#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <kf\kf_vector.h>

#include "FlockingAgentSpecification.hpp"

namespace FA
{
	/**
		Houses data for a sensor duing the update process
	*/
	struct SesnsorCache
	{
	public:
		SesnsorCache() :count(0){}

		void Accum(kf::Vector2f& v){ vec += v; ++count; }
		
		kf::Vector2f Average()
		{
			if (count == 0)
				return kf::Vector2f();

			return vec / (float)count; 
		}

		kf::Vector2f vec;
		size_t count;
	};

	/**
		Class to represent a single flocking agent within the simulation.
	*/
	class FlockingAgent : public sf::Drawable{
	public:
		FlockingAgent() :mMaxAccel(0), mIsPrey(true), mIsSuper(false){}
		virtual ~FlockingAgent(){}

		//called once before any update calls per frame, used to prepare data and reset caches
		virtual void Prepare();

		//called potentially many times per frame, used to accumuate flocking data
		//N.B. The agent themself does not and should not track which agents it has already
		//	flocked with. So while this can and should be called multiple times with different 
		//	groups of agents per frame no 1 agent should exist within more than 1 of those groups
		virtual void Update(float dt, FlockingAgent** firstFA, size_t count);

		//called once per frame after all updates called, used to wrap up and apply calcs to agent 
		//	from values determined during update calls
		virtual void Finalise(float dt);

		//render this object
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		

		void AddForce(kf::Vector2f& force){ mBody->ApplyForceToCenter(force, true); }

		kf::Vector2f GetPosition() const { return mBody->GetWorldCenter(); }
		void SetPosition(kf::Vector2f& p) const { mBody->SetTransform(p, mBody->GetAngle()); }

		kf::Vector2f GetHeading() const { return kf::Vector2f(mBody->GetLinearVelocity()).normalise(); }
		
		kf::Vector2f GetVelocity() const { return kf::Vector2f(mBody->GetLinearVelocity()); }
		void SetVelocity(kf::Vector2f& vel) { mBody->SetLinearVelocity(vel); }
		
		float GetSpeed() const { return mBody->GetLinearVelocity().Length(); }


	protected:
		PUBLIC_PTR_PROPERTY(b2Body, Body);
		PUBLIC_DATA_PROPERTY(SensorArray, SensorArray);
		PUBLIC_DATA_PROPERTY(float, MaxAccel);
		PUBLIC_DATA_PROPERTY(bool, IsPrey);
		PUBLIC_DATA_PROPERTY(bool, IsSuper);
		PUBLIC_PTR_PROPERTY(sf::CircleShape, CentreCircle);
		PUBLIC_PTR_PROPERTY(sf::ConvexShape, GroupingFan);

		SesnsorCache mHeadingCache, mGroupingCache, mAvoidanceCache, mSpeedCache, mFleeCache, mChaseCache;

	private:
		void PredChaseUpdate(int i, float dt, FlockingAgent** firstFA, size_t count);
		void PreyFleeUpdate(int i, float dt, FlockingAgent** firstFA, size_t count);
		void PreyPreyUpdate(int i, float dt, FlockingAgent** firstFA, size_t count);
	};
}


#endif//FLOCKINGAGENT_H