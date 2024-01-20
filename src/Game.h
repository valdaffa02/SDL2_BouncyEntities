#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <vector>
#include <sstream>

class AssetManager;

class Game
{
private:

	SDL_Window* window;
	static bool isRunning;

public:
	Game();
	~Game();
	
	static int mouse_x, mouse_y;
	
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static AssetManager* assets;

	void init(const char* title, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	void addFloaty();

	bool running() { return isRunning; }

	enum groupLabels : std::size_t
	{
		groupText,
		groupFloater
	};

	int count;

};