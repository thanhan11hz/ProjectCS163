#include "Node.hpp"

void Node::draw() {
    DrawCircleV(position,35,Fade(GRAY,0.6f * alpha));
    DrawCircleV(position,30,Fade(GRAY,alpha));
    DrawCircleLinesV(position,35,Fade(BLACK,alpha * 0.6f));
    DrawCircleLinesV(position,30,Fade(BLACK,alpha));
    int minFontSize = 10;
    int baseSize = 20 - (std::to_string(val).size() * 2); 
    int fontSize = (baseSize < minFontSize) ? minFontSize : baseSize;
    int spacing = 2;
    Vector2 textSize = MeasureTextEx(font,std::to_string(val).c_str(),fontSize,spacing);
    Vector2 textPos = {
        position.x - 30 + (60 - textSize.x)/2.0f,
        position.y - 30 + (60 - textSize.y)/2.0f
    };
    DrawTextEx(font,std::to_string(val).c_str(),textPos,fontSize,spacing,WHITE);
}

void Node::drawHighlight() {
    DrawCircleV(position,35,Fade(RED,alpha * 0.6f));
    DrawCircleV(position,30,Fade(GRAY,alpha));
    DrawCircleLinesV(position,35,Fade(BLACK,alpha * 0.6f));
    DrawCircleLinesV(position,30,Fade(BLACK,alpha));
    int minFontSize = 10;
    int baseSize = 20 - (std::to_string(val).size() * 2); 
    int fontSize = (baseSize < minFontSize) ? minFontSize : baseSize;
    int spacing = 2;
    Vector2 textSize = MeasureTextEx(font,std::to_string(val).c_str(),fontSize,spacing);
    Vector2 textPos = {
        position.x - 30 + (60 - textSize.x)/2.0f,
        position.y - 30 + (60 - textSize.y)/2.0f
    };
    DrawTextEx(font,std::to_string(val).c_str(),textPos,fontSize,spacing,WHITE);
}