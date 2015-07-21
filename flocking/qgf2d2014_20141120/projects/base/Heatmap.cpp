#include "Heatmap.h"

#define CANVAS_WIDTH 512
#define CANVAS_HEIGHT 512
#define SPREAD_RADIUS 3;

Heatmap::Heatmap()
{
	image = new sf::Image();
	image->create(CANVAS_WIDTH, CANVAS_HEIGHT, sf::Color::Black);
	lastSaveFilename = "resources/Heatmaps/image" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".png";
	image->saveToFile(lastSaveFilename);

	start = std::chrono::system_clock::now();
}

Heatmap::~Heatmap()
{
	SaveToFile();
}

void Heatmap::Update()
{

	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;

	if (elapsed_seconds.count() >= waitTime)
	{
		for (auto &a : birdPositions)
		{
			SetImagePixel(new sf::Vector2i(a->pos.x, a->pos.y), a->col);
			delete a;
		}

		birdPositions.clear();
		start = std::chrono::system_clock::now();
	}

}

void Heatmap::SetImagePixel(sf::Vector2i* a, sf::Color color)
{
	SetImagePixel(a->x, a->y, color);
}

void Heatmap::SetImagePixel(const int x, const int y, sf::Color color)
{
	if (x < 0 || x >= image->getSize().x || y < 0 || y >= image->getSize().y)
		return;

	if (image->getPixel(x, y) != sf::Color::Black)
	{
		//combine colours
		sf::Color combined = image->getPixel(x, y) + color;
		image->setPixel(x, y, combined);
		return;
	}

	image->setPixel(x, y, color);

	return;
}

void Heatmap::SetPos(float x, float y, sf::Color color)
{
	SetPos(sf::Vector2i(x, y), color);
}

void Heatmap::SetPos(sf::Vector2i& birdPos, sf::Color color)
{
	birdPos.x += CANVAS_WIDTH / 2;
	birdPos.y += CANVAS_HEIGHT / 2;
	if (birdPos.x > 0 && birdPos.x < 512 && birdPos.y > 0 && birdPos.y < 512)
	{
		//Create the struct of data then save
		HeatmapData* data = new HeatmapData();
		data->pos = birdPos;
		data->col = color;
		this->birdPositions.push_back(data);
	}
}

void Heatmap::SaveToFile()
{
	//loop through whole image, pixel by pixel, interperlating if needed

	/*int spreadRadius = 3;
	for (int x = 0; x < CANVAS_WIDTH; x++)
	{
		for (int y = 0; y < CANVAS_HEIGHT; y++)
		{
			if (image->getPixel(x, y).a == 255)
			{
				//unsigned int size = (spreadRadius * 2) * (spreadRadius * 2);
				for (int i = 0; i < spreadRadius * 2; i++)
				{
					for (int j = 0; j < spreadRadius * 2; j++)
					{
						if ((x - spreadRadius) + i > 0 && (x - spreadRadius) + i < CANVAS_WIDTH)
						{
							if ((y - spreadRadius) + j > 0 && (y - spreadRadius) + j < CANVAS_HEIGHT)
							{
								sf::Vector2i distance = 
							}
						}
					}
				}
			}
		}
	}*/
	image->saveToFile(lastSaveFilename);
}