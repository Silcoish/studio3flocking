#include "GUI.h"

GUI::GUI(sf::RenderWindow* renderWindow)
{
	if (renderWindow == NULL)
	{
		std::cout << "renderWindow was null, aborting GUI creation";
		return;
	}
	
	tgui::Gui gui(*renderWindow);

	tgui::Button::Ptr button(g);
	//button->load(THEME_CONFIG_FILE);
	button->setPosition(40, 25);
	button->setText("Quit");
	button->setCallbackId(1);
	button->bindCallback(tgui::Button::LeftMouseClicked);
	button->setSize(300, 40);

}

GUI::~GUI()
{

}

/*void GUI::HandleEvent(sf::Event* e)
{
	g.handleEvent(*e);
}

void GUI::HandleDraw(sf::RenderWindow* renderWindow)
{
	g.draw(renderWindow);
}*/