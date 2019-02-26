#ifndef KNIGHT_H
#define KNIGHT_H

#include <SDL2/SDL.h>
#include <sdlgl/game/scene.h>
#include <sdlgl/game/physical_entity.h>
#include <sdlgl/graphics/sprite.h>

class Knight : public PhysicalEntity {

	enum State {
    	IDLE,
    	WALKING
    };

    SDL_Color color;
    State state;
    
    Sprite sprite_idle;
    Sprite sprite_walk;

    void handle_inputs(Inputs *inputs);


public:

    Knight(Scene *scene, float x, float y, SDL_Color color);
    void update();
    void render();

};

#endif