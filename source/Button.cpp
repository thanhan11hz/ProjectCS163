#include "Button.hpp"
#include "GlobalVar.hpp"

void Button::draw() {
    Color colorDraw = isHovered ? Fade(WHITE,0.6f) : WHITE;
    Rectangle sourceRec = {0, 0, (float)picture.width, (float)picture.height};
    Vector2 origin = {0, 0};
    DrawTexturePro(picture, sourceRec, rec, origin, 0.0f, colorDraw);
}

void Button::checkHovered(Vector2 mousePosition) {
    if (CheckCollisionPointRec(mousePosition,rec)) {
        isHovered = true;
    } else isHovered = false;
}

bool Button::checkPress(Vector2 mousePosition, bool mousePressed) {
    if (CheckCollisionPointRec(mousePosition,rec) && mousePressed) {
        return true;
    }
    return false;
}