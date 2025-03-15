#include "SLList.hpp"

void SLList::init() {
    initView();
    root = nullptr;
}

void SLList::draw() {
    drawView();
    if (!root) return;
    Step currStep;
    if (!stepmanager.step.empty() && stepmanager.currentStep >= 0) {
        currStep = stepmanager.step[stepmanager.currentStep];
        log.infor = currStep.description;
        code.lineHighlighted = currStep.highlightedLine;
    } else {
        currStep.highlightedNode = nullptr;
    }

    ListNode* curr = root;
    ListNode* prev = nullptr;
    while (curr) {
        if (curr != currStep.highlightedNode) {
            curr->drawNode();
        } else {
            curr->drawHighlightNode();
        }
        if (prev) {
            curr->drawEdge(prev);
        }
        prev = curr;
        curr = curr->next;
    }
    code.lineHighlighted = -1; // Reset highlighted line after drawing
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
                initData();
                func = Function::NONE;
                break;
            case Function::INSERT:
                insertData();
                func = Function::NONE;
                break;
            case Function::DELETE:
                deleteData();
                func = Function::NONE;
                break;
            case Function::SEARCH:
                searchData();
                func = Function::NONE;
                break;
            default:
                break;
        }
    }

    if (panel.isForwardPressed()) {
        stepmanager.isPlaying = false;
        stepmanager.nextStep();
    }
    if (panel.isPausePressed()) {
        stepmanager.isPlaying = false;
    }
    if (panel.isRewindPressed()) {
        stepmanager.isPlaying = false;
        stepmanager.prevStep();
    }
    if (panel.isPlayPressed()) {
        stepmanager.isPlaying = true;
        stepmanager.playAll();
    }
}

void SLList::exit() {
    code.codeline.clear();
    box.someList.clear();
    ListNode* curr = root;
    while (curr) {
        ListNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    root = nullptr;
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

    for (int i = 0; i < box.someList.size(); ++i) {
        Step step;
        step.highlightedNode = nullptr;
        step.highlightedLine = 0;
        stepmanager.step.push_back(step);

        if (!root) {
            root = new ListNode(box.someList[i]);
            root->position = {480,400};
            step.highlightedLine = 1;
            stepmanager.step.push_back(step);
            step.highlightedLine = 2;
            stepmanager.step.push_back(step);
            step.highlightedLine = 3;
            stepmanager.step.push_back(step);
            continue;
        }

        step.highlightedLine = 4;
        stepmanager.step.push_back(step);

        ListNode* curr = root;
        while (curr->next) {
            step.highlightedNode = curr;
            step.highlightedLine = 6;
            stepmanager.step.push_back(step);
            curr = curr->next;
        }

        step.highlightedNode = curr;
        step.highlightedLine = 7;
        stepmanager.step.push_back(step);

        curr->next = new ListNode(box.someList[i]);
        curr->next->position = {curr->position.x + 80, curr->position.y};
    }
    box.someList.clear();
    std::cout<<std::endl;
    ListNode *curr = root;
    while (curr) {
        std::cout<<curr->val<<" ";
        curr = curr->next;
    }
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
            step.highlightedNode = nullptr;
            step.highlightedLine = 0;
            stepmanager.step.push_back(step);
            continue;
        }

        step.highlightedLine = 1;
        stepmanager.step.push_back(step);

        ListNode* curr = root;
        ListNode* prev = nullptr;
        while (curr && curr->val != box.someList[i]) {
            step.highlightedNode = curr;
            step.highlightedLine = 3;
            stepmanager.step.push_back(step);
            prev = curr;
            step.highlightedLine = 4;
            stepmanager.step.push_back(step);
            curr = curr->next;
        }

        if (!curr) {
            step.highlightedLine = 5;
            stepmanager.step.push_back(step);
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " isn't in the list");
            continue;
        }

        if (!prev) {
            step.highlightedLine = 6;
            stepmanager.step.push_back(step);
            root = root->next;
            delete curr;
        } else {
            step.highlightedLine = 8;
            stepmanager.step.push_back(step);
            prev->next = curr->next;
            delete curr;
        }
    }
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
            step.highlightedNode = nullptr;
            step.highlightedLine = 0;
            stepmanager.step.push_back(step);
            continue;
        }

        step.highlightedLine = 1;
        stepmanager.step.push_back(step);

        ListNode* curr = root;
        while (curr && curr->val != box.someList[i]) {
            step.highlightedNode = curr;
            step.highlightedLine = 3;
            stepmanager.step.push_back(step);
            curr = curr->next;
        }

        step.highlightedLine = 4;
        if (!curr) {
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " not found");
        } else {
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " found");
        }
        stepmanager.step.push_back(step);
    }
}