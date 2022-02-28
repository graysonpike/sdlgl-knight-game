#include <iostream>

#include "sdlgl/graphics/graphics.h"
#include <sdlgl/game/clock.h>
#include <sdlgl/game/scene.h>
#include <sdlgl/audio/audio.h>
#include <sdlgl/input/inputs.h>
#include <stdlib.h>
#include <time.h>

#include <sdlgl/ui/fps_display.h>
#include <sdlgl/ui/entity_count.h>
#include <sdlgl/game/context.h>

#include "entities/knight.h"
#include "ui/state_display.h"


#define KEY_TOGGLE_DEBUG_VISUALS SDL_SCANCODE_GRAVE // Tilde


void game_loop(Context context, Scene *scene) {
    context.inputs->update();
    context.clock->tick();
    context.graphics->clear_screen({255, 255, 255, 255});
    
    scene->update(context.clock->get_delta());
    scene->render();

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

    Context context(new Graphics(640, 480), new Audio(), new Inputs(), new Clock());

    // Load resources
    context.graphics->get_resources()->load_resources("resources.json");

    // Create and populate scene
    Scene *scene = new Scene(context.graphics, context.audio, context.inputs);

    // Game entities
    Knight *knight = new Knight(
        scene, 100, 100, {255, 255, 255, 255});
    scene->add_entity(knight);

    // UI entities
    scene->add_entity(new FPS_Display(
        scene, "base_text", {0, 0, 0, 255}));
    scene->add_entity(new EntityCount(
        scene, "base_text", {0, 0, 0, 255}));
    scene->add_entity(new StateDisplay(
        scene, "base_text", {0, 0, 0, 255}, knight));

    *context.loop = true;
    while (*context.loop) {
        game_loop(context, scene);
    }

    return 0;
}
