#pragma once

#include "../Game.h"
#include "Components.h"

class MouseController : public Component
{
public:
	TransformComponent* transform;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_MOUSEMOTION)
		{
			int x;
			int y;
			SDL_GetMouseState(&x, &y);
			std::cout << x << " : " << y << std::endl;
		}
	}
};