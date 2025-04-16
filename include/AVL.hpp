#ifndef AVL_hpp
#define AVL_hpp

#include <unordered_map>
#include <algorithm>
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
        std::vector<Edge*> edge;        
        void init();
        void draw();
        std::unordered_map<TreeNode*, int> subtreeWidth;
        int calculateSubtreeWidth(TreeNode* node);
        int countRightmost(TreeNode* node);
        int countLeftmost(TreeNode* node);
        void calculatePositions(TreeNode* node, int x, int y);
        void resetAlphaNode(TreeNode* node);
        void resetAlphaEdge(std::vector<Edge*> edge);
        void resetColorNode(TreeNode* node);
        void resetColorEdge(std::vector<Edge*> edge);
        void drawEdge(std::vector<Edge*> edge);
        void drawNode(TreeNode* node, int highlightNode);
        void run();
        void remove();
        void exit();
        void copyNode(TreeNode* source, TreeNode* &des);
        void copyEdge(std::vector<Edge*> source, std::vector<Edge*> &des, TreeNode* head);
        TreeNode* findNodebyID(TreeNode* head, int ID);
        Edge* findEdgebyEndPoint(std::vector<Edge*> list, int endPoint2ID);
        void prepareTransition();
        void scanNode(TreeNode* root, std::unordered_map<int,TreeNode*> &node);
        TreeNode* findParent(TreeNode* node, int ID);
        void safeRemoveEdge(int nodeID);
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
        void updateData();
        void insertNode(TreeNode* &node, int key, Step step);
        void deleteNode(TreeNode* &node, int key, Step step);
        void searchNode(TreeNode* node, int key, Step step);
        bool deleteSuccess;
        void printInorder(TreeNode* node);
};

#endif