#include "FlockingAgent.hpp"
#include "Debug.hpp"


const float RIGHT_ANGLE = 90 * 0.0174532925f;

//draw lines to indicate sensors
void DrawAgentSensorDebug(const kf::Vector2f& pos, const kf::Vector2f& heading, const FA::SensorSettings& ss, const sf::Color& col)
{
	auto left = heading;
	left.rotate(ss.GetAngle() * DEG2RAD);
	auto right = heading;
	right.rotate(ss.GetAngle() * -DEG2RAD);

	auto gr = ss.GetRadius();
	FA::Debug::Instance().DrawLine(pos, pos + left * gr, col);
	FA::Debug::Instance().DrawLine(pos, pos + right * gr, col);
}


//helper to check if a local offset vector is within a given sensor
bool IsInSensor(FA::FlockingAgent& fa, const FA::SensorSettings& sensor, kf::Vector2f& localOffset)
{
	float l = localOffset.length();
	if (l > sensor.GetRadius()) return false;
	/*
	//the div acounts for non normalised dot
	if ((localOffset.dot(GetHeading())/l) < sensor.GetAngle()) return false;
	*/
	kf::Vector2f localOffDir = localOffset.normalise();
	float angleDif = acos(fa.GetHeading().dot(localOffDir)) / DEG2RAD;

	if (angleDif > sensor.GetAngle()) return false;


	return true;
}

void FA::FlockingAgent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	mCentreCircle->setFillColor(mIsPrey ? sf::Color::Blue : sf::Color::Red);

	auto pos = GetPosition();
	mCentreCircle->setPosition(pos);
	target.draw(*mCentreCircle, states);

	if (!Debug::Instance().GetEnabled()) return;

	auto localheading = GetHeading();
	//heading
	Debug::Instance().DrawLine(pos, pos + localheading * 5, sf::Color::Green);

	DrawAgentSensorDebug(pos, localheading, mSensorArray.GetAvoidance(), sf::Color::Cyan);
	DrawAgentSensorDebug(pos, localheading, mSensorArray.GetGrouping(), sf::Color::Magenta);
	DrawAgentSensorDebug(pos, localheading, mSensorArray.GetHeading(), sf::Color::Red);
	DrawAgentSensorDebug(pos, localheading, mSensorArray.GetSpeed(), sf::Color::Yellow);
	DrawAgentSensorDebug(pos, localheading, mSensorArray.GetFlee(), sf::Color::White);
}


void FA::FlockingAgent::Prepare()
{
	//reset all caches
	mHeadingCache = SesnsorCache();
	mGroupingCache = SesnsorCache();
	mAvoidanceCache = SesnsorCache();
	mSpeedCache = SesnsorCache();
	mFleeCache = SesnsorCache();
	mChaseCache = SesnsorCache();
}


void FA::FlockingAgent::Update(float dt, FlockingAgent** agents, size_t count)
{
	if (mIsPrey)
		mLifeTime += dt;

	mSpeedtotal.total += GetSpeed();
	mSpeedtotal.i++;

	//std::cout << (mSpeedtotal.total / mSpeedtotal.i)<< std::endl;

	#pragma omp parallel for
	for (int i = 0; i < count; ++i)
	{
		//don't try to flock with yourself
		if (agents[i] == this) continue;

		//if preditor and other agent is prey
		if (!mIsPrey && agents[i]->GetIsPrey())
		{
			PredChaseUpdate(i, dt, agents, count);
		}
		//if prey and other agent is predator
		else if (mIsPrey && !agents[i]->GetIsPrey())
		{
			PreyFleeUpdate(i, dt, agents, count);
		}
		//if prey and other agent is prey
		else if (mIsPrey && agents[i]->GetIsPrey())
		{
			PreyPreyUpdate(i, dt, agents, count);
		}
	}
}

void FA::FlockingAgent::PredChaseUpdate(int i, float dt, FlockingAgent** agents, size_t count)
{
	//preditor is dumb right now, just wants to find the closest prey
	kf::Vector2f diff = agents[i]->GetPosition() - GetPosition();

	if (IsInSensor(*this,mSensorArray.GetChase(), diff))
	{
		if (mChaseCache.count == 0 || mChaseCache.vec.length_squared() > diff.length_squared())
		{
			mChaseCache.vec = diff;
			if (diff.length_squared() < mCollisionRange)
			{
				agents[i]->SetIsPrey(false);
				Stats::getInstance().lifeTimes.push_back(agents[i]->mLifeTime);
			}
			else
				mChaseCache.count++;

		}
	}
}

