#include "state_display.h"

#define X_COORD 16
#define Y_COORD (16 * 3)


StateDisplay::StateDisplay(Scene *scene, std::string font, SDL_Color color)
: Entity(scene) {
    this->font = font;
    this->color = color;
    knight = nullptr;
}


StateDisplay::StateDisplay(Scene *scene, std::string font, SDL_Color color, Knight *knight)
: Entity(scene) {
    this->font = font;
    this->color = color;
    this->knight = knight;
}


void StateDisplay::set_knight(Knight *knight) {
	this->knight = knight;
}


void StateDisplay::update() {}


void StateDisplay::render() {
	if (!scene->get_graphics()->get_debug_visuals_enabled() || knight == nullptr) {
        return;
    }

    SDL_Texture *text_texture = NULL;
    int texture_width, texture_height;
    
    std::string text = "Knight State: " + knight->get_state();

    scene->get_graphics()->get_font_renderer()->load_font_texture(
        &text_texture, font, text, color);
    SDL_QueryTexture(text_texture, NULL, NULL, &texture_width, &texture_height);

    SDL_Rect dst = {
        X_COORD,
        Y_COORD,
        texture_width,
        texture_height
    };

    SDL_RenderCopy(scene->get_graphics()->get_renderer(), text_texture, NULL, &dst);

    SDL_DestroyTexture(text_texture);
}
