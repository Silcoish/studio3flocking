#include "Debug.hpp"

FA::Debug& FA::Debug::Instance()
{
	static Debug inst;
	return inst;
}

void FA::Debug::DrawLine(const kf::Vector2f& start, const kf::Vector2f& end, const sf::Color& col)
{
	mLines.push_back(LineData(start,end, col));
}

void FA::Debug::DrawAll(sf::RenderTarget& rt)
{
	if (!mEnabled)
		goto END;

	sf::Vertex line[2];

	for (auto& l : mLines)
	{
		line[0].position = l.start;
		line[0].color = l.col;
		line[1].position = l.end;
		line[1].color = l.col;
		rt.draw(line, 2, sf::Lines);
	}
	
END:
	mLines.clear();
}