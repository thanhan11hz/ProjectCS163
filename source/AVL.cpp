#include "AVL.hpp"

void AVL::init() {
    initView();
    root = nullptr;
    stepmanager.currentStep = 0;
    SetMousePosition(780,400);
}

void AVL::draw() {
    drawView();
    if (root) {
        if (!stepmanager.step.empty() && stepmanager.currentStep >= 0) {
            Step currStep = stepmanager.step[stepmanager.currentStep];
            log.infor = currStep.description;
            code.lineHighlighted = currStep.highlightedLine;
            drawView();
            calculatePositions((TreeNode*)currStep.tempRoot,980,120,50);
            drawEdges((TreeNode*)currStep.tempRoot);
            drawTree((TreeNode*)currStep.tempRoot,currStep.highlightedNode);
        } else {
            calculatePositions(root,980,120,50);
            drawEdges(root);
            drawTree(root,-1);
        }
    }
}

void AVL::calculatePositions(TreeNode* node, int x, int y, int offsetX) {
    if (!node) return;
    node->position.x = x;
    node->position.y = y;
    if (node->left) 
        calculatePositions(node->left, x - offsetX, y + 100, offsetX / 2);
    if (node->right) 
        calculatePositions(node->right, x + offsetX, y + 100, offsetX / 2);
}

void AVL::drawEdges(TreeNode* node) {
    if (!node) return;
    if (node->left)
        node->drawEdge(node->left);

    if (node->right)
        node->drawEdge(node->right);
    drawEdges(node->left);
    drawEdges(node->right);
}

void AVL::drawTree(TreeNode* node, int highlightNode) {
    if (!node) return;
    if (node->ID == highlightNode) node->drawHighlightNode();
    else node->drawNode();
    drawTree(node->left,highlightNode);
    drawTree(node->right,highlightNode);
}

void AVL::run() {
    eventView();
    
    if (option.isInitialize()) func = Function::INIT;
    if (option.isAdd()) func = Function::INSERT;
    if (option.isDelete()) func = Function::DELETE;
    if (option.isSearch()) func = Function::SEARCH;

    if (!box.isOpen && func != Function::NONE) {
        switch (func) {
            case Function::INIT:
                remove();
                initData();
                func = Function::NONE;
                break;
            case Function::INSERT:
                remove();
                insertData();
                func = Function::NONE;
                break;
            case Function::DELETE:
                remove();
                deleteData();
                func = Function::NONE;
                break;
            case Function::SEARCH:
                remove();
                searchData();
                func = Function::NONE;
                break;
            default:
                break;
        }
    }

    if (panel.isForwardPressed()) {
        stepmanager.isPlaying = false;
        panel.isPlaying = false;
        stepmanager.nextStep();
    }
     if (panel.isRewindPressed()) {
        stepmanager.isPlaying = false;
        panel.isPlaying = false;
        stepmanager.prevStep();
    }
    if (panel.isPausePressed()) {
        stepmanager.isPlaying = false;
        panel.isPlaying = false;
    }
    if (panel.isPlayPressed()) {
        stepmanager.isPlaying = true;
        panel.isPlaying = true;
    }
    if (stepmanager.isPlaying && stepmanager.currentStep < stepmanager.step.size() - 1) {
        stepmanager.nextStep();
        draw();
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(800 / stepmanager.speed)));
    } else {
        stepmanager.isPlaying = false;
        panel.isPlaying = false; 
    }
}

void AVL::remove() {
    stepmanager.isPlaying = false;
    code.codeline.clear();
    //std::cout<<stepmanager.step.size()<<"\n";
    for (int i = 0; i < stepmanager.step.size(); ++i) {
        //printInorder((TreeNode*)stepmanager.step[i].tempRoot);
        deleteTree((TreeNode*)stepmanager.step[i].tempRoot);
        //std::cout<<"In remove\n";
        stepmanager.step[i].tempRoot = nullptr;
    }
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
}

