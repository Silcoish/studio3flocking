#include "GUI.h"

GUI::GUI(sf::RenderWindow* renderWindow)
{
	if (renderWindow == NULL)
	{
		std::cout << "renderWindow was null, aborting GUI creation";
		return;
	}
	std::cout << "DONE" << std::endl;

}

GUI::~GUI()
{

}