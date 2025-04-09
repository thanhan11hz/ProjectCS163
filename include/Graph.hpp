#ifndef Graph_hpp
#define Graph_hpp

#include <cmath>
#include <random>
#include <unordered_map>
#include "Logic.hpp"
#include "View.hpp"
class Graph : public Logic, public View {
    public:
        class GraphNode: public Node {
            public:
                Vector2 fixedPosition;
                Vector2 velocity;
                bool isDragging = false;
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
        std::vector<Edge*> edge;
        std::vector<std::vector<int>> edges;
        std::vector<std::vector<std::pair<int, int>>> adjList;
        
        void adjMatrixToEdges();
        void edgesToAdjList();
        struct DSU{
            std::vector<int> parent;
            std::vector<int> size;

            DSU(int n){
                parent.resize(n + 1);
                size.resize(n + 1);

                for (int i = 1; i <= n; i++){
                    parent[i] = i;
                    size[i] = 1;
                }
            }
            int find(int a);
            void unite(int a, int b);
        };

        std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;
        float accumulatedTime = 0.0f;
        const float stepDuration = 0.5f;
        
        void init();
        void draw();
        void drawNode(std::vector<GraphNode*> vertex);
        void drawEdge(std::vector<Edge*> edge);
        void resetColorNode();
        void resetColorNode(Step step);
        void resetColorEdge();
        void resetColorEdge(Step step);
        void run();
        void prepareTransition();
        void exit();
        void remove();
        void initData();
        void dijkstra();
        void checkConnected();
        void prim();
        void kruskal();
};

#endif