#include "AVL.hpp"

void AVL::init() {
    initView();
    root = nullptr;
    stepmanager.currentStep = 0;
    SetMousePosition(780,400);
    box.someList.clear();
}

void AVL::draw() {
    drawView();
    if (!root) return;
    if (!stepmanager.step.empty() && stepmanager.currentStep >= 0) {
        Step currStep = stepmanager.step[stepmanager.currentStep];
        log.infor = currStep.description;
        code.lineHighlighted = currStep.highlightedLine;
        BeginScissorMode(400,80,1040,640);
        BeginMode2D(camera);
        if (stepmanager.isTransitioning) {
            Animation currAnimation = currStep.animQueue.animation.front();
            Step& prevStep = stepmanager.step[stepmanager.currentStep - 1];
            if (currAnimation.type == AnimateType::DELETION) {
                resetColorNode((TreeNode*)prevStep.tempRoot);
                resetColorEdge(prevStep.tempEdge);
                drawEdge(prevStep.tempEdge);
                drawNode((TreeNode*)prevStep.tempRoot,currStep.highlightedNode);
            } else if (currAnimation.type == AnimateType::MOVEMENT) {
                resetColorNode((TreeNode*)prevStep.tempRoot);
                resetColorEdge(prevStep.tempEdge);
                drawEdge(prevStep.tempEdge);
                drawNode((TreeNode*)prevStep.tempRoot,currStep.highlightedNode);
            } else if (currAnimation.type == AnimateType::INSERTION) {
                resetColorNode((TreeNode*)currStep.tempRoot);
                resetColorEdge(currStep.tempEdge);
                drawEdge(currStep.tempEdge);
                drawNode((TreeNode*)currStep.tempRoot,currStep.highlightedNode);
            }
        } else {
            calculateSubtreeWidth((TreeNode*)currStep.tempRoot);
            calculatePositions((TreeNode*)currStep.tempRoot,980,120);
            resetAlphaEdge(currStep.tempEdge);
            resetAlphaNode((TreeNode*)currStep.tempRoot);
            resetColorNode((TreeNode*)currStep.tempRoot);
            resetColorEdge(currStep.tempEdge);
            drawEdge(currStep.tempEdge);
            drawNode((TreeNode*)currStep.tempRoot,currStep.highlightedNode);
        }
        EndMode2D();
        EndScissorMode();
    } else {
        BeginScissorMode(400,80,1040,640);
        BeginMode2D(camera);
        calculateSubtreeWidth(root);
        calculatePositions(root,980,120);
        resetColorNode(root);
        resetColorEdge(edge);
        drawEdge(edge);
        drawNode(root,-1);
        EndMode2D();
        EndScissorMode();
    }
}

int AVL::countRightmost(TreeNode* node) {
    if (!node) return 0;
    return 1 + countRightmost(node->right);
}

int AVL::countLeftmost(TreeNode* node) {
    if (!node) return 0;
    return 1 + countLeftmost(node->left);
}

int AVL::calculateSubtreeWidth(TreeNode* node) {
    if (!node) return 0;
    int leftWidth = calculateSubtreeWidth(node->left);
    int rightWidth = calculateSubtreeWidth(node->right);
    subtreeWidth[node] = subtreeWidth[node->left] + subtreeWidth[node->right];
    if (node->left || node->left) subtreeWidth[node] += 60; 
    return subtreeWidth[node];
}

void AVL::calculatePositions(TreeNode* node, int x, int y) {
    if (!node) return;

    node->position.x = x;
    node->position.y = y;

    int leftWidth = (node->left) ? subtreeWidth[node->left] + 30 : 0;
    int rightWidth = (node->right) ? subtreeWidth[node->right] + 30 : 0;
    if (node->left) calculatePositions(node->left, x - leftWidth, y + 80);
    if (node->right) calculatePositions(node->right, x + rightWidth, y + 80);
}

void AVL::drawEdge(std::vector<Edge*> edge) {
    for (int i = 0; i < edge.size(); ++i) {
        edge[i]->draw();
    }
}

void AVL::resetAlphaNode(TreeNode* node) {
    if (!node) return;
    if (theme == colorType::HOT) {
        node->currentColor = myColor1[0];
        node->targetColor = myColor1[2];
    } else {
        node->currentColor = myColor2[0];
        node->targetColor = myColor2[2];
    }
    node->alpha = 1.0f;
    resetAlphaNode(node->left);
    resetAlphaNode(node->right);
}

void AVL::resetAlphaEdge(std::vector<Edge*> edge) {
    for (int i = 0; i < edge.size(); ++i) {
        if (theme == colorType::HOT) {
            edge[i]->currentColor = myColor1[1];
            edge[i]->targetColor = myColor1[2];
        } else {
            edge[i]->currentColor = myColor2[1];
            edge[i]->targetColor = myColor2[2];
        }
        edge[i]->alpha = 1.0f;
    }
}

