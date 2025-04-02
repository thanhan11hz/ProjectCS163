#include "AVL.hpp"

void AVL::init() {
    initView();
    root = nullptr;
    stepmanager.currentStep = 0;
    SetMousePosition(780,400);
}

void AVL::draw() {
    drawView();
    if (!root) return;
    if (!stepmanager.step.empty() && stepmanager.currentStep >= 0) {
        Step currStep = stepmanager.step[stepmanager.currentStep];
        log.infor = currStep.description;
        code.lineHighlighted = currStep.highlightedLine;
        drawView();
        if (stepmanager.isTransitioning) {
            Animation currAnimation = currStep.animQueue.animation.front();
            Step& prevStep = stepmanager.step[stepmanager.currentStep - 1];
            if (currAnimation.type == AnimateType::DELETION) {
                drawNode((TreeNode*)prevStep.tempRoot,currStep.highlightedNode);
                drawEdge(prevStep.tempEdge);
            } else if (currAnimation.type == AnimateType::MOVEMENT) {
                drawNode((TreeNode*)prevStep.tempRoot,currStep.highlightedNode);
                drawEdge(prevStep.tempEdge);
            } else if (currAnimation.type == AnimateType::INSERTION) {
                drawNode((TreeNode*)currStep.tempRoot,currStep.highlightedNode);
                std::cout<<currStep.tempEdge.size();
                drawEdge(currStep.tempEdge);
            }
        } else {
            calculatePositions((TreeNode*)currStep.tempRoot,980,120,0);
            drawNode((TreeNode*)currStep.tempRoot,currStep.highlightedNode);
            drawEdge(currStep.tempEdge);
        }
    } else {
        calculatePositions(root,980,120,0);
        drawEdge(edge);
        drawNode(root,-1);
    }
}

void AVL::calculatePositions(TreeNode* node, int x, int y, int level) {
    if (!node) return;
    node->position.x = x;
    node->position.y = y;
    int treeHeight = getHeight(root); 
    int offsetX = 100 / (1 << level); 
    if (node->left) calculatePositions(node->left, x - offsetX, y + 100, level + 1);
    if (node->right) calculatePositions(node->right, x + offsetX, y + 100, level + 1);
}

void AVL::drawEdge(std::vector<Edge*> edge) {
    for (int i = 0; i < edge.size(); ++i) {
        edge[i]->draw();
    }
}

