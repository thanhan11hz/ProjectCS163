#include "SLList.hpp"

void SLList::init() {
    initView();
    root = nullptr;
    stepmanager.currentStep = 0;
    SetMousePosition(780,400);
}

void SLList::draw() {
    drawView();
    if (root) {
        if (!stepmanager.step.empty() && stepmanager.currentStep >= 0) {
            Step currStep = stepmanager.step[stepmanager.currentStep];
            log.infor = currStep.description;
            code.lineHighlighted = currStep.highlightedLine;
            drawView();
            ListNode* curr = (ListNode*)currStep.tempRoot;
            ListNode* prev = nullptr;
            while (curr) {
                if (curr->ID == currStep.highlightedNode) {
                    curr->drawHighlightNode();
                } else {
                    curr->drawNode();
                }
                //std::cout<<stepmanager.currentStep;
                if (prev) {
                    curr->drawEdge(prev);
                }
                prev = curr;
                curr = curr->next;
            }
            code.lineHighlighted = -1; 
        } else {
            ListNode* curr = root;
            ListNode* prev = nullptr;
            while (curr) {
                curr->drawNode();
                if (prev) {
                    curr->drawEdge(prev);
                }
                prev = curr;
                curr = curr->next;
            }
        }
    }
}

void SLList::run() {
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
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(500 / stepmanager.speed)));
    } else {
        stepmanager.isPlaying = false;
        panel.isPlaying = false; 
    }
    
}

void SLList::remove() {
    code.codeline.clear();
    for (int i = 0; i < stepmanager.step.size(); ++i) {
        ListNode* curr = (ListNode*)stepmanager.step[i].tempRoot;
        while (curr) {
            ListNode* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
    }
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
}

void SLList::exit() {
    code.codeline.clear();
    box.someList.clear();
    log.infor.clear();
    ListNode* curr = root;
    while (curr) {
        ListNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    for (int i = 0; i < stepmanager.step.size(); ++i) {
        ListNode* curr = (ListNode*)stepmanager.step[i].tempRoot;
        stepmanager.step[i].tempRoot = nullptr;
        while (curr) {
            ListNode* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
    }
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
    root = nullptr;
}

void SLList::copy(ListNode* source, ListNode* &des) {
    if (!source) {
        des = nullptr;
        return;
    }
    des = new ListNode(source->val);
    des->position = source->position;
    des->ID = source->ID;
    ListNode* currSource = source->next;
    ListNode* currDes = des;
    while (currSource) {
        currDes->next = new ListNode(currSource->val);  
        currDes = currDes->next; 
        currDes->position = currSource->position;
        currDes->ID = currSource->ID;
        currSource = currSource->next;
    }
    currDes->next = nullptr;
}

void SLList::initData() {
    if (box.someList.empty()) return;
    root = new ListNode(box.someList[0]);
    root->position = {480, 400};
    ListNode* curr = root;
    for (int i = 1; i < box.someList.size(); ++i) {
        curr->next = new ListNode(box.someList[i]);
        curr->next->position = {curr->position.x + 80, curr->position.y};
        curr = curr->next;
    }
    box.someList.clear();
}

void SLList::insertData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "if (!root) {          ",
        "   root = new Node;   ",
        "   root -> val = x;   ",
        "   return;        }   ",
        "Node* curr = root;    ",
        "while (curr->next)    ",
        "{ curr = curr->next; }",
        "curr->next = new Node;",
        "curr->next->val = x;  ",
    };
    ListNode* tmp = nullptr;
    for (int i = 0; i < box.someList.size(); ++i) {
        Step step;
        step.highlightedNode = -1;
        step.highlightedLine = 0;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);
        if (!root) {
            root = new ListNode(box.someList[i]);
            root->position = {480,400};
            step.highlightedLine = 1;
            step.highlightedNode = root->ID;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            step.highlightedLine = 2;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            step.highlightedLine = 3;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            continue;
        }

        step.highlightedNode = root->ID;
        step.highlightedLine = 4;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        stepmanager.step.push_back(step);

        ListNode* curr = root;
        while (curr && curr->next) {
            curr = curr->next;
            step.highlightedNode = curr->ID;
            step.highlightedLine = 6;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
        }

        if (curr) {
            curr->next = new ListNode(box.someList[i]);
            curr->next->position = {curr->position.x + 80, curr->position.y};
            step.highlightedNode = curr->next->ID;
            step.highlightedLine = 7;
            step.description.push_back("Insert number " + std::to_string(box.someList[i]) + " successfully");
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            step.highlightedLine = 8;
            stepmanager.step.push_back(step);
        }

    }
    box.someList.clear();
}

