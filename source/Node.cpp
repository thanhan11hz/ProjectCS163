#include "Node.hpp"

void Node::draw() {
    DrawCircleV(position,25,Fade(GRAY,0.6f * alpha));
    DrawCircleV(position,20,Fade(GRAY,alpha));
    DrawCircleLinesV(position,25,Fade(BLACK,alpha * 0.6f));
    DrawCircleLinesV(position,20,Fade(BLACK,alpha));
    int minFontSize = 10;
    int baseSize = 20 - (std::to_string(val).size() * 2); 
    int fontSize = (baseSize < minFontSize) ? minFontSize : baseSize;
    int spacing = 2;
    Vector2 textSize = MeasureTextEx(font,std::to_string(val).c_str(),fontSize,spacing);
    Vector2 textPos = {
        position.x - 20 + (40 - textSize.x)/2.0f,
        position.y - 20 + (40 - textSize.y)/2.0f
    };
    DrawTextEx(font,std::to_string(val).c_str(),textPos,fontSize,spacing,WHITE);
}

void Node::drawHighlight() {
    DrawCircleV(position,25,Fade(RED,alpha * 0.6f));
    DrawCircleV(position,20,Fade(GRAY,alpha));
    DrawCircleLinesV(position,25,Fade(BLACK,alpha * 0.6f));
    DrawCircleLinesV(position,20,Fade(BLACK,alpha));
    int minFontSize = 10;
    int baseSize = 20 - (std::to_string(val).size() * 2); 
    int fontSize = (baseSize < minFontSize) ? minFontSize : baseSize;
    int spacing = 2;
    Vector2 textSize = MeasureTextEx(font,std::to_string(val).c_str(),fontSize,spacing);
    Vector2 textPos = {
        position.x - 20 + (40 - textSize.x)/2.0f,
        position.y - 20 + (40 - textSize.y)/2.0f
    };
    DrawTextEx(font,std::to_string(val).c_str(),textPos,fontSize,spacing,WHITE);
}