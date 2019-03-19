#ifndef STATE_DISPLAY_H
#define STATE_DISPLAY_H

#include <string>
#include <SDL2/SDL.h>

#include "../game/entity.h"
#include "../game/scene.h"
#include "../graphics/graphics.h"

class StateDisplay : public Entity {

	std::string font;
	SDL_Color color;

public:

	StateDisplay(Scene *scene);
	StateDisplay(Scene *scene, std::string font, SDL_Color color);

	void update();
	void render();

};

#endif