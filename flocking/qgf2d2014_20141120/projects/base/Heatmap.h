#ifndef HEATMAP_H
#define HEATMAP_H

#include <iostream>
#include <vector>
#include <chrono>
#include "SFML/Graphics.hpp"

struct HeatmapData
{
	sf::Vector2i pos;
	sf::Color col;
};

class Heatmap
{
private:
	sf::Image* image;

	//time tracking
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_seconds;
	float waitTime = 1.0f;

	std::string lastSaveFilename = "image.png";

public:
	std::vector<HeatmapData*> birdPositions;
public:
	Heatmap();
	~Heatmap();
	void Update();
	void SetImagePixel(sf::Vector2i* mousePos, sf::Color color);
	void SetImagePixel(const int x, const int y, sf::Color color);
	void SetPos(sf::Vector2i& mousePos, sf::Color color);
	void SetPos(float x, float y, sf::Color color);
	void SaveToFile();
};

#endif