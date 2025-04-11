#ifndef HTable_hpp
#define HTable_hpp

#include <unordered_map>
#include <algorithm>
#include "Logic.hpp"
#include "View.hpp"
class HTable: public Logic, public View {

    public:
        class ListNode: public Node {
            public:
                ListNode* next = nullptr;
                ListNode(int val) {
                    this->val = val;
                    next = nullptr;
                    ID = globalID;
                    globalID++;
                }
        };
        std::vector<ListNode*> HSvalue;
        std::vector<Edge*> edge;
        void init();
        void draw();
        void calculatePosition(std::vector<ListNode*> table);
        void calculatePosition(std::vector<Node*> table);
        void drawNode(std::vector<ListNode*> table, int highlight);
        void drawNode(std::vector<Node*> table, int highlight);
        void drawEdge(std::vector<Edge*> edge);
        void resetAlphaNode(std::vector<Node*> table);
        void resetAlphaEdge(std::vector<Edge*> edge);
        void resetColorNode(std::vector<Node*> table);
        void resetColorNode(std::vector<ListNode*> table);
        void resetColorEdge(std::vector<Edge*> edge);
        void run();
        void exit();
        void remove();
        void initData();
        void insertData();
        void deleteData();
        void searchData();
        void copyNode(std::vector<ListNode*> source, std::vector<Node*> &des); 
        void copyEdge(std::vector<Edge*> source, std::vector<Edge*> &des, std::vector<Node*> table);
        ListNode* findNodebyID(std::vector<Node*> table, int ID);
        Edge* findEdgebyEndPoint(std::vector<Edge*> list, int endPoint2ID);  
        void prepareTransition();
        void scanNode(std::vector<Node*> table, std::unordered_map<int,ListNode*> &node);
        void safeRemoveEdge(int nodeID);
        void printHTable(std::vector<Node*> table); 
        void printHTable(std::vector<ListNode*> table); 
};

#endif