void AVL::drawNode(TreeNode* node, int highlightNode) {
    if (!node) return;
    if (node->ID == highlightNode) node->drawHighlight();
    else node->draw();
    drawNode(node->left,highlightNode);
    drawNode(node->right,highlightNode);
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

    if (stepmanager.isTransitioning) {
        stepmanager.updateTransitionProgress();
        if (stepmanager.isTransitionComplete()) stepmanager.finishTransition();
    } else if (panel.isForwardPressed()) {
        stepmanager.isPlaying = false;
        panel.isPlaying = false;
        stepmanager.nextStep();
        prepareTransition();
    } else if (panel.isRewindPressed()) {
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
        if (stepmanager.isTransitioning) {
            stepmanager.updateTransitionProgress();
            if (stepmanager.isTransitionComplete()) stepmanager.finishTransition();
        } else {
            stepmanager.nextStep();
            prepareTransition();
        }
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
    for (int i = 0; i < stepmanager.step.size(); ++i) {
        deleteTree((TreeNode*)stepmanager.step[i].tempRoot);
        stepmanager.step[i].tempRoot = nullptr;
        for (int j = 0; j < stepmanager.step[i].tempEdge.size(); ++j) {
            delete stepmanager.step[i].tempEdge[j];
        }
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
    for (int j = 0; j < edge.size(); ++j) {
        delete edge[j];
        edge[j] = nullptr;
    }
    for (int i = 0; i < stepmanager.step.size(); ++i) {
        deleteTree((TreeNode*)stepmanager.step[i].tempRoot);
        stepmanager.step[i].tempRoot = nullptr;
        for (int j = 0; j < stepmanager.step[i].tempEdge.size(); ++j) {
            delete stepmanager.step[i].tempEdge[j];
            stepmanager.step[i].tempEdge[j] = nullptr;
        }
    }
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
    edge.clear();
}

void AVL::copyNode(TreeNode* source, TreeNode* &des) {
    if (!source) {
        des = nullptr;
        return;
    }
    des = new TreeNode(source->val);
    des->position = source->position;
    des->ID = source->ID;
    copyNode(source->left,des->left);
    copyNode(source->right,des->right);
}

void AVL::copyEdge(std::vector<Edge*> source, std::vector<Edge*> &des, TreeNode* head) {
    if (source.size() == 0) {
        des.clear();
        return;
    }
    des.resize(source.size());
    for (int i = 0; i < source.size(); ++i) {
        des[i] = new Edge(findNodebyID(head,source[i]->endPoint1->ID),findNodebyID(head,source[i]->endPoint2->ID));
        des[i]->ID = source[i]->ID;
    }
}

AVL::TreeNode* AVL::findNodebyID(TreeNode* head, int ID) {
    if (!head) return nullptr;
    if (head->ID == ID) return head;
    TreeNode* leftNode = findNodebyID(head->left,ID);
    if (leftNode) return leftNode;
    TreeNode* rightNode = findNodebyID(head->right,ID);
    if (rightNode) return rightNode;
    return nullptr;
}

Edge* AVL::findEdgebyEndPoint(std::vector<Edge*> list, int endPoint2ID) {
    if (list.size() == 0) return nullptr;
    for (int i = 0; i < list.size(); ++i) {
        if (list[i]->endPoint2->ID == endPoint2ID) return list[i];
    }
    return nullptr;
}

void AVL::prepareTransition() {
    Step& currStep = stepmanager.step[stepmanager.currentStep];
    if (stepmanager.currentStep <= 0) return;
    Step& prevStep = stepmanager.step[stepmanager.currentStep - 1];
    calculatePositions((TreeNode*)currStep.tempRoot,980,120,0);
    calculatePositions((TreeNode*)prevStep.tempRoot,980,120,0);
    std::unordered_map<int,TreeNode*> currNode;
    std::unordered_map<int,TreeNode*> prevNode;
    std::unordered_map<int,Edge*> currEdge;
    std::unordered_map<int,Edge*> prevEdge;
    scanNode((TreeNode*)currStep.tempRoot,currNode);
    scanNode((TreeNode*)prevStep.tempRoot,prevNode);
    for (int i = 0; i < currStep.tempEdge.size(); ++i) {
        currEdge[currStep.tempEdge[i]->ID] = currStep.tempEdge[i];
    }
    for (int i = 0; i < prevStep.tempEdge.size(); ++i) {
        prevEdge[prevStep.tempEdge[i]->ID] = prevStep.tempEdge[i];
    }
    Animation anim;
    anim.type = AnimateType::DELETION;
    for (auto it : prevNode) {
        if (currNode.find(it.first) == currNode.end()) {
            anim.deletedNode.push_back(it.second);
        }
    }
    for (auto it : prevEdge) {
        if (currEdge.find(it.first) == currEdge.end()) {
            anim.deletedEdge.push_back(it.second);
        }
    }
    if (anim.deletedEdge.size() || anim.deletedNode.size()) currStep.animQueue.addAnimation(anim);
    anim.type = AnimateType::MOVEMENT;
    for (auto it : prevNode) {
        if (currNode.find(it.first) != currNode.end()) {
            anim.movedNode.push_back({it.second,currNode.find(it.first)->second});
        }
    }
    if (anim.movedNode.size()) currStep.animQueue.addAnimation(anim);
    anim.type = AnimateType::INSERTION;
    for (auto it : currNode) {
        if (prevNode.find(it.first) == prevNode.end()) {
            it.second->alpha = 0.0f;
            anim.insertedNode.push_back(it.second);
        }
    }
    for (auto it : currEdge) {
        if (prevEdge.find(it.first) == prevEdge.end()) {
            it.second->alpha = 0.0f;
            anim.insertedEdge.push_back(it.second);
        }
    }
    if (anim.insertedNode.size() || anim.insertedEdge.size()) currStep.animQueue.addAnimation(anim);
}

void AVL::scanNode(TreeNode* root, std::unordered_map<int,TreeNode*> &node) {
    if (!root) return;
    node[root->ID] = root;
    scanNode(root->left,node);
    scanNode(root->right,node);
}

AVL::TreeNode* AVL::findParent(TreeNode* node, int ID) {
    if (!node || node->ID == ID) return nullptr;
    if (node->left && node->left->ID == ID) return node;
    if (node->right && node->right->ID == ID) return node;
    TreeNode* leftNode = findParent(node->left,ID);
    if (leftNode) return leftNode;
    return findParent(node->right,ID);
}

void AVL::safeRemoveEdge(int nodeID) {
    Edge* edgeToRemove = findEdgebyEndPoint(edge, nodeID);
    if (edgeToRemove) {
        auto it = find(edge.begin(), edge.end(), edgeToRemove);
        if (it != edge.end()) {
            delete *it;
            edge.erase(it);
        }
    }
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
    TreeNode* z = x->left;
    x->left=node;
    safeRemoveEdge(x->ID);
    Edge *line = new Edge(x,node);
    edge.push_back(line);
    TreeNode* parent = findParent(root,node->ID);
    if (parent) {
        safeRemoveEdge(node->ID);
        line = new Edge(parent,x);
        edge.push_back(line);
    }
    node->right=z;
    if (z) {
        safeRemoveEdge(z->ID);
        line = new Edge(node,z);
        edge.push_back(line);
    }
    node->height=1+std::max(getHeight(node->left),getHeight(node->right));
    x->height=1+std::max(getHeight(x->left),getHeight(x->right));
    node = x;
}

void AVL::rightRotate(TreeNode* &node) {
    TreeNode* x = node->left;
    TreeNode* z = x->right;
    x->right=node;
    safeRemoveEdge(x->ID);
    Edge *line = new Edge(x,node);
    edge.push_back(line);
    TreeNode* parent = findParent(root,node->ID);
    if (parent) {
        safeRemoveEdge(node->ID);
        line = new Edge(parent,x);
        edge.push_back(line);
    }
    node->left=z;
    if (z) {
        safeRemoveEdge(z->ID);
        line = new Edge(node,z);
        edge.push_back(line);
    }
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
    remove();
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
    // for (int i = 0; i < stepmanager.step.size(); ++i) {
    //     for (int j = 0; j < stepmanager.step[i].tempEdge.size(); ++j) {
    //         std::cout<<stepmanager.step[i].tempEdge[j]->ID<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
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
        copyNode(root,tmp); 
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        node = new TreeNode(key);
        TreeNode* parent = findParent(root,node->ID);
        if (parent) {
            Edge* line = new Edge(parent,node);
            edge.push_back(line);
        }
        step.highlightedLine = 1;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        return;
    }
    if (node->val == key) {
        step.highlightedLine = 2;
        step.highlightedNode = node->val;
        step.description.push_back("");
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        return;
    }
    else if (node->val > key) {
        step.highlightedLine = 3;
        step.highlightedNode = node->val;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        insertNode(node->left,key,step);
    } else {
        step.highlightedLine = 4;
        step.highlightedNode = node->val;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        insertNode(node->right,key,step);
    }
    step.highlightedLine = 5;
    step.highlightedNode = node->val;
    copyNode(root,tmp);
    step.tempRoot = tmp;
    tmp = nullptr;
    copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
    stepmanager.step.push_back(step);
    step.highlightedLine = 6;
    step.highlightedNode = node->val;
    copyNode(root,tmp);
    step.tempRoot = tmp;
    tmp = nullptr;
    copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
    stepmanager.step.push_back(step);
    node->height = 1 + std::max(getHeight(node->left),getHeight(node->right));
    int bl = balanceFactor(node);

    if (bl > 1 && node->left->val > key) {
        step.highlightedLine = 7;
        step.highlightedNode = node->val;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        rightRotate(node);
        step.highlightedLine = -1;
        copyNode(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
    }

    if (bl > 1 && node->left->val < key) {
        step.highlightedLine = 8;
        step.highlightedNode = node->val;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        leftRotate(node->left);
        step.highlightedLine = -1;
        copyNode(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        rightRotate(node);
        step.highlightedLine = -1;
        copyNode(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
    }

    if (bl < -1 && node->right->val < key) {
        step.highlightedLine = 9;
        step.highlightedNode = node->val;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        leftRotate(node);
        step.highlightedLine = -1;
        copyNode(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
    }

    if (bl < -1 && node->right->val > key) {
        step.highlightedLine = 10;
        step.highlightedNode = node->val;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        rightRotate(node->right);
        step.highlightedLine = -1;
        copyNode(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        leftRotate(node);
        step.highlightedLine = -1;
        copyNode(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
    }
}

void AVL::deleteNode(TreeNode* &node, int key, Step step) {
    TreeNode* tmp;
    if (node == nullptr) {
        step.highlightedLine = 0;
        step.highlightedNode = -1;
        step.description.push_back("Number " + std::to_string(key) + " is not in the tree");
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        return;
    }
    if (node->val > key)  {
        step.highlightedLine = 1;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        deleteNode(node->left,key,step);
    }
    else if (node->val < key) {
        step.highlightedLine = 2;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        deleteNode(node->right,key,step);
    } else {
        if (!node->left) {
            step.highlightedLine = 3;
            step.highlightedNode = node->ID;
            copyNode(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
            stepmanager.step.push_back(step);
            step.highlightedLine = 4;
            copyNode(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
            stepmanager.step.push_back(step);
            TreeNode* tmp = node->right;
            safeRemoveEdge(node->ID);
            if (node->right) {
                safeRemoveEdge(node->right->ID);
                Edge* line = new Edge(findParent(root,node->ID),node->right);
                edge.push_back(line);
            }
            delete node;
            node = tmp;
            step.highlightedLine = 5;
            if (node) step.highlightedNode = node->ID;
            else step.highlightedNode = -1;
            copyNode(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
            stepmanager.step.push_back(step);
            step.highlightedLine = 6;
            copyNode(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
            stepmanager.step.push_back(step);
            return;
        }
        if (!node->right) {
            step.highlightedLine = 7;
            step.highlightedNode = node->ID;
            copyNode(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
            stepmanager.step.push_back(step);
            step.highlightedLine = 8;
            copyNode(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
            stepmanager.step.push_back(step);
            TreeNode* tmp = node->left;
            safeRemoveEdge(node->ID);
            if (node->left) {
                safeRemoveEdge(node->left->ID);
                Edge* line = new Edge(findParent(root,node->ID),node->left);
                edge.push_back(line);
            }
            delete node;
            node = tmp;
            step.highlightedLine = 9;
            if (node) step.highlightedNode = node->ID;
            else step.highlightedNode = -1;
            copyNode(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
            stepmanager.step.push_back(step);
            step.highlightedLine = 10;
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            return;
        }
        step.highlightedLine = 11;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        step.highlightedLine = 12;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);        
        TreeNode* succ = getSuccessor(node);
        node->val=succ->val;
        deleteNode(node->right,succ->val,step);
    }
    if (!node) {
        step.highlightedLine = 13;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        return;
    }
    step.highlightedLine = 14;
    step.highlightedNode = node->ID;
    copyNode(root,tmp);
    step.tempRoot = tmp;
    tmp = nullptr;
    copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
    stepmanager.step.push_back(step);
    step.highlightedLine = 15;
    step.highlightedNode = node->ID;
    copyNode(root,tmp);
    step.tempRoot = tmp;
    tmp = nullptr;
    copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
    stepmanager.step.push_back(step);
    node->height = 1 + std::max(getHeight(node->left),getHeight(node->right));
    int bf = balanceFactor(node);
    if (bf > 1 && balanceFactor(node->left) >= 0) {
        step.highlightedLine = 16;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        rightRotate(node);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
    }
    if (bf > 1 && balanceFactor(node->left) < 0) {
        step.highlightedLine = 18;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        leftRotate(node->left);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        rightRotate(node);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
    }
    if (bf < -1 && balanceFactor(node->right) <= 0) {
        step.highlightedLine = 21;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        leftRotate(node);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
    }
    if (bf < -1 && balanceFactor(node->right) > 0) {
        step.highlightedLine = 23;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        rightRotate(node->right);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        leftRotate(node);
        step.highlightedLine = -1;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
    }
}

void AVL::searchNode(TreeNode* node, int key, Step step) {
    TreeNode* tmp = nullptr;
    if (!node) {
        step.highlightedLine = 0;
        step.highlightedNode = -1;
        step.description.push_back("Number " + std::to_string(key) + " is not in the tree");
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        return;
    }                                
    if (node->val == key) {
        step.highlightedLine = 1;
        step.highlightedNode = node->ID;
        step.description.push_back("Number " + std::to_string(key) + " is in the tree");
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        return;
    }                  
    else if (node->val > key) {
        step.highlightedLine = 2;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        searchNode(node->left,key,step);
    } 
    else {
        step.highlightedLine = 3;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
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