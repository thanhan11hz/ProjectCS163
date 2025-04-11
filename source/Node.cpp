#include "Node.hpp"

void Node::draw() {
    DrawCircleV(position,25,Fade(currentColor,alpha));
    DrawCircleV(position,20,Fade(currentColor,alpha));
    DrawCircleLinesV(position,25,Fade(BLACK,alpha));
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
    DrawTextEx(font,std::to_string(val).c_str(),textPos,fontSize,spacing,BLACK);
}

void Node::drawHighlight() {
    DrawCircleV(position,25,Fade(targetColor,alpha));
    DrawCircleV(position,20,Fade(currentColor,alpha));
    DrawCircleLinesV(position,25,Fade(BLACK,alpha));
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
    DrawTextEx(font,std::to_string(val).c_str(),textPos,fontSize,spacing,BLACK);
}

void Node::drawAnimation() {
    Color color = (Color){
        (unsigned char)(currentColor.r + (targetColor.r - currentColor.r) * progress),
        (unsigned char)(currentColor.g + (targetColor.g - currentColor.g) * progress),
        (unsigned char)(currentColor.b + (targetColor.b - currentColor.b) * progress),
        (unsigned char)(currentColor.a + (targetColor.a - currentColor.a) * progress)
    };
    DrawCircleV(position,25,Fade(color,alpha));
    DrawCircleV(position,20,Fade(currentColor,alpha));
    DrawCircleLinesV(position,25,Fade(BLACK,alpha));
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
    DrawTextEx(font,std::to_string(val).c_str(),textPos,fontSize,spacing,BLACK);
}