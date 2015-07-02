#include "ObstacleFactory.hpp"
#include "App.hpp"

FA::Obstacle* FA::ObstacleFactory::Create(Params& p)
{
	Obstacle* retval = new Obstacle(p.type);

	//create a static body
	b2BodyDef bodyDef;
	bodyDef.position.Set(0.0f, 0.0f);
	b2Body* body = mPhysWorld->CreateBody(&bodyDef);

	switch (p.type)
	{
	case FA::Obstacle::Rect:
		{
			b2PolygonShape box;
			box.SetAsBox(p.size.x/2, p.size.y/2);
			body->CreateFixture(&box, 0);
		}
		break;
	case FA::Obstacle::Circ:
		{
			b2CircleShape circ;
			circ.m_radius = p.size.x;
			body->CreateFixture(&circ, 0);
		}
		break;
	default:
		break;
	}

	retval->SetBody(body);

	//set required values
	retval->SetPosition(p.pos);
	retval->SetRotation(p.rotation);
	retval->SetObsSize(p.size);


	FA::App::Instance().GetScene()->AddObstacle(retval);

	return retval;
}

void FA::ObstacleFactory::Destroy(Obstacle* o)
{
	FA::App::Instance().GetScene()->EraseObstacle(o);

	//deleting body tasks fixtures with it
	mPhysWorld->DestroyBody(o->GetBody());
	o->SetBody(nullptr);

	delete o;
}