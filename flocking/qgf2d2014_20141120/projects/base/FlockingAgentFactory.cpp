#include "FlockingAgentFactory.hpp"
#include "Range.hpp"
#include "App.hpp"

FA::FlockingAgent* FA::AgentFactory::Create(const FA::AgentFactory::Params& p)const
{
	FlockingAgent* retval = new FlockingAgent();

	//generate from rands
	float size = p.size.Rand();
	float mass = p.mass.Rand();
	retval->SetSensorArray(p.spec.Generate());
	
	//set up dynamic body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 0.0f);
	bodyDef.fixedRotation = true;
	b2Body* body = mPhysWorld->CreateBody(&bodyDef);
	body->SetLinearDamping(0.1f);

	b2CircleShape c;
	c.m_radius = size;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &c;
	fixtureDef.density = mass / (b2_pi *size*size);
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	retval->SetBody(body);

	//make centre circle
	retval->SetCentreCircle(new sf::CircleShape(size));
	retval->GetCentreCircle()->setOrigin(size, size);

	//set vals
	retval->SetPosition(p.spawnAt.Rand());
	retval->SetVelocity(p.startingVel.Rand());
	retval->SetMaxAccel(p.accel.Rand());
	retval->SetIsPrey(p.isPrey);


	FA::App::Instance().GetScene()->AddAgent(retval);

	return retval;
}

void FA::AgentFactory::Destroy(FA::FlockingAgent* fa)const
{
	FA::App::Instance().GetScene()->EraseAgent(fa);

	//deleting body tasks fixtures with it
	mPhysWorld->DestroyBody(fa->GetBody());
	fa->SetBody(nullptr);

	//flocking agent didn't create this so we clean it up
	delete fa->GetCentreCircle();
	fa->SetCentreCircle(nullptr);

	delete fa;
}