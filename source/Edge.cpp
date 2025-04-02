#include "Edge.hpp"

void Edge::draw() {
    Vector2 diff = {endPoint1->position.x - endPoint2->position.x, endPoint1->position.y - endPoint2->position.y};
   float dis = std::sqrt(diff.x * diff.x + diff.y * diff.y);
   Vector2 point1 = {(30 * endPoint2->position.x - (30 - dis) * endPoint1->position.x) / dis, (30 * endPoint2->position.y - (30 - dis) * endPoint1->position.y) / dis};
   Vector2 point2 = {((dis - 30) * endPoint2->position.x + 30 * endPoint1->position.x) / dis, ((dis - 30) * endPoint2->position.y + 30 * endPoint1->position.y) / dis};
   DrawLineEx(point1,point2,5,BLACK);
}

void Edge::drawHighlighted() {
    Vector2 diff = {endPoint1->position.x - endPoint2->position.x, endPoint1->position.y - endPoint2->position.y};
    float dis = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    Vector2 point1 = {(30 * endPoint2->position.x - (30 - dis) * endPoint1->position.x) / dis, (30 * endPoint2->position.y - (30 - dis) * endPoint1->position.y) / dis};
    Vector2 point2 = {((dis - 30) * endPoint2->position.x + 30 * endPoint1->position.x) / dis, ((dis - 30) * endPoint2->position.y + 30 * endPoint1->position.y) / dis};
    DrawLineEx(point1,point2,5,RED);
}