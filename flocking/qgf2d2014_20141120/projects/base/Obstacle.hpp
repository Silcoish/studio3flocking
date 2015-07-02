#ifndef OBSTACLE_H
#define OBSTACLE_H

//lazy includes
#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include "kf\kf_vector2.h"
#include "Macros.hpp"

namespace FA
{
	/**
		Represents a simple rotated rect or circle in the physics world
	*/
	class Obstacle : public sf::Drawable
	{
	public:
		enum Type
		{
			Rect,
			Circ
		};

		Obstacle(Type t) :mType(t){}
		virtual ~Obstacle(){}


		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		kf::Vector2f GetPosition() const { return mBody->GetWorldCenter(); }
		void SetPosition(kf::Vector2f& p) const { mBody->SetTransform(p, mBody->GetAngle()); }
		float GetRotation() const { return mBody->GetAngle(); }
		void SetRotation(float f) { mBody->SetTransform(GetPosition(), f); }

	protected:
		PUBLIC_DATA_PROPERTY(Type, Type);
		PUBLIC_DATA_PROPERTY(kf::Vector2f, ObsSize);	//x,y for rect, x for radius
		PUBLIC_PTR_PROPERTY(b2Body, Body);
	};
}


#endif//OBSTACLE_H