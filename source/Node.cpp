#include "Node.hpp"

void Node::drawNode() {
    if (isHighlighted) {
        DrawCircleV(position,20,RED);
        int fontSize = 20;
        int spacing = 5;
        Vector2 textSize = MeasureTextEx(font,std::to_string(val).c_str(),fontSize,spacing);
        Vector2 textPos = {
            position.x + (20 - textSize.x)/2.0f,
            position.y + (20 - textSize.y)/2.0f
        };
        DrawTextEx(font,std::to_string(val).c_str(),textPos,fontSize,spacing,WHITE);
    } else {
        DrawCircleV(position,20,ORANGE);
    }
}