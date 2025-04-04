#ifndef SLList_hpp
#define SLList_hpp

#include <unordered_map>
#include <algorithm>
#include "Logic.hpp"
#include "View.hpp"
class SLList: public Logic, public View {
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
        ListNode *root = nullptr;
        std::vector<Edge*> edge;
        std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;
        float accumulatedTime = 0.0f;
        const float stepDuration = 0.5f;        
        void init();
        void draw();
        void calculatePosition(ListNode* head);
        void drawNode(ListNode* head, int highlight);
        void drawEdge(std::vector<Edge*> edge);
        void resetAlphaNode(ListNode* head);
        void resetAlphaEdge(std::vector<Edge*> edge);
        void run();
        void remove();
        void exit();
        void copyNode(ListNode* source, Node* &des);
        void copyEdge(std::vector<Edge*> source, std::vector<Edge*> &des, ListNode* head);
        ListNode* findNodebyID(ListNode* head, int ID);
        Edge* findEdgebyEndPoint(std::vector<Edge*> list, int endPoint2ID);
        void prepareTransition();
        void safeRemoveEdge(int nodeID);
        void initData();
        void insertData();
        void deleteData();
        void searchData();
};

#endif