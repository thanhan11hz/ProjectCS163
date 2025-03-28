#include "Node.hpp"

void Node::drawNode() {
    DrawCircleV(position,20,ORANGE);
    int fontSize = 20;
    int spacing = 5;
    Vector2 textSize = MeasureTextEx(font,std::to_string(val).c_str(),fontSize,spacing);
    Vector2 textPos = {
        position.x - 20 + (40 - textSize.x)/2.0f,
        position.y - 20 + (40 - textSize.y)/2.0f
    };
    DrawTextEx(font,std::to_string(val).c_str(),textPos,fontSize,spacing,WHITE);
}

void Node::drawHighlightNode() {
    DrawCircleV(position,20,RED);
    int fontSize = 20;
    int spacing = 5;
    Vector2 textSize = MeasureTextEx(font,std::to_string(val).c_str(),fontSize,spacing);
    Vector2 textPos = {
        position.x - 20 + (40 - textSize.x)/2.0f,
        position.y - 20 + (40 - textSize.y)/2.0f
    };
    DrawTextEx(font,std::to_string(val).c_str(),textPos,fontSize,spacing,WHITE);
}

void Node::drawEdge(Node* A) {
    DrawLine((3*position.x+A->position.x)/4.0f,(3*position.y+A->position.y)/4.0f,(position.x+3*A->position.x)/4.0f,(position.y+3*A->position.y)/4.0f,BLACK);
}
