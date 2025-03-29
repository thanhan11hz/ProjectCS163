#include "SLList.hpp"

void SLList::init() {
    initView();
    root = nullptr;
    stepmanager.currentStep = 0;
    SetMousePosition(780,400);
}

void SLList::draw() {
    drawView();
    if (!root) return;
    if (!stepmanager.step.empty() && stepmanager.currentStep >= 0) {
        Step currStep = stepmanager.step[stepmanager.currentStep];
        log.infor = currStep.description;
        code.lineHighlighted = currStep.highlightedLine;
        drawView();
        calculatePosition((ListNode*)currStep.tempRoot);
        ListNode* curr = (ListNode*)currStep.tempRoot;
        ListNode* prev = nullptr;
        while (curr) {
            if (curr->ID == currStep.highlightedNode) {
                curr->drawHighlightNode();
            } else {
                curr->drawNode();
            }
            if (prev) {
                curr->drawEdge(prev);
            }
            prev = curr;
            curr = curr->next;
        }
        code.lineHighlighted = -1; 
    } else {
        calculatePosition(root);
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

void SLList::calculatePosition(ListNode* head) {
    int count = 0;
    ListNode* curr = head;
    while (curr) {
        count++; 
        curr = curr->next;
    }
    if (count <= 11) {
        float distx = (1020 - (90 * count - 30)) / 2.0f;
        ListNode* curr = head;
        for (int i = 0; i < count; ++i) {
            curr->position = {450 + distx + 90 * i, 400};
            curr = curr->next;
        }
        return;
    }
    curr = head;
    float disty = (640 - (90 * ceil(count / 11) - 30)) / 2.0f;
    for (int i = 0; i < count; ++i) {
        int currRow = i / 11;
        int currColumn;
        if (currRow % 2 == 0) currColumn = i % 11;
        else currColumn = 10 - i % 11;
        curr->position = {480.0f + 90 * currColumn, 110 + disty + 90 * currRow};
        curr = curr->next;
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

void SLList::copy(ListNode* source, Node* &des) {
    if (!source) {
        des = nullptr;
        return;
    }
    ListNode* alias = nullptr;
    alias = new ListNode(source->val);
    alias->ID = source->ID;
    ListNode* currSource = source->next;
    ListNode* currDes = alias;
    while (currSource) {
        currDes->next = new ListNode(currSource->val);  
        currDes = currDes->next; 
        currDes->ID = currSource->ID;
        currSource = currSource->next;
    }
    currDes->next = nullptr;
    des = (Node*) alias;
}

void SLList::initData() {
    if (box.someList.empty()) return;
    root = new ListNode(box.someList[0]);
    ListNode* curr = root;
    for (int i = 1; i < box.someList.size(); ++i) {
        curr->next = new ListNode(box.someList[i]);
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
    for (int i = 0; i < box.someList.size(); ++i) {
        Step step;
        step.highlightedNode = -1;
        step.highlightedLine = 0;
        copy(root,step.tempRoot);
        stepmanager.step.push_back(step);
        if (!root) {
            root = new ListNode(box.someList[i]);
            step.highlightedLine = 1;
            step.highlightedNode = root->ID;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            step.highlightedLine = 2;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            step.highlightedLine = 3;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            continue;
        }

        step.highlightedNode = root->ID;
        step.highlightedLine = 4;
        copy(root,step.tempRoot);
        stepmanager.step.push_back(step);

        ListNode* curr = root;
        while (curr && curr->next) {
            curr = curr->next;
            step.highlightedNode = curr->ID;
            step.highlightedLine = 6;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
        }

        if (curr) {
            curr->next = new ListNode(box.someList[i]);
            step.highlightedNode = curr->next->ID;
            step.highlightedLine = 7;
            step.description.push_back("Insert number " + std::to_string(box.someList[i]) + " successfully");
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            copy(root,step.tempRoot);
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
    for (int i = 0; i < box.someList.size(); ++i) {
        Step step;
        if (!root) {
            step.highlightedNode = -1;
            step.highlightedLine = 0;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            continue;
        }
        copy(root,step.tempRoot);
        step.highlightedLine = 1;
        step.highlightedNode = root->ID;
        stepmanager.step.push_back(step);

        ListNode* curr = root;
        ListNode* prev = nullptr;
        while (curr && curr->val != box.someList[i]) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 2;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            step.highlightedNode = curr->ID;
            step.highlightedLine = 3;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            prev = curr;
            step.highlightedLine = 4;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            curr = curr->next;
        }

        if (!curr) {
            copy(root,step.tempRoot);
            step.highlightedLine = 5;
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " isn't in the list");
            stepmanager.step.push_back(step);
            continue;
        }

        if (!prev) {
            root = root->next;
            delete curr;
            curr = root;
            step.highlightedLine = 7;
            step.highlightedNode = curr->ID;
            copy(root,step.tempRoot);
            step.description.push_back("Delete number " + std::to_string(box.someList[i]) + " successfully");
            stepmanager.step.push_back(step);
        } else {
            prev->next = curr->next;
            delete curr;
            curr = prev;
            step.highlightedLine = 8;
            step.highlightedNode = prev->ID;
            copy(root,step.tempRoot);
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
    for (int i = 0; i < box.someList.size(); ++i) {
        Step step;
        if (!root) {
            step.highlightedNode = -1;
            step.highlightedLine = 0;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            continue;
        }

        copy(root,step.tempRoot);
        step.highlightedLine = 1;
        stepmanager.step.push_back(step);

        ListNode* curr = root;
        while (curr && curr->val != box.someList[i]) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 2;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            step.highlightedNode = curr->ID;
            step.highlightedLine = 3;
            copy(root,step.tempRoot);
            stepmanager.step.push_back(step);
            curr = curr->next;
        }
        
        step.highlightedLine = 4;
        copy(root,step.tempRoot);
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