#include "App.hpp"
#include "Debug.hpp"

const std::string WINDOW_TITLE = "Flocking Agents - ";

FA::App& FA::App::Instance()
{
	static App app;
	return app;
}

bool FA::App::Init()
{
	if (mIsInit)
		return true;
	
	//create window, worlds, factories, etc.
	mWindow = new sf::RenderWindow(sf::VideoMode(800, 800, 32), WINDOW_TITLE, sf::Style::Close);
	mGui = new GUI(mWindow);
	mWindow->setVerticalSyncEnabled(false);
	mClock = new sf::Clock();
	mPhysWorld = new b2World(b2Vec2_zero);
	mAgentFactory = new FA::AgentFactory(mPhysWorld);
	mObstacleFactory = new FA::ObstacleFactory(mPhysWorld);
	mScene = new FA::Scene();


	mIsInit = true;
	return mIsInit;
}

void FA::App::Run()
{
	if (mIsRunning) 
		return;

	mIsRunning = true;

	sf::Clock frameRateClock;

	int count = 0;

	while (mWindow->isOpen())
	{
		// Events are things such as keys being pressed, the window closing, etc.
		// There could be several events waiting for us, so use a loop to process them all.
		sf::Event ev;
		while (mWindow->pollEvent(ev))
		{
			if ((ev.type == sf::Event::Closed) || ((ev.type == sf::Event::KeyPressed) && (ev.key.code == sf::Keyboard::Escape)))
			{
				mWindow->close();
				break;
			}

			mGui->HandleEvent(&ev);

			if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Key::Pause)
				Debug::Instance().SetEnabled(!Debug::Instance().GetEnabled());
		}

		// deltaT is the amount of time that has gone by since the last frame.
		float deltaT = std::min(mClock->restart().asSeconds(), 0.1f);

		//step scene
		mScene->Update(deltaT);

		//default physics steps recommended by box2d
		mPhysWorld->Step(deltaT, 6, 2);

		// Clear the window.
		mWindow->clear();

		//render scene
		mScene->Render(*mWindow);
		mGui->HandleDraw(mWindow);
		

		Debug::Instance().DrawAll(*mWindow);

		// Calling display will make the contents of the window appear on screen (before this, it was kept hidden in the back buffer).
		mWindow->display();
		if (frameRateClock.getElapsedTime().asSeconds() > 0.1f)
		{
			deltaT = roundf(1 / deltaT);
			int frames = deltaT;
			mWindow->setTitle(WINDOW_TITLE + std::to_string(frames));
			frameRateClock.restart();
		}
	}

	std::vector<FlockingAgent*> agents;
	mScene->AgentListCopy(agents);
	for (auto *a : agents)
	{
		if (a->GetIsPrey())
			Stats::getInstance().lifeTimes.push_back(a->mLifeTime);
	}
}

FA::App::~App()
{
	//not really needed as we're a program lifetime singleton but it's better to be sure

	//since we are taking it all down with us we could optimise this but lets be thorough

	//get a copy
	std::vector<FlockingAgent*> agents;
	mScene->AgentListCopy(agents);
	//clear so factory destroy doesn't have to remove 1 at a time as they are ALL dying
	mScene->ClearAgentList();

	for (auto* a : agents)
		mAgentFactory->Destroy(a);

	agents.clear();

	std::vector<Obstacle*> obs;
	mScene->ObstacleListCopy(obs);
	mScene->ClearObstacleList();

	for (auto* o : obs)
		mObstacleFactory->Destroy(o);

	obs.clear();

	DELETE_NULLIFY(mScene);
	DELETE_NULLIFY(mObstacleFactory);
	DELETE_NULLIFY(mAgentFactory);
	DELETE_NULLIFY(mPhysWorld);
	DELETE_NULLIFY(mClock);
	DELETE_NULLIFY(mWindow);
}