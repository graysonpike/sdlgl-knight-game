#ifndef STATE_DISPLAY_H
#define STATE_DISPLAY_H

#include <string>
#include <SDL2/SDL.h>

#include "sdlgl/game/entity.h"
#include "sdlgl/game/scene.h"
#include "sdlgl/graphics/graphics.h"

#include "../entities/knight.h"


class StateDisplay : public Entity {

	std::string font;
	SDL_Color color;
	Knight *knight;

public:

	StateDisplay(Scene *scene);
	StateDisplay(Scene *scene, std::string font, SDL_Color color);
	StateDisplay(Scene *scene, std::string font, SDL_Color color, Knight *knight);

	void update();
	void render();
	void set_knight(Knight *knight);

};

#endif