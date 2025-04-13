#include "Edge.hpp"

void Edge::draw() {
    Vector2 diff = {endPoint1->position.x - endPoint2->position.x, endPoint1->position.y - endPoint2->position.y};
   float dis = std::sqrt(diff.x * diff.x + diff.y * diff.y);
   Vector2 point1 = {(20 * endPoint2->position.x - (20 - dis) * endPoint1->position.x) / dis, (20 * endPoint2->position.y - (20 - dis) * endPoint1->position.y) / dis};
   Vector2 point2 = {((dis - 20) * endPoint2->position.x + 20 * endPoint1->position.x) / dis, ((dis - 20) * endPoint2->position.y + 20 * endPoint1->position.y) / dis};
   DrawLineEx(point1,point2,5,Fade(currentColor,alpha));
   if (mode == Mode::GRAPH) {
        Vector2 mid = { (point1.x + point2.x) * 0.5f, (point1.y + point2.y) * 0.5f };
        Vector2 dir = { point2.x - point1.x, point2.y - point1.y };
        float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
        if (len != 0)
        {
            dir.x /= len; 
            dir.y /= len;
        }
        Vector2 perp = { -dir.y, dir.x };
        float offsetDistance = 10.0f;
        perp.x *= offsetDistance;
        perp.y *= offsetDistance;
        Vector2 textPos = { mid.x + perp.x, mid.y + perp.y };
        if (theme == colorType::HOT) DrawTextEx(font, std::to_string(weight).c_str(), textPos, 20, 5, myColor1[0]); 
        else DrawTextEx(font, std::to_string(weight).c_str(), textPos, 20, 5, myColor2[0]);
   }
}

void Edge::drawHighlighted() {
    Vector2 diff = {endPoint1->position.x - endPoint2->position.x, endPoint1->position.y - endPoint2->position.y};
    float dis = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    Vector2 point1 = {(20 * endPoint2->position.x - (20 - dis) * endPoint1->position.x) / dis, (20 * endPoint2->position.y - (20 - dis) * endPoint1->position.y) / dis};
    Vector2 point2 = {((dis - 20) * endPoint2->position.x + 20 * endPoint1->position.x) / dis, ((dis - 20) * endPoint2->position.y + 20 * endPoint1->position.y) / dis};
    DrawLineEx(point1,point2,5,Fade(targetColor,alpha));
    if (mode == Mode::GRAPH) {
        Vector2 mid = { (point1.x + point2.x) * 0.5f, (point1.y + point2.y) * 0.5f };
        Vector2 dir = { point2.x - point1.x, point2.y - point1.y };
        float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
        if (len != 0)
        {
            dir.x /= len; 
            dir.y /= len;
        }
        Vector2 perp = { -dir.y, dir.x };
        float offsetDistance = 10.0f;
        perp.x *= offsetDistance;
        perp.y *= offsetDistance;
        Vector2 textPos = { mid.x + perp.x, mid.y + perp.y };
        if (theme == colorType::HOT) DrawTextEx(font, std::to_string(weight).c_str(), textPos, 20, 5, myColor1[0]); 
        else DrawTextEx(font, std::to_string(weight).c_str(), textPos, 20, 5, myColor2[0]);
   }
}

void Edge::drawAnimation() {
    Vector2 diff = {endPoint1->position.x - endPoint2->position.x, endPoint1->position.y - endPoint2->position.y};
    float dis = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    Vector2 point1 = {(20 * endPoint2->position.x - (20 - dis) * endPoint1->position.x) / dis, (20 * endPoint2->position.y - (20 - dis) * endPoint1->position.y) / dis};
    Vector2 point2 = {((dis - 20) * endPoint2->position.x + 20 * endPoint1->position.x) / dis, ((dis - 20) * endPoint2->position.y + 20 * endPoint1->position.y) / dis};
    Vector2 point3 = {point2.x * progress + point1.x * (1.0f - progress), point2.y * progress + point1.y * (1.0f - progress)};
    DrawLineEx(point1,point3,5,Fade(targetColor,alpha));
    DrawLineEx(point3,point2,5,Fade(currentColor,alpha));
    if (mode == Mode::GRAPH) {
        Vector2 mid = { (point1.x + point2.x) * 0.5f, (point1.y + point2.y) * 0.5f };
        Vector2 dir = { point2.x - point1.x, point2.y - point1.y };
        float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
        if (len != 0)
        {
            dir.x /= len; 
            dir.y /= len;
        }
        Vector2 perp = { -dir.y, dir.x };
        float offsetDistance = 20.0f;
        perp.x *= offsetDistance;
        perp.y *= offsetDistance;
        Vector2 textPos = { mid.x + perp.x, mid.y + perp.y };
        DrawTextEx(font, std::to_string(weight).c_str(), textPos, 25, 5, BLACK);
   }   
}