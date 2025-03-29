#ifndef Node_hpp
#define Node_hpp

#include <iostream>
#include <string>
#include <cmath>
#include "raylib.h"
#include "GlobalVar.hpp"

class Node {
    public:
        int val;
        Vector2 position;
        int ID;
        void drawNode();
        void drawHighlightNode();
        void drawEdge(Node* x);
        void drawHighlightedEdge(Node* x);
};

#endif 