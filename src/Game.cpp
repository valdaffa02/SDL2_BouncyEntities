#include "Game.h"
#include "ECS/Components.h"
#include "AssetManager.h"
#include "Vector2D.h"

#include <iostream>
//#include <string>

Manager manager;
SDL_Renderer* Game::renderer = nullptr;

SDL_Event Game::event;

AssetManager* Game::assets = new AssetManager(&manager);

int Game::mouse_x = 0;
int Game::mouse_y = 0;



auto& label(manager.addEntity());
auto& startText(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flag = 0;

	if (fullscreen)
	{
		flag = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flag);

		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created!" << std::endl;
		}

		isRunning = true;
		count = 0;
	}
	else {
		isRunning = false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error: TTF failed to initialize" << std::endl;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
	{
		std::cout << "Error: Mix OpenAudio Failed!" << std::endl;
	}

	//adding textures to assetManager
	assets->AddTexture("red_ball", "assets/red_ball.png");
	assets->AddTexture("yellow_ball", "assets/yellow_ball.png");
	assets->AddTexture("green_ball", "assets/green_ball.png");
	assets->AddTexture("blue_ball", "assets/blue_ball.png");
	assets->AddTexture("black_ball", "assets/black_ball.png");

	assets->AddFont("arial_16", "assets/arial.ttf", 16);
	assets->AddFont("arial_32", "assets/arial.ttf", 32);

	assets->AddFxWav("pop_fx_01", "assets/pop_01.wav");
	assets->AddFxWav("pop_fx_02", "assets/pop_02.wav");

	SDL_Color black = { 0, 0, 0, 255 };
	
	label.addComponent<UILabel>(10, 10, "test", "arial_16", black);
	startText.addComponent<UILabel>(260, 308, "Click Anywhere!", "arial_32", black);

	label.addGroup(Game::groupText);
	startText.addGroup(Game::groupText);
}

auto& floaters(manager.getGroup(Game::groupFloater));
auto& texts(manager.getGroup(Game::groupText));

void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&Game::mouse_x, &Game::mouse_y);
			
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
				case SDL_BUTTON_LEFT:
					addFloaty();
					break;
				case SDL_BUTTON_RIGHT:
					if (!floaters.empty())
					{
						floaters.front()->destroy();
						assets->PlayFxWav("pop_fx_02");
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void Game::update()
{
	count++;
	//std::cout << count << std::endl;
	manager.refresh();
	manager.update();

	label.getComponent<UILabel>().SetLabelText("Number of floaters: " + std::to_string(floaters.size()), "arial_16");
	//std::cout << std::to_string(floaters.size()) << std::endl;

	if (floaters.size() == 1)
	{
		startText.destroy();
	}

	for (auto& floaty : floaters)
	{
		if (floaty->getComponent<TransformComponent>().position.x > (800 - 32))
		{
			floaty->getComponent<TransformComponent>().velocity.x = -1;
		}
		if (floaty->getComponent<TransformComponent>().position.x < 0)
		{
			floaty->getComponent<TransformComponent>().velocity.x = 1;
		}
		if (floaty->getComponent<TransformComponent>().position.y > (640 - 32))
		{
			floaty->getComponent<TransformComponent>().velocity.y = -1;
		}
		if (floaty->getComponent<TransformComponent>().position.y < 0)
		{
			floaty->getComponent<TransformComponent>().velocity.y = 1;
		}
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);

	for (auto& floaty : floaters)
	{
		floaty->draw();
		//std::cout << "floater drawn!" << std::endl;
	}

	for (auto& text : texts)
	{
		text->draw();
	}

	SDL_RenderPresent(renderer);
}
void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::addFloaty()
{
	//std::cout << "LMB Clicked! - " << mouse_x << ", " << mouse_y << std::endl;
	auto& floaty(manager.addEntity());

	floaty.addComponent<TransformComponent>(static_cast<float>(Game::mouse_x), static_cast<float>(Game::mouse_y), 32, 32, 1);

	switch (floaters.size() % 4)
	{
		case 0:
			floaty.addComponent<SpriteComponent>("red_ball");
			floaty.getComponent<TransformComponent>().velocity.x = 1;
			floaty.getComponent<TransformComponent>().velocity.y = 1;
			break;
		case 1:
			floaty.addComponent<SpriteComponent>("green_ball");
			floaty.getComponent<TransformComponent>().velocity.x = -1;
			floaty.getComponent<TransformComponent>().velocity.y = 1;
			break;
		case 2:
			floaty.addComponent<SpriteComponent>("blue_ball");
			floaty.getComponent<TransformComponent>().velocity.x = -1;
			floaty.getComponent<TransformComponent>().velocity.y = -1;
			break;
		case 3:
			floaty.addComponent<SpriteComponent>("yellow_ball");
			floaty.getComponent<TransformComponent>().velocity.x = 1;
			floaty.getComponent<TransformComponent>().velocity.y = -1;
			break;
		default:
			break;
	}
	floaty.addGroup(Game::groupFloater);
	assets->PlayFxWav("pop_fx_01");
}


bool Game::isRunning;