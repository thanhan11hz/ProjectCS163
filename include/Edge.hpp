#ifndef Edge_hpp
#define Edge_hpp

#include "GlobalVar.hpp"
#include "Node.hpp"

class Edge {
    public:
        Node *endPoint1 = nullptr, *endPoint2 = nullptr;
        int ID = -1;
        int alpha = 1.0f;
        Color currentColor = BLACK;
        Color targetColor = RED;
        Edge(): endPoint1(nullptr), endPoint2(nullptr) {
            ID = globalID;
            globalID++;
        };
        Edge(Node* endPoint1, Node* endPoint2): endPoint1(endPoint1), endPoint2(endPoint2) {
            ID = globalID;
            globalID++;
        };
        void draw();
        void drawHighlighted();
};

#endif