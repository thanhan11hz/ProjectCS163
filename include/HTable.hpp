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
        void init();
        void draw();
        void run();
        void exit();
        void initData();
        void insertData();
        void deleteData();
        void searchData();
        void remove();

        void copyList(ListNode *source, ListNode *&des);
        void copy(std::vector<ListNode*> source, std::vector<ListNode*> &des);

        std::vector<ListNode*> HSvalue;
        
};

#endif
//