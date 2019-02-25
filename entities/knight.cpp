#include <sdlgl/graphics/resources.h>

#include "knight.h"


#define KEY_P1_MOVE_UP      SDL_SCANCODE_W
#define KEY_P1_MOVE_DOWN    SDL_SCANCODE_S
#define KEY_P1_MOVE_LEFT    SDL_SCANCODE_A
#define KEY_P1_MOVE_RIGHT   SDL_SCANCODE_D
#define KEY_P1_ATTACK       SDL_SCANCODE_PERIOD
#define KEY_P1_BLOCK        SDL_SCANCODE_COMMA

#define ATTACK_DELAY (1.0f)


Knight::Knight(Scene *scene, float x, float y, SDL_Color color) :
    PhysicalEntity(scene, x, y, 0, 0),
    color(color) {

    Resources *resources = scene->get_graphics()->get_resources();

    sprite_idle = resources->get_sprite("knight_idle");
    w = sprite_idle.get_width();
    h = sprite_idle.get_height();

}


void Knight::handle_inputs(Inputs *inputs) {

    float delta = scene->get_delta();

    if (inputs->is_key_down(KEY_P1_MOVE_UP)) {
        
    } else if (inputs->is_key_down(KEY_P1_MOVE_DOWN)) {
        
    }

    if (inputs->is_key_down(KEY_P1_MOVE_LEFT)) {
        
    } else if (inputs->is_key_down(KEY_P1_MOVE_RIGHT)) {
        
    }

    if (inputs->is_key_down_event(KEY_P1_ATTACK)) {
        
    }

}


void Knight::update() {

    handle_inputs(scene->get_inputs());

    float delta = scene->get_delta();

    clamp();

}

void Knight::render() {
    float delta = scene->get_delta();
    sprite_idle.draw(scene->get_graphics()->get_renderer(), this->x, this->y, delta);
}