void FA::FlockingAgent::PreyFleeUpdate(int i, float dt, FlockingAgent** agents, size_t count)
{
	//prey wants to find the average pos of its preditors 
	kf::Vector2f diff = agents[i]->GetPosition() - GetPosition();
	float diffLen = diff.length();
	kf::Vector2f diffNorm = diff / diffLen;

	if (IsInSensor(*this,mSensorArray.GetFlee(), diff))
	{
		mFleeCache.Accum(diff);
	}
}

void FA::FlockingAgent::PreyPreyUpdate(int i, float dt, FlockingAgent** agents, size_t count)
{
	//flock my pretties
	kf::Vector2f diff = agents[i]->GetPosition() - GetPosition();
	float diffLen = diff.length();
	kf::Vector2f diffNorm = diff / diffLen;

	if (IsInSensor(*this,mSensorArray.GetAvoidance(), diff))
	{
		//avoid weights them by how close they are and points away from them
		//mAvoidanceCache.Accum(diffNorm * -1 * (mSensorArray.GetAvoidance().GetRadius() - diffLen));

		//more simple method, more strictly in line with boids
		mAvoidanceCache.Accum(diff);
	}

	if (IsInSensor(*this,mSensorArray.GetGrouping(), diff))
		mGroupingCache.Accum(diff);

	if (IsInSensor(*this,mSensorArray.GetHeading(), diff))
		mHeadingCache.Accum(agents[i]->GetHeading());

	//just slide speed in there under x
	if (IsInSensor(*this,mSensorArray.GetSpeed(), diff))
		mSpeedCache.Accum(kf::Vector2f(agents[i]->GetSpeed(), 0));
}

void FA::FlockingAgent::Finalise(float dt)
{
	//move away from the local centre to avoid very tight clumping
	kf::Vector2f avoid = mAvoidanceCache.Average();// *mSensorArray.GetAvoidance().GetInfluence();
	avoid = avoid * -1;//away from that location
	avoid *= mSensorArray.GetAvoidance().GetInfluence();


	//we want to travel to the centre of our local group
	kf::Vector2f group = mGroupingCache.Average();
	if (mGroupingCache.count > 0)
	{
		group = group;//this has already been done in local space -GetPosition();
		group.normalise();
		group *= mSensorArray.GetGrouping().GetInfluence();
	}

	//we want to match the average of all our friends headings
	//	so find the diff between headings
	kf::Vector2f heading;
	kf::Vector2f ourHeading = GetHeading();
	if (mHeadingCache.count > 0)
	{
		kf::Vector2f averageHeading = mHeadingCache.Average().normalise();
		kf::Vector2f ourHeading = GetHeading();

		float averageAng = atan2(averageHeading.x, averageHeading.y), ourAng = atan2(ourHeading.x, ourHeading.y);

		heading = ourHeading;

		float angleDiff = ourAng - averageAng;
		heading.rotate(RIGHT_ANGLE);
		heading = heading * angleDiff * mSensorArray.GetHeading().GetInfluence();
	}

	//we want to travel at the same speed as our friends
	kf::Vector2f spd;
	if (mSpeedCache.count > 0)
	{
		float speedDif = mSpeedCache.Average().x - GetSpeed();
		spd = ourHeading * mSensorArray.GetSpeed().GetInfluence();
	}

	//very much don't be where the preditors are
	kf::Vector2f flee = mFleeCache.Average();
	flee = flee * -1;//away from that location
	flee *= mSensorArray.GetFlee().GetInfluence();

	//go after our prey
	kf::Vector2f chase;
	if(!mIsPrey)
		chase = mChaseCache.vec.normalise();

	chase *= mSensorArray.GetChase().GetInfluence();


	//accumulate and clamp to max impulse
	auto totalForce = avoid + group + heading + spd + flee + chase;
	auto tfl = totalForce.length();
	if (tfl > mMaxAccel)
	{
		totalForce /= tfl;
		totalForce *= mMaxAccel;
	}

	AddForce(totalForce*dt);

}