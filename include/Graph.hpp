#ifndef Graph_hpp
#define Graph_hpp

#include <cmath>
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
        void applyElectricForce();
        void updateVertex();
        void handleMouse();
        std::vector<GraphNode*> vertex;
        class Edge {
            GraphNode* endPoint1,endPoint2;
        };
        std::vector<Edge*> edge;
        void init();
        void draw();
        void run();
        void exit();
        void initData();
        void insertData();
        void deleteData();
        void searchData();
};

#endif