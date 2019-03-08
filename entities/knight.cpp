#include <sdlgl/graphics/resources.h>

#include "knight.h"


#define KEY_P1_MOVE_UP      SDL_SCANCODE_W
#define KEY_P1_MOVE_DOWN    SDL_SCANCODE_S
#define KEY_P1_MOVE_LEFT    SDL_SCANCODE_A
#define KEY_P1_MOVE_RIGHT   SDL_SCANCODE_D
#define KEY_P1_ATTACK       SDL_SCANCODE_PERIOD
#define KEY_P1_BLOCK        SDL_SCANCODE_COMMA

#define ATTACK_TIME (1.0f)
#define H_SPEED (100)
#define V_SPEED (100)


Knight::Knight(Scene *scene, float x, float y, SDL_Color color) :
    PhysicalEntity(scene, x, y, 0, 0),
    color(color),
    state(IDLE),
    facing(RIGHT) {

    Resources *resources = scene->get_graphics()->get_resources();

    sprite_idle = resources->get_sprite("knight_idle");
    sprite_walk = resources->get_sprite("knight_walk");
    sprite_attack = resources->get_sprite("knight_attack");
    w = sprite_idle.get_width();
    h = sprite_idle.get_height();

}


void Knight::handle_inputs(Inputs *inputs) {

    float delta = scene->get_delta();

    if (state == WALKING) {
        state = IDLE;
    }
    if (state == ATTACKING) {
        attack_timer -= delta;
        if (attack_timer <= 0) {
            state = IDLE;
        } else {
            return;
        }
    }
    if (inputs->is_key_down(KEY_P1_MOVE_UP)) {
        state = WALKING;
        y -= V_SPEED * delta;
    } else if (inputs->is_key_down(KEY_P1_MOVE_DOWN)) {
        state = WALKING;
        y += V_SPEED * delta;
    }

    if (inputs->is_key_down(KEY_P1_MOVE_LEFT)) {
        state = WALKING;
        facing = LEFT;
        x -= H_SPEED * delta;
    } else if (inputs->is_key_down(KEY_P1_MOVE_RIGHT)) {
        state = WALKING;
        facing = RIGHT;
        x += H_SPEED * delta;
    }

    if (inputs->is_key_down_event(KEY_P1_ATTACK)) {
        if(state != ATTACKING) {
            state = ATTACKING;
            attack_timer = ATTACK_TIME;
            sprite_attack.reset();
        }
    }

}


void Knight::update() {

    float delta = scene->get_delta();
    handle_inputs(scene->get_inputs());
    clamp();

}

void Knight::render() {
    float delta = scene->get_delta();
    switch (state) {
        case IDLE:
            if (facing == RIGHT) {
                sprite_idle.draw(scene->get_graphics()->get_renderer(), x, y, delta);
            } else {
                sprite_idle.draw(scene->get_graphics()->get_renderer(), x, y, 0.0f, true, false, delta);
            }
            break;
        case WALKING:
            if (facing == RIGHT) {
                sprite_walk.draw(scene->get_graphics()->get_renderer(), x, y, delta);
            } else {
                sprite_walk.draw(scene->get_graphics()->get_renderer(), x, y, 0.0f, true, false, delta);
            }
            break;
        case ATTACKING:
            if (facing == RIGHT) {
                sprite_attack.draw(scene->get_graphics()->get_renderer(), x, y, delta);
            } else {
                sprite_attack.draw(scene->get_graphics()->get_renderer(), x, y, 0.0f, true, false, delta);
            }
            break;
    }
}