void SLList::deleteData() {
    if (box.someList.empty()) return;

    code.codeline = {
        "if (!root) return;                     ",
        "ListNode *curr = root, *prev = nullptr;",
        "while (curr && curr->val != x)         ",
        "{   prev = curr;                       ",
        "    curr = curr->next;   }             ",
        "if (!curr) return;                     ",
        "if (!prev)                             ",
        "    {root = root->next;                ",
        "    delete curr;}                      ",
        "else prev->next = prev->next->next;    "
    };
    ListNode* tmp = nullptr;
    for (int i = 0; i < box.someList.size(); ++i) {
        Step step;
        if (!root) {
            step.highlightedNode = -1;
            step.highlightedLine = 0;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            continue;
        }
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        step.highlightedLine = 1;
        step.highlightedNode = root->ID;
        stepmanager.step.push_back(step);

        ListNode* curr = root;
        ListNode* prev = nullptr;
        while (curr && curr->val != box.someList[i]) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 2;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            step.highlightedNode = curr->ID;
            step.highlightedLine = 3;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            prev = curr;
            step.highlightedLine = 4;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            curr = curr->next;
        }

        if (!curr) {
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            step.highlightedLine = 5;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " isn't in the list");
            stepmanager.step.push_back(step);
            continue;
        }

        if (!prev) {
            root = root->next;
            delete curr;
            root->position = {480,400};
            curr = root;
            while (curr->next) {
                curr->next->position = {curr->position.x + 80, curr->position.y};
                curr = curr->next;
            }
            step.highlightedLine = 6;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            step.highlightedLine = 7;
            step.description.push_back("Delete number " + std::to_string(box.someList[i]) + " successfully");
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
        } else {
            prev->next = curr->next;
            delete curr;
            curr = prev;
            while (curr->next) {
                curr->next->position = {curr->position.x + 80, curr->position.y};
                curr = curr->next;
            }
            step.highlightedLine = 8;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
        }
    }
    box.someList.clear();
}

void SLList::searchData() {
    if (box.someList.empty()) return;

    code.codeline = {
        "if (!root) return;            ",
        "ListNode* curr = root;        ",
        "while (curr && curr->val != x)",
        "    {curr = curr->next;}      ",
        "return (!curr) ? false : true;"
    };
    ListNode *tmp = nullptr;
    for (int i = 0; i < box.someList.size(); ++i) {
        Step step;
        if (!root) {
            step.highlightedNode = -1;
            step.highlightedLine = 0;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            continue;
        }

        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        step.highlightedLine = 1;
        stepmanager.step.push_back(step);

        ListNode* curr = root;
        while (curr && curr->val != box.someList[i]) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 2;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            step.highlightedNode = curr->ID;
            step.highlightedLine = 3;
            copy(root,tmp);
            step.tempRoot = tmp;
            tmp = nullptr;
            stepmanager.step.push_back(step);
            curr = curr->next;
        }
        
        step.highlightedLine = 4;
        copy(root,tmp);
        step.tempRoot = tmp;
        tmp = nullptr;
        if (!curr) {
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " hasn't found");
        } else {
            step.highlightedNode = curr->ID;
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " has found");
        }
        stepmanager.step.push_back(step);
    }
    box.someList.clear();
}