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
        }; // For graph

        std::vector<Edge*> edge;
        bool Comparator(std::vector<int> a, std::vector<int> b);
        vector<vector<int>> edges;
        vector<vector<pair<int, int>>> ADJList;
        
        void ADJmatrixtoEdges();
        void EdgestoADJList();
        struct DSU{
            vector<int> parent;
            vector<int> size;

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