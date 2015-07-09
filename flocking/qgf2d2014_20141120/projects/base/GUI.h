#ifndef FLOCKING_GUI_H
#define FLOCKING_GUI_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "TGUI/TGUI.hpp"

class GUI
{
private:
	tgui::Gui g;

public:
	GUI(sf::RenderWindow* renderWindow);
	~GUI();

	void HandleEvent(sf::Event* e);
	void HandleDraw(sf::RenderWindow* renderWindow);
};

#endif
