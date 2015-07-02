#ifndef RANGE_H
#define RANGE_H

#include "Macros.hpp"
#include "kf\kf_vector2.h"
#include  <cstdlib>

/**
	Class for generating a value between a given range
*/
class Range
{
public:
	Range() :mMin(0), mMax(1){}
	Range(float min, float max) :mMin(min), mMax(max){}

	float Rand()const 
	{ 
		return (float)((rand() / (double)RAND_MAX)*(mMax - mMin) + mMin); 
	}
	
	PUBLIC_DATA_PROPERTY(float, Min);
	PUBLIC_DATA_PROPERTY(float, Max)
};

/**
	Generator for random points on a circlular disc. Uniform in cylindrical space not euclidean
*/
class Circle
{
public:
	Circle(){}
	Circle(const Range& r, const kf::Vector2f& c) :mRadius(r), mCentre(c){}
	Circle(float rmin, float rmax, const kf::Vector2f& c) :mRadius(rmin, rmax), mCentre(c){}

	kf::Vector2f Rand()const
	{
		kf::Vector2f pos = mCentre;
		kf::Vector2f offset(mRadius.Rand(), 0);
		offset.rotate(Range(0, TAU).Rand());

		return pos + offset;
	}

	PUBLIC_DATA_PROPERTY(Range, Radius);
	PUBLIC_DATA_PROPERTY(kf::Vector2f, Centre);
};

#endif//RANGE_H