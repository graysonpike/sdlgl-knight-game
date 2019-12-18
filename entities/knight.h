#ifndef KNIGHT_H
#define KNIGHT_H

#include <SDL2/SDL.h>
#include <sdlgl/game/scene.h>
#include <sdlgl/game/physical_entity.h>
#include <sdlgl/graphics/sprite.h>

class Knight : public PhysicalEntity {

    static const char* state_strings[];

	enum State {
    	IDLE,
        WALKING,
        ATTACKING,
        RAISING_TO_BLOCK,
        BLOCKING,
        LOWERING_FROM_BLOCK
    };

    enum Facing {
        RIGHT,
        LEFT
    };

    SDL_Color color;
    State state;
    Facing facing;
    float attack_timer;
    float block_transition_timer;

    std::map<std::string, Sprite> sprites;
    std::map<std::string, Texture> textures;

    void handle_inputs(Inputs *inputs);
    void move(Inputs *inputs, float delta);
    void attack();
    void block();

public:

    Knight(Scene *scene, float x, float y, SDL_Color color);
    void update();
    void render();
    std::string get_state();

};

#endif