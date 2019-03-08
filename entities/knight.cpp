#include <sdlgl/graphics/resources.h>

#include "knight.h"


#define KEY_P1_MOVE_UP      SDL_SCANCODE_W
#define KEY_P1_MOVE_DOWN    SDL_SCANCODE_S
#define KEY_P1_MOVE_LEFT    SDL_SCANCODE_A
#define KEY_P1_MOVE_RIGHT   SDL_SCANCODE_D
#define KEY_P1_ATTACK       SDL_SCANCODE_COMMA
#define KEY_P1_BLOCK        SDL_SCANCODE_PERIOD

#define ATTACK_TIME (1.0f)
#define RAISE_TO_BLOCK_TIME (0.6f)
#define LOWER_FROM_BLOCK_TIME (0.6f)
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
    sprite_block = resources->get_sprite("knight_block");
    sprite_unblock = resources->get_sprite("knight_unblock");
    texture_blocking = resources->get_texture("knight_blocking");

    w = sprite_idle.get_width();
    h = sprite_idle.get_height();

    attack_timer = 0;
    block_transition_timer = 0;

}


void Knight::move(Inputs *inputs, float delta) {
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
}

void Knight::attack() {
    if(state == IDLE || state == WALKING) {
        state = ATTACKING;
        attack_timer = ATTACK_TIME;
        sprite_attack.reset();
    }
}


void Knight::block() {
    switch (state) {
        case IDLE:
        case WALKING:
            block_transition_timer = RAISE_TO_BLOCK_TIME;
            sprite_block.reset();
            state = RAISING_TO_BLOCK;
            break;
        case LOWERING_FROM_BLOCK:
            sprite_block.set_frame_timer(block_transition_timer);
            block_transition_timer = RAISE_TO_BLOCK_TIME - block_transition_timer;
            state = RAISING_TO_BLOCK;
            break;
    }
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
        }
    }

    if (state == RAISING_TO_BLOCK) {
        block_transition_timer -= delta;
        if (block_transition_timer <= 0) {
            state = BLOCKING;
        }
    }

    if (state == LOWERING_FROM_BLOCK) {
        block_transition_timer -= delta;
        if (block_transition_timer <= 0) {
            state = IDLE;
        }
    }



    if (inputs->is_key_down_event(KEY_P1_ATTACK)) {
        attack();
    }

    if (inputs->is_key_down(KEY_P1_BLOCK)) {
        block();
    } else {
        switch (state) {
            case BLOCKING:
                block_transition_timer = LOWER_FROM_BLOCK_TIME;
                sprite_unblock.reset();
                state = LOWERING_FROM_BLOCK;
                break;
            case RAISING_TO_BLOCK:
                sprite_unblock.set_frame_timer(block_transition_timer);
                block_transition_timer = LOWER_FROM_BLOCK_TIME - block_transition_timer;
                state = LOWERING_FROM_BLOCK;
                break;
        }
    }
    
    if (state == WALKING || state == IDLE) {
        move(inputs, delta);
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
        case RAISING_TO_BLOCK:
            if (facing == RIGHT) {
                sprite_block.draw(scene->get_graphics()->get_renderer(), x, y, delta);
            } else {
                sprite_block.draw(scene->get_graphics()->get_renderer(), x, y, 0.0f, true, false, delta);
            }
            break;
        case LOWERING_FROM_BLOCK:
            if (facing == RIGHT) {
                sprite_unblock.draw(scene->get_graphics()->get_renderer(), x, y, delta);
            } else {
                sprite_unblock.draw(scene->get_graphics()->get_renderer(), x, y, 0.0f, true, false, delta);
            }
            break;
        case BLOCKING:
            if (facing == RIGHT) {
                texture_blocking.draw(scene->get_graphics()->get_renderer(), x, y);
            } else {
                texture_blocking.draw(scene->get_graphics()->get_renderer(), x, y, 0.0f, true, false);
            }
            break;
    }
}