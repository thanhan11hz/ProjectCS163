#include "Button.hpp"
#include "GlobalVar.hpp"

void Button::draw() {
    Color colorDraw = isHovered ? Fade(color,0.6f) : color;
    float roundness = 0.2f; 
    int segments = 10; 
    DrawRectangleRounded(rec, roundness, segments, colorDraw);
    float fontSize = 20;
    float spacing = 2;
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);
    Vector2 textPos = {
        rec.x + rec.width / 2.0f - textSize.x / 2.0f,
        rec.y + rec.height / 2.0f - textSize.y / 2.0f
    };
    DrawTextEx(font, text.c_str(), textPos, fontSize, spacing, (Color){248,222,34,255});
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