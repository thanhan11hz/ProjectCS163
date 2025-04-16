#include "HTable.hpp"
using namespace std;

void HTable::init() {
    initView();
    HSvalue.clear();
    stepmanager.currentStep = 0;
    box.someList.clear();
}

void HTable::draw() {
    drawView();
    if (HSvalue.empty()) return;
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
                resetColorNode(prevStep.tempTable);
                resetColorEdge(prevStep.tempEdge);
                drawEdge(prevStep.tempEdge);
                drawNode(prevStep.tempTable,currStep.highlightedNode);
            } else if (currAnimation.type == AnimateType::MOVEMENT) {
                resetColorNode(prevStep.tempTable);
                resetColorEdge(prevStep.tempEdge);
                drawEdge(prevStep.tempEdge);
                drawNode(prevStep.tempTable,currStep.highlightedNode);
            } else if (currAnimation.type == AnimateType::INSERTION) {
                resetColorNode(currStep.tempTable);
                resetColorEdge(currStep.tempEdge);
                drawEdge(currStep.tempEdge);
                drawNode(currStep.tempTable,currStep.highlightedNode);
            }
        } else {
            calculatePosition(currStep.tempTable);
            resetAlphaEdge(currStep.tempEdge);
            resetAlphaNode(currStep.tempTable);
            resetColorNode(currStep.tempTable);
            resetColorEdge(currStep.tempEdge);
            drawEdge(currStep.tempEdge);
            drawNode(currStep.tempTable,currStep.highlightedNode);
            code.lineHighlighted = -1;
        }
        EndMode2D();
        EndScissorMode();
    } else {
        BeginScissorMode(400,80,1040,640);
        BeginMode2D(camera);
        calculatePosition(HSvalue);
        resetColorNode(HSvalue);
        resetColorEdge(edge);
        drawEdge(edge);
        drawNode(HSvalue,-1);
        EndMode2D();
        EndScissorMode();
    }
}

void HTable::calculatePosition(std::vector<ListNode*> table) {
    int offsetY = 200;
    int cellWidth = 60;
    int cellHeight = 40;
    int gap = 10;
    int whiteAreaX = 400;
    int whiteAreaWidth = 1440 - whiteAreaX;
    int totalWidth = box.primeNumber * cellWidth + (box.primeNumber - 1) * gap;
    int offsetX = whiteAreaX + (whiteAreaWidth - totalWidth) / 2;
    for (int i = 0; i < table.size(); ++i) {
        ListNode* curr = table[i];
        int nodeOffsetY = offsetY + cellHeight + 20;
        while (curr) {
            curr->position = {
                (float)(offsetX + i * (cellWidth + gap) + cellWidth / 2),
                (float)(nodeOffsetY + 20)
            };
            curr = curr->next;
            nodeOffsetY += 80;
        }
    }
}

void HTable::calculatePosition(std::vector<Node*> table) {
    int offsetY = 200;
    int cellWidth = 60;
    int cellHeight = 40;
    int gap = 10;
    int whiteAreaX = 400;
    int whiteAreaWidth = 1440 - whiteAreaX;
    int totalWidth = box.primeNumber * cellWidth + (box.primeNumber - 1) * gap;
    int offsetX = whiteAreaX + (whiteAreaWidth - totalWidth) / 2;
    for (int i = 0; i < table.size(); ++i) {
        ListNode* curr = (ListNode*)table[i];
        int nodeOffsetY = offsetY + cellHeight + 20;
        while (curr) {
            curr->position = {
                (float)(offsetX + i * (cellWidth + gap) + cellWidth / 2),
                (float)(nodeOffsetY + 20)
            };
            curr = curr->next;
            nodeOffsetY += 80;
        }
    }
}