void AVL::exit() {
    code.codeline.clear();
    box.someList.clear();
    log.infor.clear();
    deleteTree(root);
    root = nullptr;
    for (int i = 0; i < stepmanager.step.size(); ++i) {
        deleteTree((TreeNode*)stepmanager.step[i].tempRoot);
        stepmanager.step[i].tempRoot = nullptr;
    }
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
}

void AVL::copy(TreeNode* source, TreeNode* &des) {
    if (!source) return;
    des = new TreeNode(source->val);
    des->position = source->position;
    des->ID = source->ID;
    copy(source->left,des->left);
    copy(source->right,des->right);
}

void AVL::deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
    root = nullptr;
}

int AVL::getHeight(TreeNode* node) {
    if (node==nullptr) {
        return 0;
    }
    return node->height;
}

void AVL::leftRotate(TreeNode* &node) {
    TreeNode* x = node->right;
    TreeNode* z= x->left;
    x->left=node;
    node->right=z;
    node->height=1+std::max(getHeight(node->left),getHeight(node->right));
    x->height=1+std::max(getHeight(x->left),getHeight(x->right));
    node = x;
}

void AVL::rightRotate(TreeNode* &node) {
    TreeNode* x = node->left;
    TreeNode* z= x->right;
    x->right=node;
    node->left=z;
    node->height=1+std::max(getHeight(node->left),getHeight(node->right));
    x->height=1+std::max(getHeight(x->left),getHeight(x->right));
    node = x;
}

AVL::TreeNode* AVL::getSuccessor(TreeNode* root) {
    if (!root || !root->right) return nullptr;
    TreeNode* curr = root->right;
    while (curr->left) {
        curr = curr->left;
    }
    return curr;
}

int AVL::balanceFactor(TreeNode* node) {
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

void AVL::initData() {
    if (box.someList.empty()) return;
    Step step;
    for (int i = 0; i < box.someList.size(); ++i) {
        insertNode(root,box.someList[i],step);
    }
    //std::cout << "Before remove\n";
    remove();
    //std::cout << "After remove\n";
    box.someList.clear();
}

void AVL::insertData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "if (node == nullptr)                                             ",
        "{    node = new TreeNode(key); return;                           ",
        "if (node->val == key) return;                                    ",
        "else if (node->val > key) insertNode(node->left,key);            ",
        "else insertNode(node->right,key,root,step);                      ",
        "node->height=1+max(getHeight(node->left),getHeight(node->right));",
        "int bl = balanceFactor(node);                                    ",
        "if (bl > 1 && node->left->val > key)                             ",
        "{   return rightRotate(node);}                                   ",
        "if (bl > 1 && node->left->val < key)                             ",
        "{   leftRotate(node->left);                                      ",
        "    rightRotate(node);}                                          ",
        "if (bl < -1 && node->right->val < key)                           ",
        "{   leftRotate(node);}                                           ",
        "if (bl < -1 && node->right->val > key)                           ",
        "{   rightRotate(node->right);                                    ",
        "    leftRotate(node);        }                                   "
    };
    Step step;
    for (int i = 0; i < box.someList.size(); ++i) {
        insertNode(root,box.someList[i],step);
    }
    box.someList.clear();
}

