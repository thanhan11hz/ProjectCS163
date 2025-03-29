#ifndef Graph_hpp
#define Graph_hpp

#include <cmath>
#include <random>
#include "Logic.hpp"
#include "View.hpp"
class Graph : public Logic, public View {
    public:
        class GraphNode: public Node {
            public:
                Vector2 fixedPosition;
                Vector2 velocity;
                bool isDragging = false;
                std::vector<GraphNode*> adj;
                void applySpringForce();
                void applyDragForce();
                void updatePosition();
        };
        bool dragging = false;
        GraphNode* selectedNode = nullptr;
        void applyElectricForce();
        void updateVertex();
        void handleMouse();
        bool checkValidPos();
        void generatePosition();
        std::vector<GraphNode*> vertex;
        class Edge {
            public:
                int ID;
                GraphNode *endPoint1,*endPoint2;
                Edge() {
                    ID = globalID;
                    globalID++;
                    endPoint1 = endPoint2 = nullptr;
                }
        };
        std::vector<Edge*> edge;
        void init();
        void draw();
        void run();
        void exit();
        void remove();
        void initData();
        void dijkstra();
        void checkConnected();
        void prim();
        void kruskal();
};

#endif