void HTable::drawNode(std::vector<ListNode*> table, int highlight) {
    int offsetY = 200;
    int cellWidth = 60;
    int cellHeight = 40;
    int gap = 10;
    int whiteAreaX = 400;
    int whiteAreaWidth = 1440 - whiteAreaX;
    int totalWidth = box.primeNumber * cellWidth + (box.primeNumber - 1) * gap;
    int offsetX = whiteAreaX + (whiteAreaWidth - totalWidth) / 2;
    for (int i = 0; i < table.size(); ++i) {
        if (theme == colorType::HOT) DrawRectangle(offsetX + i * (cellWidth + gap), offsetY, cellWidth, cellHeight, myColor1[0]);
        else DrawRectangle(offsetX + i * (cellWidth + gap), offsetY, cellWidth, cellHeight, myColor2[0]);
        Vector2 textSize = MeasureTextEx(GetFontDefault(),std::to_string(i).c_str(),20,2);
        Vector2 textPos = {
            (float)(offsetX + i * (cellWidth + gap) + (cellWidth - textSize.x) / 2.0f),
            (float)(offsetY + (cellHeight - textSize.y) / 2.0f)
        };
        DrawTextEx(GetFontDefault(), std::to_string(i).c_str(), textPos, 20, 2, BLACK);
        ListNode* curr = table[i];
        while (curr) {
            if (curr->ID == highlight) curr->drawHighlight();
            else curr->draw();
            curr = curr->next;
        }
    }
}

void HTable::drawNode(std::vector<Node*> table, int highlight) {
    int offsetY = 200;
    int cellWidth = 60;
    int cellHeight = 40;
    int gap = 10;
    int whiteAreaX = 400;
    int whiteAreaWidth = 1440 - whiteAreaX;
    int totalWidth = box.primeNumber * cellWidth + (box.primeNumber - 1) * gap;
    int offsetX = whiteAreaX + (whiteAreaWidth - totalWidth) / 2;
    for (int i = 0; i < table.size(); ++i) {
        if (theme == colorType::HOT) DrawRectangle(offsetX + i * (cellWidth + gap), offsetY, cellWidth, cellHeight, myColor1[0]);
        else DrawRectangle(offsetX + i * (cellWidth + gap), offsetY, cellWidth, cellHeight, myColor2[0]);
        Vector2 textSize = MeasureTextEx(GetFontDefault(),std::to_string(i).c_str(),20,2);
        Vector2 textPos = {
            (float)(offsetX + i * (cellWidth + gap) + (cellWidth - textSize.x) / 2.0f),
            (float)(offsetY + (cellHeight - textSize.y) / 2.0f)
        };
        DrawTextEx(GetFontDefault(), std::to_string(i).c_str(), textPos, 20, 2, BLACK);
        ListNode* curr = (ListNode*)table[i];
        while (curr) {
            if (curr->ID == highlight) curr->drawHighlight();
            else curr->draw();
            curr = curr->next;
        }
    }
}

void HTable::drawEdge(std::vector<Edge*> edge) {
    for (int i = 0; i < edge.size(); ++i) {
        edge[i]->draw();
    }
}

void HTable::resetAlphaNode(std::vector<Node*> table) {
    for (int i = 0; i < table.size(); ++i) {
        if (!table[i]) continue;
        ListNode* curr = (ListNode*) table[i];
        while (curr) {
            if (theme == colorType::HOT) {
                curr->currentColor = myColor1[0];
                curr->targetColor = myColor1[2];
            } else {
                curr->currentColor = myColor2[0];
                curr->targetColor = myColor2[2];
            }
            curr->alpha = 1.0f;
            curr = curr->next;
        }
    }
}

void HTable::resetAlphaEdge(std::vector<Edge*> edge) {
    for (int i = 0; i < edge.size(); ++i) {
        edge[i]->alpha = 1.0f;
    }
}

void HTable::resetColorNode(std::vector<Node*> table) {
    for (int i = 0; i < table.size(); ++i) {
        if (!table[i]) continue;
        ListNode* curr = (ListNode*) table[i];
        while (curr) {
            if (theme == colorType::HOT) {
                curr->currentColor = myColor1[0];
                curr->targetColor = myColor1[2];
            } else {
                curr->currentColor = myColor2[0];
                curr->targetColor = myColor2[2];
            }
            curr = curr->next;
        }
    }
}

void HTable::resetColorNode(std::vector<ListNode*> table) {
    for (int i = 0; i < table.size(); ++i) {
        if (!table[i]) continue;
        ListNode* curr = table[i];
        while (curr) {
            if (theme == colorType::HOT) {
                curr->currentColor = myColor1[0];
                curr->targetColor = myColor1[2];
            } else {
                curr->currentColor = myColor2[0];
                curr->targetColor = myColor2[2];
            }
            curr = curr->next;
        }
    }
}

