#ifndef HTable_hpp
#define HTable_hpp

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
        void init();
        void draw();
        void run();
        void exit();
        void remove();
        void initData();
        void insertData();
        void deleteData();
        void searchData();
        void copy(std::vector<ListNode*> source, std::vector<Node*> &des);   
        void printHTable(std::vector<Node*> table); 
        void printHTable(std::vector<ListNode*> table); 
};

#endif
//