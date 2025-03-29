#ifndef SLList_hpp
#define SLList_hpp

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
        void init();
        void draw();
        void calculatePosition(ListNode* head);
        void run();
        void remove();
        void exit();
        void copy(ListNode* source, Node* &des);
        void initData();
        void insertData();
        void deleteData();
        void searchData();
};

#endif