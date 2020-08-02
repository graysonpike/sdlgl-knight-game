#include <iostream>

#include "sdlgl/graphics/graphics.h"
#include <sdlgl/game/clock.h>
#include <sdlgl/game/scene.h>
#include <sdlgl/input/inputs.h>
#include <stdlib.h>
#include <time.h>

#include <sdlgl/ui/fps_display.h>
#include <sdlgl/ui/entity_count.h>

#include "entities/knight.h"
#include "ui/state_display.h"


#define KEY_TOGGLE_DEBUG_VISUALS SDL_SCANCODE_GRAVE // Tilde


struct Context {
    Graphics *graphics;
    Scene *scene;
    Inputs *inputs;
    Clock *clock;
    bool *loop;
};


void game_loop(struct Context context) {
    context.inputs->update();
    context.clock->tick();
    context.graphics->clear_screen({255, 255, 255, 255});
    
    context.scene->update(context.clock->get_delta());
    context.scene->render();

    context.graphics->present_renderer(context.clock->get_delta());

    // If ESC or 'X' button is pressed, leave the update loop and exit
    if (context.inputs->get_quit()) {
        *context.loop = false;
    }

    // If tilde key is pressed, toggle debug visuals
    if (context.inputs->is_key_down_event(KEY_TOGGLE_DEBUG_VISUALS)) {
        context.graphics->toggle_debug_visuals();
    }
}


int main() {

    srand(time(NULL));

    struct Context context;

    context.clock = new Clock();
    context.inputs = new Inputs();

    // Load a window
    context.graphics = new Graphics(640, 480);

    // Load resources
    context.graphics->get_resources()->load_resources("resources.json");

    // Create and populate scene
    context.scene = new Scene(context.inputs, context.graphics);

    // Game entities
    Knight *knight = new Knight(
        context.scene, 100, 100, {255, 255, 255, 255});
    context.scene->add_entity(knight);

    // UI entities
    context.scene->add_entity(new FPS_Display(
        context.scene, "base_text", {0, 0, 0, 255}));
    context.scene->add_entity(new EntityCount(
        context.scene, "base_text", {0, 0, 0, 255}));
    context.scene->add_entity(new StateDisplay(
        context.scene, "base_text", {0, 0, 0, 255}, knight));

    *context.loop = true;
    while (*context.loop) {
        game_loop(context);
    }

    return 0;
}