void HTable::resetColorEdge(std::vector<Edge*> edge) {
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

void HTable::run() {
    eventView();

    if (option.isInitialize()) {
        box.enteredValues = false;
        box.enteredPrime = false;
        func = Function::INIT;
        box.IniFunction = true;
    }
    if (!box.IniFunction){
        if (option.isAdd()) {
            box.enteredValues = false;
            box.enteredPrime = true;
            func = Function::INSERT;
        }
        if (option.isDelete()) {
            box.enteredValues = false;
            box.enteredPrime = true;
            func = Function::DELETE;
        }
        if (option.isSearch()) {
            box.enteredValues = false;
            box.enteredPrime = true;
            func = Function::SEARCH;
        }
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
    stepmanager.speed = slider.speed; // speed increament


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
        accumulatedTime = 0.0f; 
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

void HTable::exit() {
    code.codeline.clear();
    log.infor.clear();
    for (auto x : HSvalue){
        if (x == nullptr) continue;
        else {
            while (x != nullptr){
                ListNode *del = x;
                x = x->next;
                delete del;
            }
        }
    }
    for (int j = 0; j < edge.size(); ++j) {
        delete edge[j];
        edge[j] = nullptr;
    }
    for (int i = 0; i < stepmanager.step.size(); ++i) {
        for (int j = 0; j < stepmanager.step[i].tempTable.size(); ++j) {
            if (!stepmanager.step[i].tempTable[j]) continue;
            ListNode* curr = (ListNode*)stepmanager.step[i].tempTable[j];
            while (curr) {
                ListNode* nextNode = curr->next;
                delete curr;
                curr = nextNode;
            }
            stepmanager.step[i].tempTable[j] = nullptr;
        }
        for (int j = 0; j < stepmanager.step[i].tempEdge.size(); ++j) {
            delete stepmanager.step[i].tempEdge[j];
        }
    }
    HSvalue.clear();
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
    edge.clear();
}

void HTable::remove(){
    code.codeline.clear();
    for (int i = 0; i < stepmanager.step.size(); ++i) {
        for (int j = 0; j < stepmanager.step[i].tempTable.size(); ++j) {
            if (!stepmanager.step[i].tempTable[j]) continue;
            ListNode* curr = (ListNode*)stepmanager.step[i].tempTable[j];
            while (curr) {
                ListNode* nextNode = curr->next;
                delete curr;
                curr = nextNode;
            }
            stepmanager.step[i].tempTable[j] = nullptr;
        }
        for (int j = 0; j < stepmanager.step[i].tempEdge.size(); ++j) {
            delete stepmanager.step[i].tempEdge[j];
        }
    }
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
}

void HTable::copyNode(std::vector<ListNode*> source, std::vector<Node*> &des) {
    if (source.size() == 0) {
        des.clear();
        return;
    }
    des.resize(source.size());
    for (int i = 0; i < source.size(); ++i) {
        if (!source[i]) {
            des[i] = nullptr;
            continue;
        }
        ListNode* head1 = source[i]->next, *head2;
        head2 = new ListNode(source[i]->val);
        head2->position = source[i]->position;
        head2->ID = source[i]->ID;
        des[i] = head2;
        while (head1) {
            head2->next = new ListNode(head1->val);
            head2 = head2->next;
            head2->ID = head1->ID;
            head2->position = head1->position;
            head1 = head1->next;
        }
    }
}

void HTable::copyEdge(std::vector<Edge*> source, std::vector<Edge*> &des, std::vector<Node*> table) {
    if (source.size() == 0) {
        des.clear();
        return;
    }
    des.resize(source.size());
    for (int i = 0; i < source.size(); ++i) {
        des[i] = new Edge(findNodebyID(table,source[i]->endPoint1->ID),findNodebyID(table,source[i]->endPoint2->ID));
        des[i]->ID = source[i]->ID;
    }
}

HTable::ListNode* HTable::findNodebyID(std::vector<Node*> table, int ID) {
    if (table.size() == 0) return nullptr;
    for (int i = 0; i < table.size(); ++i) {
        ListNode* curr = (ListNode*) table[i];
        if (!curr) continue;
        while (curr) {
            if (curr->ID == ID) return curr;
            curr = curr->next;
        }
    }
    return nullptr;
}

Edge* HTable::findEdgebyEndPoint(std::vector<Edge*> list, int endPoint2ID) {
    if (list.size() == 0) return nullptr;
    for (int i = 0; i < list.size(); ++i) {
        if (list[i]->endPoint2->ID == endPoint2ID) return list[i];
    }
    return nullptr;
}

void HTable::prepareTransition() {
    Step& currStep = stepmanager.step[stepmanager.currentStep];
    if (stepmanager.currentStep <= 0) return;
    Step& prevStep = stepmanager.step[stepmanager.currentStep - 1];
    calculatePosition(currStep.tempTable);
    calculatePosition(prevStep.tempTable);
    std::unordered_map<int,ListNode*> currNode;
    std::unordered_map<int,ListNode*> prevNode;
    std::unordered_map<int,Edge*> currEdge;
    std::unordered_map<int,Edge*> prevEdge;
    scanNode(currStep.tempTable,currNode);
    scanNode(prevStep.tempTable,prevNode);
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

void HTable::scanNode(std::vector<Node*> table, std::unordered_map<int,ListNode*> &node) {
    if (table.size() == 0) return;
    for (int i = 0; i < table.size(); ++i) {
        if (!table[i]) continue;
        ListNode* curr = (ListNode*) table[i];
        while (curr) {
            node[curr->ID] = curr;
            curr = curr->next;
        }
    }
}

void HTable::safeRemoveEdge(int nodeID) {
    Edge* edgeToRemove = findEdgebyEndPoint(edge, nodeID);
    if (edgeToRemove) {
        auto it = find(edge.begin(), edge.end(), edgeToRemove);
        if (it != edge.end()) {
            delete *it;
            edge.erase(it);
        }
    }
}

void HTable::printHTable(std::vector<Node*> table) {
    for (int i = 0; i < table.size(); ++i) {
        std::cout<<i<<" : ";
        ListNode* curr = (ListNode*)table[i];
        while (curr) {
            std::cout<<curr->val<<" ";
            curr = curr->next;
        }
        std::cout<<std::endl;
    }
}

void HTable::printHTable(std::vector<ListNode*> table) {
    for (int i = 0; i < table.size(); ++i) {
        std::cout<<i<<" : ";
        ListNode* curr = (ListNode*)table[i];
        while (curr) {
            std::cout<<curr->val<<" ";
            curr = curr->next;
        }
        std::cout<<std::endl;
    }
}

void HTable::initData() {
    if (box.someList.empty()) return;
    if (HSvalue.size() != box.primeNumber) {
        HSvalue.resize(box.primeNumber, nullptr);
    }

    for (int val : box.someList) {
        int index = val % box.primeNumber;
        ListNode* newNode = new ListNode(val);

        if (!HSvalue[index]) {
            HSvalue[index] = newNode;
        } else {
            ListNode* curr = HSvalue[index];
            while (curr->next) {
                curr = curr->next;
            }
            curr->next = newNode;
            Edge* line = new Edge(curr,curr->next);
            edge.push_back(line);
        }
    }
    box.someList.clear();
}

void HTable::insertData() {
    if (box.someList.empty()) return;
    if (box.primeNumber < 0) return;
    code.codeline = {
        "index ← value mod prime                    ",
        "newNode ← new Node(value)                  ",                              
        "if HSvalue[index] is NULL then             ",
        "   { HSvalue[index] ← newNode; return }    ",       
        "curr ← HSvalue[index]                      ",                     
        "while curr.next ≠ NULL do curr ← curr.next ",                    
        "curr.next ← newNode                        "
    };
    Step step;
    for (int i = 0; i < box.someList.size(); ++i) {
        step.highlightedNode = -1;
        step.highlightedLine = 0;
        copyNode(HSvalue,step.tempTable);
        copyEdge(edge,step.tempEdge,step.tempTable);
        stepmanager.step.push_back(step);
        int index = box.someList[i] % box.primeNumber;
        ListNode* newNode = new ListNode(box.someList[i]);
        if (!HSvalue[index]) {
            HSvalue[index] = newNode;
            step.highlightedNode = HSvalue[index]->ID;
            step.highlightedLine = 2;
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
            continue;
        }
        ListNode* curr = HSvalue[index];
        while (curr->next) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 5;
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
            curr = curr->next;
        }
        curr->next = newNode;
        Edge* line = new Edge(curr,curr->next);
        edge.push_back(line);
        step.highlightedNode = curr->next->ID;
        step.highlightedLine = 6;
        copyNode(HSvalue,step.tempTable);
        copyEdge(edge,step.tempEdge,step.tempTable);
        stepmanager.step.push_back(step);
    }
    box.someList.clear();
}

void HTable::deleteData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "index ← value mod prime                                         ",
        "if HSvalue[index] is NULL then return                           ",
        "cur ← HSvalue[index]                                            ",
        "if HSvalue[index].val = value then                              ",
        "{ HSvalue[index] ← HSvalue[index].next; return }                ",
        "while cur.next ≠ NULL and cur.next.val ≠ value do cur ← cur.next",
        "if cur.next ≠ NULL then                                         ",
        "{ del ← cur.next                                                ",
        "  cur.next ← cur.next.next                                      ",
        "  delete del }                                                  "           
    };
    Step step;
   printHTable(HSvalue);
    for (int i = 0; i < box.someList.size(); ++i) {
        step.highlightedNode = -1;
        step.highlightedLine = 0;
        copyNode(HSvalue,step.tempTable);
        stepmanager.step.push_back(step);
        int index = box.someList[i] % box.primeNumber;
        if (!HSvalue[index]) {
            step.highlightedLine = 1;
            step.description.push_back("Index " + std::to_string(index) + " does not has value");
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
            continue;
        }
    
        if (HSvalue[index]->val == box.someList[i]) {
            ListNode* tmp = HSvalue[index];
            if (HSvalue[index]->next) safeRemoveEdge(HSvalue[index]->next->ID);
            HSvalue[index] = HSvalue[index]->next;
            delete tmp;
            step.highlightedLine = 3;
            step.description.push_back("Delete " + to_string(box.someList[i]) + " successfully");
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
            continue;
        }
        
        ListNode* curr = HSvalue[index];
        while (curr->next && curr->next->val != box.someList[i]) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 5;
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
            curr = curr->next;
        }
    
        if (curr->next) {
            ListNode* tmp = curr->next;
            if (curr->next) safeRemoveEdge(curr->next->ID);
            curr->next = curr->next->next;
            delete tmp;
            tmp = nullptr;
            step.highlightedNode = curr->ID;
            step.highlightedLine = 9;
            step.description.push_back("Delete " + to_string(box.someList[i]) + " successfully");
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
        } else {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 9;
            step.description.push_back(to_string(box.someList[i]) + " has not been inserted yet.");
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
        }
    }
    printHTable(HSvalue);
    box.someList.clear();
}

