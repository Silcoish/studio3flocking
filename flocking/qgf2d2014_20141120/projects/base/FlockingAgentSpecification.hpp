#ifndef FLOCKINGAGENTSPECIFICATION_H
#define FLOCKINGAGENTSPECIFICATION_H

#include "Range.hpp"

/**
	Settings and helpers for generating sensor settings for flocking agents
*/

namespace FA
{
	/**
		Data used as the settings for 1 sensor on a flocking agent
		*/
	class SensorSettings
	{
	public:
		SensorSettings() :SensorSettings(0, 0, 0){}
		SensorSettings(float r, float a, float i) :mRadius(r), mAngle(a), mInfluence(i){}

		PUBLIC_DATA_PROPERTY(float, Radius);
		PUBLIC_DATA_PROPERTY(float, Angle);
		PUBLIC_DATA_PROPERTY(float, Influence);
	};

	/**
		required data for 1 sensor setting to be generated
		*/
	class SensorSpecification
	{
	public:
		SensorSpecification(){}
		SensorSpecification(Range r, Range a, Range i) :mRadius(r), mAngle(a), mInfluence(i){}
		SensorSpecification(float rmin, float rmax, float amin, float amax, float imin, float imax) :
			mRadius(rmin, rmax), mAngle(amin, amax), mInfluence(imin, imax){}

		SensorSettings Generate()const
		{
			return SensorSettings(mRadius.Rand(), mAngle.Rand(), mInfluence.Rand());
		}

		PUBLIC_DATA_PROPERTY(Range, Radius);
		PUBLIC_DATA_PROPERTY(Range, Angle);
		PUBLIC_DATA_PROPERTY(Range, Influence);
	};

	/**
		Data required for 1 agent to have its full suite of sensors
		*/
	class SensorArray
	{
	public:
		SensorArray(){}
		SensorArray(SensorSettings h, SensorSettings g, SensorSettings a, SensorSettings s, SensorSettings f, SensorSettings c) :
			mHeading(h), mGrouping(g), mAvoidance(a), mSpeed(s), mFlee(f), mChase(c){}


		PUBLIC_DATA_PROPERTY(SensorSettings, Heading);
		PUBLIC_DATA_PROPERTY(SensorSettings, Grouping);
		PUBLIC_DATA_PROPERTY(SensorSettings, Avoidance);
		PUBLIC_DATA_PROPERTY(SensorSettings, Speed);
		PUBLIC_DATA_PROPERTY(SensorSettings, Flee);
		PUBLIC_DATA_PROPERTY(SensorSettings, Chase);
	};

	/**
		Data required to generate 1 SensorArray
	*/
	class SensorArraySpecification
	{
	public:
		SensorArraySpecification(){}
		SensorArraySpecification(SensorSpecification h, SensorSpecification g, SensorSpecification a, SensorSpecification s, SensorSpecification(f), SensorSpecification c) :
			mHeading(h), mGrouping(g), mAvoidance(a), mSpeed(s), mFlee(f), mChase(c){}


		SensorArray Generate()const
		{
			return SensorArray(mHeading.Generate(), mGrouping.Generate(), mAvoidance.Generate(), mSpeed.Generate(), mFlee.Generate(), mChase.Generate());
		}

		PUBLIC_DATA_PROPERTY(SensorSpecification, Heading);
		PUBLIC_DATA_PROPERTY(SensorSpecification, Grouping);
		PUBLIC_DATA_PROPERTY(SensorSpecification, Avoidance);
		PUBLIC_DATA_PROPERTY(SensorSpecification, Speed);
		PUBLIC_DATA_PROPERTY(SensorSpecification, Flee);
		PUBLIC_DATA_PROPERTY(SensorSpecification, Chase);
	};
}


#endif//FLOCKINGAGENTSPECIFICATION_H