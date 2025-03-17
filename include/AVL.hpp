#ifndef AVL_hpp
#define AVL_hpp

#include "Logic.hpp"
#include "View.hpp"

class AVL : public Logic, public View {
    public:
        class TreeNode : public Node {
            TreeNode *left = nullptr,*right = nullptr;
            TreeNode(int val) {
                this->val = val;
                left = right = nullptr;
                ID = globalID;
                globalID++;
            }
        };
        void init();
        void draw(); 
        void run();
        void remove();
        void exit();
        void copy(TreeNode* source, TreeNode* &des);
        void initData();
        void insertData();
        void deleteData();
        void searchData();
};

#endif