void AVL::deleteData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "if (node == nullptr) return;                                         ",
        "if (node->val > key) delete(node->left,key);                         ",
        "else if (node->val < key) delete(node->right,key);                   ",
        "else { if (!node->left)                                              ",
        "        {   Node* tmp = node->right;                                 ",
        "            delete node;                                             ",
        "            node = tmp;                 }                            ",
        "        if (!node->right)                                            ",
        "        {   Node* tmp = node->left;                                  ",
        "            delete node;                                             ",
        "            node = tmp;                }                             ",
        "        Node* succ = getSuccessor(node);                             ",
        "        node->val=succ->val;                                         ",
        "        deleteNode(node->right,succ->val,root,step);  }              ",
        "if (!node) return;                                                   ",
        "node->height = 1 + max(getHeight(node->left),getHeight(node->right));",
        "int bf = balanceFactor(node);                                        ",
        "if (bf > 1 && balanceFactor(node->left) >= 0)                        ",
        "{   rightRotate(node);}                                              ",
        "if (bf > 1 && balanceFactor(node->left) < 0)                         ",
        "{   leftRotate(node->left);                                          ",
        "    rightRotate(node);}                                              ",
        "if (bf < -1 && balanceFactor(node->right) <= 0)                      ",
        "{   leftRotate(node);}                                               ",
        "if (bf < -1 && balanceFactor(node->right) > 0)                       ",
        "{   rightRotate(node->right);                                        ",
        "    leftRotate(node);}                                               "
    };
    Step step;
    for (int i = 0; i < box.someList.size(); ++i) {
        deleteNode(root,box.someList[i],step);
    }
    box.someList.clear();
}

void AVL::searchData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "if (!root) return false;                                ",
        "if (root->val == key) return true;                      ",
        "else if (root->val > key) return search(root->left,key);",
        "else return search(root->right,key);                    "
    };
    Step step;
    for (int i = 0; i < box.someList.size(); ++i) {
        searchNode(root,box.someList[i],step);
    }
    std::cout<<stepmanager.step.size();
    for (int i = 0; i < stepmanager.step.size(); ++i) {
        for (auto x: stepmanager.step[i].description) {
            std::cout<<x<<" ";
        }
        std::cout<<std::endl;
    }
    box.someList.clear();
}

