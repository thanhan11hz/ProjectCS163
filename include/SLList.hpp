#ifndef SLList_hpp
#define SLList_hpp

#include "Logic.hpp"
#include "View.hpp"
class SLList: public Logic, public View {
    public:
        class ListNode: public Node {
            public:
                ListNode* next;
                ListNode(int val) {
                    this->val = val;
                    next = nullptr;
                }
        };
        ListNode *root;        
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