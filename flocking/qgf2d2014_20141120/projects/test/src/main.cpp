#include "SFML/Graphics.hpp"
#include "cmath"
#include "kf/kf_log.h"
#include "qgf2d/system.h"
#include "qgf2d/anim.h"

using namespace std;
using namespace qgf;
using namespace kf;
using namespace sf;

int main()
{
	// This sets the working directory to where the executable is.
	initDirectory();

	// Start the KF log system, tell it to log to the console and a file.
	Log::getDefault().addCout();
	Log::getDefault().addFile("base.log");
	kf_log("This is the base qgf2d project");

	// Open a window to draw into. This one is 800 x 600 in 32 bit colour. The string "Base" is the window title.
	RenderWindow window(VideoMode(800, 600, 32), "Base");

	// Load a texture from the data directory.
	Texture *backgroundTexture = new Texture();
	if (!backgroundTexture->loadFromFile("data/sky.jpg"))
	{
		return EXIT_FAILURE;
	}

	// Create a sprite, using the previous texture.
	Sprite *backgroundSprite = new Sprite(*backgroundTexture);
	// Scale the sprite so the texture stretches to fit in the window.
	backgroundSprite->scale(800.0f/backgroundTexture->getSize().x, 600.0f/backgroundTexture->getSize().y);

	// Set up a font for text rendering.
	sf::Font myFont;
	if (!myFont.loadFromFile("data/bluehigh.ttf"))
	{
		return EXIT_FAILURE;
	}

	// Create a text object. This is like a sprite but for text printing.
	sf::Text *text = new sf::Text();
	text->setString("QGF2D");
	text->setFont(myFont);
	text->setPosition(400, 20);

	// The clock is used to keep track of how much time has gone past.
	sf::Clock clock;

	// This is the main game loop. It continues while the window is open.
	while (window.isOpen())
	{
		// Events are things such as keys being pressed, the window closing, etc.
		// There could be several events waiting for us, so use a loop to process them all.
		Event ev;
		while (window.pollEvent(ev))
		{
			if ((ev.type == Event::Closed) || ((ev.type == Event::KeyPressed) && (ev.key.code == Keyboard::Escape)))
			{
				window.close();
				break;
			}
		}

		// deltaT is the amount of time that has gone by since the last frame.
		float deltaT = clock.restart().asSeconds();

		// Clear the window.
		window.clear();
		// Draw the background picture.
		window.draw(*backgroundSprite);
		window.draw(*text);
		// Calling display will make the contents of the window appear on screen (before this, it was kept hidden in the back buffer).
		window.display();
	}

	// Clean up
	delete text;
	delete backgroundSprite;
	delete backgroundTexture;

	return 0;
}