void HTable::searchData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "index ← value mod prime                                   ",
        "if HSvalue[index] is NULL then return false               ",
        "curr ← HSvalue[index]                                     ",
        "while curr ≠ NULL and curr.val ≠ value do curr ← curr.next",
        "return curr ≠ NULL ? true : false                         "
    };
    Step step;
    for (int i = 0; i < box.someList.size(); ++i) {
        int index = box.someList[i] % box.primeNumber;
        step.highlightedNode = -1;
        step.highlightedLine = 0;
        copyNode(HSvalue,step.tempTable);
        copyEdge(edge,step.tempEdge,step.tempTable);
        stepmanager.step.push_back(step);
        if (!HSvalue[index]) {
            step.highlightedLine = 1;
            step.description.push_back("Index " + std::to_string(index) + " does not has value");
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
            continue;
        }
        ListNode* curr = HSvalue[index];
        while (curr && curr->val != box.someList[i]) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 3;
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
            curr = curr->next;
        }
        if (curr) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 4;
            step.description.push_back("Value " + to_string(box.someList[i]) + " is found");
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
        } else {
            step.highlightedNode = -1;
            step.highlightedLine = 4;
            step.description.push_back("Value " + to_string(box.someList[i]) + " is not found");
            copyNode(HSvalue,step.tempTable);
            copyEdge(edge,step.tempEdge,step.tempTable);
            stepmanager.step.push_back(step);
        }
    }
    box.someList.clear();
}

void HTable::updateData(){

}