void AVL::resetColorNode(TreeNode* node) {
    if (!node) return;
    if (theme == colorType::HOT) {
        node->currentColor = myColor1[0];
        node->targetColor = myColor1[2];
    } else {
        node->currentColor = myColor2[0];
        node->targetColor = myColor2[2];
    }
    resetColorNode(node->left);
    resetColorNode(node->right);
}

void AVL::resetColorEdge(std::vector<Edge*> edge) {
    for (int i = 0; i < edge.size(); ++i) {
        if (theme == colorType::HOT) {
            edge[i]->currentColor = myColor1[1];
            edge[i]->targetColor = myColor1[2];
        } else {
            edge[i]->currentColor = myColor2[1];
            edge[i]->targetColor = myColor2[2];
        }
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
    
    if (option.isInitialize()){
        func = Function::INIT;
        box.IniFunction = true;
    } 
    if (!box.IniFunction){
        if (option.isAdd()) func = Function::INSERT;
        if (option.isDelete()) func = Function::DELETE;
        if (option.isSearch()) func = Function::SEARCH;
    }

    if (!box.isOpen && func != Function::NONE) {
        switch (func) {
            case Function::INIT:
                exit();
                initData();
                func = Function::NONE;
                box.IniFunction = false;
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
    stepmanager.speed = slider.speed; // speed increment

    if (stepmanager.isTransitioning) {
        stepmanager.updateTransitionProgress();
        if (stepmanager.isTransitionComplete()) stepmanager.finishTransition();
    } else if (panel.isForwardPressed()) {
        stepmanager.isPlaying = false;
        panel.isPlaying = false;
        if (stepmanager.currentStep < stepmanager.step.size() - 1) {
            stepmanager.nextStep();
            prepareTransition();
        }
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
    auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastUpdateTime).count();
    lastUpdateTime = now;
    if (stepmanager.isPlaying && stepmanager.currentStep < stepmanager.step.size() - 1) {
        accumulatedTime += deltaTime * stepmanager.speed;
        while (accumulatedTime >= stepDuration && stepmanager.isPlaying) {
            accumulatedTime -= stepDuration;
            if (stepmanager.isTransitioning) {
                stepmanager.updateTransitionProgress();
                if (stepmanager.isTransitionComplete()) stepmanager.finishTransition();
            } else {
                stepmanager.nextStep();
                prepareTransition();
            }
        }
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
    calculateSubtreeWidth((TreeNode*)currStep.tempRoot);
    calculatePositions((TreeNode*)currStep.tempRoot,980,120);
    calculateSubtreeWidth((TreeNode*)prevStep.tempRoot);
    calculatePositions((TreeNode*)prevStep.tempRoot,980,120);
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
    TreeNode* parent = findParent(root,node->ID);
    if (parent) {
        safeRemoveEdge(node->ID);
        Edge* line = findEdgebyEndPoint(edge,x->ID);
        std::swap(line->endPoint1,line->endPoint2);
        line = new Edge(parent,x);
        edge.push_back(line);
    } else {
        Edge* line = findEdgebyEndPoint(edge,x->ID);
        std::swap(line->endPoint1,line->endPoint2);
    }
    x->left=node;
    node->right=z;
    if (z) {
        safeRemoveEdge(z->ID);
        Edge* line = new Edge(node,z);
        edge.push_back(line);
    }
    node->height=1+std::max(getHeight(node->left),getHeight(node->right));
    x->height=1+std::max(getHeight(x->left),getHeight(x->right));
    node = x;
}

void AVL::rightRotate(TreeNode* &node) {
    TreeNode* x = node->left;
    TreeNode* z = x->right;
    TreeNode* parent = findParent(root,node->ID);
    if (parent) {
        safeRemoveEdge(node->ID);
        Edge* line = findEdgebyEndPoint(edge,x->ID);
        std::swap(line->endPoint1,line->endPoint2);
        line = new Edge(parent,x);
        edge.push_back(line);
    } else {
        Edge* line = findEdgebyEndPoint(edge,x->ID);
        std::swap(line->endPoint1,line->endPoint2);
    }
    x->right=node;
    node->left=z;
    if (z) {
        safeRemoveEdge(z->ID);
        Edge* line = new Edge(node,z);
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
        "if !n: return new Node(k)                          ",
        "if k == n.val: return n                            ",
        "k < n.val ? insert(n.left,k) : insert(n.right,k)   ",
        "n.height = 1 + max(height(n.left), height(n.right))",
        "b = balance(n)                                     ",
        "if b > 1:                                          ",
        "  if k < n.left.val: rightRot(n)                   ",
        "  else: leftRot(n.left), rightRot(n)               ",
        "if b < -1:                                         ",
        "  if k > n.right.val: leftRot(n)                   ",
        "  else: rightRot(n.right), leftRot(n)              ",
        "return n                                           "
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
        "if !n: return                                      ",
        "if k < n.val: delete(n.left, k)                    ",
        "else if k > n.val: delete(n.right, k)              ",
        "else:                                              ",
        "  if !n.left: n = n.right                          ",
        "  else if !n.right: n = n.left                     ",
        "  else:                                            ",
        "    s = getSuccessor(n)                            ",
        "    n.val = s.val                                  ",
        "    delete(n.right, s.val)                         ",
        "if !n: return                                      ",
        "n.height = 1 + max(height(n.left), height(n.right))",
        "bf = balance(n)                                    ",
        "if bf > 1:                                         ",
        "  if balance(n.left) >= 0: rightRot(n)             ",
        "  else: leftRot(n.left), rightRot(n)               ",
        "if bf < -1:                                        ",
        "  if balance(n.right) <= 0: leftRot(n)             ",
        " else: rightRot(n.right), leftRot(n)               "
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
        "if !root: return false                         ",
        "if root.val == key: return true                ",
        "if key < root.val: return search(root.left,key)",
        "else: return search(root.right,key)            ",
    };
    Step step;
    for (int i = 0; i < box.someList.size(); ++i) {
        searchNode(root,box.someList[i],step);
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
        step.highlightedLine = 0;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        return;
    }
    if (node->val == key) {
        step.highlightedLine = 1;
        step.highlightedNode = node->ID;
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
        insertNode(node->left,key,step);
    } else {
        step.highlightedLine = 2;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        insertNode(node->right,key,step);
    }
    step.highlightedLine = 3;
    step.highlightedNode = node->ID;
    copyNode(root,tmp);
    step.tempRoot = tmp;
    tmp = nullptr;
    copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
    stepmanager.step.push_back(step);
    step.highlightedLine = 4;
    step.highlightedNode = node->ID;
    copyNode(root,tmp);
    step.tempRoot = tmp;
    tmp = nullptr;
    copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
    stepmanager.step.push_back(step);
    node->height = 1 + std::max(getHeight(node->left),getHeight(node->right));
    int bl = balanceFactor(node);

    if (bl > 1 && node->left->val > key) {
        rightRotate(node);
        step.highlightedLine = 6;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        return;
    }

    if (bl > 1 && node->left->val < key) {
        leftRotate(node->left);
        step.highlightedLine = 7;
        step.highlightedNode = node->left->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        rightRotate(node);
        step.highlightedNode = node->ID;
        copyNode(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        return;
    }

    if (bl < -1 && node->right->val < key) {
        leftRotate(node);
        step.highlightedLine = 9;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        return;
    }

    if (bl < -1 && node->right->val > key) {
        rightRotate(node->right);
        step.highlightedLine = 10;
        step.highlightedNode = node->right->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        leftRotate(node);
        step.highlightedNode = node->ID;
        copyNode(root, tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        return;
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
            step.highlightedLine = 4;
            step.highlightedNode = node->ID;
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
            step.highlightedLine = 4;
            if (node) step.highlightedNode = node->ID;
            else step.highlightedNode = -1;
            copyNode(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
            stepmanager.step.push_back(step);
            return;
        }
        if (!node->right) {
            step.highlightedLine = 5;
            step.highlightedNode = node->ID;
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
            step.highlightedLine = 5;
            if (node) step.highlightedNode = node->ID;
            else step.highlightedNode = -1;
            copyNode(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
            stepmanager.step.push_back(step);
            return;
        }
        step.highlightedLine = 7;
        step.highlightedNode = node->ID;
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
        step.highlightedLine = 10;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
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
    node->height = 1 + std::max(getHeight(node->left),getHeight(node->right));
    int bf = balanceFactor(node);
    if (bf > 1 && balanceFactor(node->left) >= 0) {
        rightRotate(node);
        step.highlightedLine = 14;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
    }
    if (bf > 1 && balanceFactor(node->left) < 0) {
        leftRotate(node->left);
        step.highlightedLine = 15;
        step.highlightedNode = node->left->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        rightRotate(node);
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
    }
    if (bf < -1 && balanceFactor(node->right) <= 0) {
        leftRotate(node);
        step.highlightedLine = 17;
        step.highlightedNode = node->ID;
        copyNode(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        copyEdge(edge,step.tempEdge,(TreeNode*)step.tempRoot);
        stepmanager.step.push_back(step);
        
    }
    if (bf < -1 && balanceFactor(node->right) > 0) {
        rightRotate(node->right);
        step.highlightedLine = 18;
        step.highlightedNode = node->right->ID;
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
        stepmanager.step.push_back(step);
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