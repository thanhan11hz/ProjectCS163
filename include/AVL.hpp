#ifndef AVL_hpp
#define AVL_hpp

#include "Logic.hpp"
#include "View.hpp"

class AVL : public Logic, public View {
    public:
        class TreeNode : public Node {
            public: 
            TreeNode *left = nullptr,*right = nullptr;
            int height;
            TreeNode(int val) {
                this->val = val;
                left = right = nullptr;
                ID = globalID;
                globalID++;
                height = 1;
            }
        };
        TreeNode* root = nullptr;
        void init();
        void draw();
        void calculatePositions(TreeNode* node, int x, int y, int offsetX);
        void drawEdges(TreeNode* node);
        void drawTree(TreeNode* node, int highlightNode);
        void run();
        void remove();
        void exit();
        void copy(TreeNode* source, TreeNode* &des);
        void deleteTree(TreeNode* root);
        int getHeight(TreeNode* node);
        TreeNode* getSuccessor(TreeNode* root);
        int balanceFactor(TreeNode* node);
        void leftRotate(TreeNode* &node);
        void rightRotate(TreeNode* &node);
        void initData();
        void insertData();
        void deleteData();
        void searchData();
        void insertNode(TreeNode* &node, int key, Step step);
        void deleteNode(TreeNode* &node, int key, Step step);
        void searchNode(TreeNode* node, int key, Step step);
        void printInorder(TreeNode* node);
};

#endif