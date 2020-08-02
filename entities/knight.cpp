#include <sdlgl/graphics/resources.h>

#include "knight.h"


#define KEY_P1_MOVE_UP      SDL_SCANCODE_W
#define KEY_P1_MOVE_DOWN    SDL_SCANCODE_S
#define KEY_P1_MOVE_LEFT    SDL_SCANCODE_A
#define KEY_P1_MOVE_RIGHT   SDL_SCANCODE_D
#define KEY_P1_ATTACK       SDL_SCANCODE_COMMA
#define KEY_P1_BLOCK        SDL_SCANCODE_PERIOD

#define ATTACK_TIME (1.0f)
#define RAISE_TO_BLOCK_TIME (0.3f)
#define LOWER_FROM_BLOCK_TIME (0.3f)
#define H_SPEED (100)
#define V_SPEED (100)
#define WIDTH (50)
#define HEIGHT (110)


Knight::Knight(Scene *scene, float x, float y, SDL_Color color) :
    PhysicalEntity(scene, x, y, WIDTH, HEIGHT),
    color(color),
    state(IDLE),
    facing(RIGHT) {

    Resources *resources = scene->get_graphics()->get_resources();

    sprites["idle"] = resources->get_sprite("knight_idle");
    sprites["walk"] = resources->get_sprite("knight_walk");
    sprites["attack"] = resources->get_sprite("knight_attack");
    sprites["block"] = resources->get_sprite("knight_block");
    sprites["unblock"] = resources->get_sprite("knight_unblock");
    textures["blocking"] = resources->get_texture("knight_blocking");

    sprites["block"].set_delay(RAISE_TO_BLOCK_TIME / 6.0f);
    sprites["unblock"].set_delay(LOWER_FROM_BLOCK_TIME / 6.0f);

    attack_timer = 0;
    block_transition_timer = 0;

}


const char* Knight::state_strings[] = {
    "IDLE",
    "WALKING",
    "ATTACKING",
    "RAISING_TO_BLOCK",
    "BLOCKING",
    "LOWERING_FROM_BLOCK"
};


std::string Knight::get_state() {
    return state_strings[state];
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
        sprites["attack"].reset();
    }
}


void Knight::block() {
    switch (state) {
        case IDLE:
        case WALKING:
            block_transition_timer = RAISE_TO_BLOCK_TIME;
            sprites["block"].reset();
            state = RAISING_TO_BLOCK;
            break;
        case LOWERING_FROM_BLOCK:
            sprites["block"].set_frame_timer(block_transition_timer);
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
                sprites["unblock"].reset();
                state = LOWERING_FROM_BLOCK;
                break;
            case RAISING_TO_BLOCK:
                sprites["unblock"].set_frame_timer(block_transition_timer);
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
                sprites["idle"].draw(scene, x, y, delta);
            } else {
                sprites["idle"].draw(scene, x, y, 0.0f, true, false, delta);
            }
            break;
        case WALKING:
            if (facing == RIGHT) {
                sprites["walk"].draw(scene, x, y, delta);
            } else {
                sprites["walk"].draw(scene, x, y, 0.0f, true, false, delta);
            }
            break;
        case ATTACKING:
            if (facing == RIGHT) {
                sprites["attack"].draw(scene, x, y, delta);
            } else {
                sprites["attack"].draw(scene, x, y, 0.0f, true, false, delta);
            }
            break;
        case RAISING_TO_BLOCK:
            if (facing == RIGHT) {
                sprites["block"].draw(scene, x, y, delta);
            } else {
                sprites["block"].draw(scene, x, y, 0.0f, true, false, delta);
            }
            break;
        case LOWERING_FROM_BLOCK:
            if (facing == RIGHT) {
                sprites["unblock"].draw(scene, x, y, delta);
            } else {
                sprites["unblock"].draw(scene, x, y, 0.0f, true, false, delta);
            }
            break;
        case BLOCKING:
            if (facing == RIGHT) {
                textures["blocking"].draw(scene, x, y);
            } else {
                textures["blocking"].draw(scene, x, y, 0.0f, true, false);
            }
            break;
    }

    if (scene->get_graphics()->get_debug_visuals_enabled()) {
        SDL_Renderer *renderer = scene->get_graphics()->get_renderer();

        // Draw red rect at x, y coords
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect box_rect = {(int)x, (int)y, w, h};
        SDL_RenderDrawRect(renderer, &box_rect);
    }

}