void AVL::insertNode(TreeNode* &node, int key, Step step) {
    TreeNode* tmp;
    if (node == nullptr) {
        step.highlightedLine = 0;
        step.highlightedNode = -1;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        node = new TreeNode(key);
        step.highlightedLine = 1;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        return;
    }
    if (node->val == key) {
        step.highlightedLine = 2;
        step.highlightedNode = node->val;
        step.description.push_back("");
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        return;
    }
    else if (node->val > key) {
        step.highlightedLine = 3;
        step.highlightedNode = node->val;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        insertNode(node->left,key,step);
    } else {
        step.highlightedLine = 4;
        step.highlightedNode = node->val;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        insertNode(node->right,key,step);
    }
    step.highlightedLine = 5;
    step.highlightedNode = node->val;
    copy(root,tmp);
    step.tempRoot = tmp;
    tmp = nullptr;
    stepmanager.step.push_back(step);
    step.highlightedLine = 6;
    step.highlightedNode = node->val;
    copy(root,tmp);
    step.tempRoot = tmp;
    tmp = nullptr;
    stepmanager.step.push_back(step);
    node->height = 1 + std::max(getHeight(node->left),getHeight(node->right));
    int bl = balanceFactor(node);

    if (bl > 1 && node->left->val > key) {
        step.highlightedLine = 7;
        step.highlightedNode = node->val;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        rightRotate(node);
        step.highlightedLine = -1;
        copy(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
    }

    if (bl > 1 && node->left->val < key) {
        step.highlightedLine = 8;
        step.highlightedNode = node->val;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        leftRotate(node->left);
        step.highlightedLine = -1;
        copy(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        rightRotate(node);
        step.highlightedLine = -1;
        copy(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
    }

    if (bl < -1 && node->right->val < key) {
        step.highlightedLine = 9;
        step.highlightedNode = node->val;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        leftRotate(node);
        step.highlightedLine = -1;
        copy(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
    }

    if (bl < -1 && node->right->val > key) {
        step.highlightedLine = 10;
        step.highlightedNode = node->val;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        rightRotate(node->right);
        step.highlightedLine = -1;
        copy(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        leftRotate(node);
        step.highlightedLine = -1;
        copy(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
    }
}

void AVL::deleteNode(TreeNode* &node, int key, Step step) {
    TreeNode* tmp;
    if (node == nullptr) {
        step.highlightedLine = 0;
        step.highlightedNode = -1;
        step.description.push_back("Number " + std::to_string(key) + " is not in the tree");
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        return;
    }
    std::cout<<node->val;
    if (node->val > key)  {
        step.highlightedLine = 1;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        deleteNode(node->left,key,step);
    }
    else if (node->val < key) {
        step.highlightedLine = 2;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        deleteNode(node->right,key,step);
    } else {
        if (!node->left) {
            step.highlightedLine = 3;
            step.highlightedNode = node->ID;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            step.highlightedLine = 4;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            TreeNode* tmp = node->right;
            delete node;
            node = tmp;
            std::cout<<"a";
            step.highlightedLine = 5;
            if (node) step.highlightedNode = node->ID;
            else step.highlightedNode = -1;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            step.highlightedLine = 6;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            return;
        }
        if (!node->right) {
            step.highlightedLine = 7;
            step.highlightedNode = node->ID;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            step.highlightedLine = 8;
            step.tempRoot = tmp;
            copy(root,tmp);
            tmp = nullptr;
            stepmanager.step.push_back(step);
            TreeNode* tmp = node->left;
            delete node;
            node = tmp;
            step.highlightedLine = 9;
            if (node) step.highlightedNode = node->ID;
            else step.highlightedNode = -1;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            step.highlightedLine = 10;
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            return;
        }
        step.highlightedLine = 11;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        step.highlightedLine = 12;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);        
        TreeNode* succ = getSuccessor(node);
        node->val=succ->val;
        deleteNode(node->right,succ->val,step);
    }
    if (!node) {
        step.highlightedLine = 13;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        return;
    }
    step.highlightedLine = 14;
    step.highlightedNode = node->ID;
    copy(root,tmp);
    step.tempRoot = tmp;
    tmp = nullptr;
    stepmanager.step.push_back(step);
    step.highlightedLine = 15;
    step.highlightedNode = node->ID;
    copy(root,tmp);
    step.tempRoot = tmp;
    tmp = nullptr;
    stepmanager.step.push_back(step);
    node->height = 1 + std::max(getHeight(node->left),getHeight(node->right));
    int bf = balanceFactor(node);
    if (bf > 1 && balanceFactor(node->left) >= 0) {
        step.highlightedLine = 16;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        rightRotate(node);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
    }
    if (bf > 1 && balanceFactor(node->left) < 0) {
        step.highlightedLine = 18;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        leftRotate(node->left);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        rightRotate(node);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
    }
    if (bf < -1 && balanceFactor(node->right) <= 0) {
        step.highlightedLine = 21;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        leftRotate(node);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
    }
    if (bf < -1 && balanceFactor(node->right) > 0) {
        step.highlightedLine = 23;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        rightRotate(node->right);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        leftRotate(node);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
    }
}

void AVL::searchNode(TreeNode* node, int key, Step step) {
    TreeNode* tmp = nullptr;
    if (!node) {
        step.highlightedLine = 0;
        step.highlightedNode = -1;
        step.description.push_back("Number " + std::to_string(key) + " is not in the tree");
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        return;
    }                                
    if (node->val == key) {
        step.highlightedLine = 1;
        step.highlightedNode = node->ID;
        step.description.push_back("Number " + std::to_string(key) + " is in the tree");
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        return;
    }                  
    else if (node->val > key) {
        step.highlightedLine = 2;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        searchNode(node->left,key,step);
    } 
    else {
        step.highlightedLine = 3;
        step.highlightedNode = node->ID;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        searchNode(node->right,key,step);
    }                   
}

void AVL::printInorder(TreeNode* node) {
    if (!node) return;
    printInorder(node->left);
    std::cout<<node->val<<" ";
    printInorder(node->right);
}