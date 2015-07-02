#include "Obstacle.hpp"

void FA::Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	static sf::RectangleShape rect;
	static sf::CircleShape circ;

	switch (mType)
	{
	case FA::Obstacle::Rect:
	{
		auto s = mObsSize;
		rect.setSize(s);
		rect.setOrigin(mObsSize / 2.0f);
		rect.setPosition(GetPosition());
		rect.setRotation(GetRotation() / DEG2RAD);
		target.draw(rect, states);
	}
		break;
	case FA::Obstacle::Circ:
		circ.setRadius(mObsSize.x);
		circ.setOrigin(mObsSize.x, mObsSize.x);
		circ.setPosition(GetPosition());
		target.draw(circ, states);

		break;
	default:
		break;
	}
}