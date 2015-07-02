#include "Scene.hpp"
#include "packetformat.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network.hpp>

FA::Scene::Scene()
{

}

FA::Scene::~Scene()
{

}

void FA::Scene::Update(float dt)
{
	//prep all

	
	for (auto f : mAgents)
	{
		f->Prepare();
	}

	//pass all agents to all
	for (auto f : mAgents)
	{
		f->Update(dt, &mAgents[0], mAgents.size());
	}

	//finalise all
	for (auto f : mAgents)
	{
		f->Finalise(dt);
	}
}

void EncapuslateRect(sf::FloatRect& rect, kf::Vector2f& p)
{
	rect.top = std::min(rect.top, p.y);
	rect.left = std::min(rect.left, p.x);
	rect.width = std::max(rect.width, p.x - rect.left);
	rect.height = std::max(rect.height, p.y - rect.top);
}

void FA::Scene::Render(sf::RenderWindow& rw)
{
	for (auto o : mObstacles)
	{
		rw.draw(*o);
	}

	//sf rect doesn't have an encap so we do our own
	sf::FloatRect r(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (auto f : mAgents)
	{
		EncapuslateRect(r, f->GetPosition());
	}
	//determine view

	//expand so there is a border
	r.left -= 50;
	r.top -= 50;
	r.width += 100;
	r.height += 100;

	//make square
	r.width = std::max(r.width, r.height);
	r.height = r.width;

	//not great but good enough
//	sf::View v(kf::Vector2f(0, 0), kf::Vector2f(100, 100));//r);
	sf::View v(r);


	rw.setView(v);

	//sf::UdpSocket sock;
	//sf::IpAddress ip("10.40.60.35");
	//PacketCircle c;
	//c.type = Packet::e_circle;
	//c.radius = 2;

	for (auto f : mAgents)
	{
		rw.draw(*f);

		/*c.x = f->GetPosition().x + 400;
		c.y = f->GetPosition().y + 400;

		sock.send(&c, sizeof(c), ip, 1300);*/
	}
}