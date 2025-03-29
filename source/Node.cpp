#include "Node.hpp"

void Node::drawNode() {
    DrawCircleV(position,35,Fade(GRAY,0.6f));
    DrawCircleV(position,30,GRAY);
    DrawCircleLinesV(position,35,BLACK);
    DrawCircleLinesV(position,30,BLACK);
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

void Node::drawHighlightNode() {
    DrawCircleV(position,35,RED);
    DrawCircleV(position,30,GRAY);
    DrawCircleLinesV(position,35,BLACK);
    DrawCircleLinesV(position,30,BLACK);
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

void Node::drawEdge(Node* A) {
   Vector2 diff = {position.x - A->position.x, position.y - A->position.y};
   float dis = std::sqrt(diff.x * diff.x + diff.y * diff.y);
   Vector2 point1 = {(30 * A->position.x - (30 - dis) * position.x) / dis, (30 * A->position.y - (30 - dis) * position.y) / dis};
   Vector2 point2 = {((dis - 30) * A->position.x + 30 * position.x) / dis, ((dis - 30) * A->position.y + 30 * position.y) / dis};
   DrawLineEx(point1,point2,5,BLACK);
}

void Node::drawHighlightedEdge(Node* A) {
    Vector2 diff = {position.x - A->position.x, position.y - A->position.y};
    float dis = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    Vector2 point1 = {(30 * A->position.x - (30 - dis) * position.x) / dis, (30 * A->position.y - (30 - dis) * position.y) / dis};
    Vector2 point2 = {((dis - 30) * A->position.x + 30 * position.x) / dis, ((dis - 30) * A->position.y + 30 * position.y) / dis};
    DrawLineEx(point1,point2,10,RED);
}
