#include "SLList.hpp"

void SLList::init() {
    initView();
    root = nullptr;
}

void SLList::draw() {
    drawView();
    if (!root || stepmanager.step.empty()) return;
    Step currStep = stepmanager.step[stepmanager.currentStep];
    log.infor = currStep.description;
    code.lineHighlighted = currStep.highlightedLine;
    if (!root) return;
    ListNode* curr = root, *prev = nullptr;
    while (curr) {
        if (curr != currStep.highlightedNode)  curr->drawNode();
        else curr->drawHighlightNode();
        if (prev) curr->drawEdge(prev);
        prev = curr;
        curr = curr->next;
    }
    code.lineHighlighted = -1;
}

void SLList::run() {
    eventView();
    if (option.isInitialize()) {
        exit();
        initData();
    } else if (option.isAdd()) {
        code.codeline.clear();
        insertData();
    } else if (option.isDelete()) {
        code.codeline.clear();
        deleteData();
    } else if (option.isSearch()) {
        code.codeline.clear();
        searchData();
    }
    if (panel.isForwardPressed()) {
        stepmanager.nextStep();
    }
    if (panel.isPausePressed()) {
        stepmanager.isPlaying = false;
    }
    if (panel.isRewindPressed()) {
        stepmanager.prevStep();
    }
}

void SLList::exit() {
    code.codeline.clear();
    someList.clear();
    ListNode *curr = root;
    while (curr) {
        ListNode* nextNode = curr->next;
        curr = curr->next;
        delete curr;
    }
    root = nullptr;
}

void SLList::initData() {
    //Gia tri bang 0
    if (someList.size() == 0) return;
    std::cout<<1;
    root = new ListNode(someList[0]);
    root->position = {480,640};
    ListNode* curr = root;
    for (int i = 1; i < someList.size(); ++i) {
        curr->next = new ListNode(someList[i]);
        curr->next->position = {curr->position.x + 80, curr->position.y};
        curr = curr->next;
    }
}
void SLList::insertData() {
    if (someList.size() == 0) return;
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
    Step step;
    for (int i = 0; i < someList.size(); ++i) {
        step.highlightedNode = nullptr;
        step.highlightedLine = 0;
        stepmanager.step.push_back(step);
        if (!root) {
            root = new ListNode(someList[i]);
            step.highlightedLine = 1;
            stepmanager.step.push_back(step);
            step.highlightedLine = 2;
            stepmanager.step.push_back(step);
            step.highlightedLine = 3;
            stepmanager.step.push_back(step);
            break;
        }
        step.highlightedLine = 4;
        stepmanager.step.push_back(step);
        step.highlightedLine = 5;
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
        step.highlightedLine = 8;
        stepmanager.step.push_back(step);
        curr->next = new ListNode(someList[i]);
    }
}
void SLList::deleteData() {
    if (someList.size() == 0) return;
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
    Step step;
    for (int i = 0; i < someList.size(); ++i) {
        if (!root) {
            step.highlightedNode = nullptr;
            step.highlightedLine = 0;
            stepmanager.step.push_back(step);
            break;
        }
        step.highlightedLine = 1;
        stepmanager.step.push_back(step);
        step.highlightedLine = 2;
        stepmanager.step.push_back(step);
        ListNode *curr = root, *prev = nullptr;
        while (curr && curr->val != someList[i]) {
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
            step.description.push_back("Number " + std::to_string(someList[i]) + " isn't belong to the Linked List");
            break;
        }
        if (!prev) {
            step.highlightedLine = 6;
            stepmanager.step.push_back(step);
            step.highlightedLine = 7;
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
    if (someList.size() == 0) return;
    code.codeline = {
        "if (!root) return;            ",
        "ListNode* curr = root;        ",
        "while (curr && curr->val != x)",
        "    {curr = curr->next;}      ",
        "return (!curr) ? false : true;"
    };
    Step step;
    for (int i = 0; i < someList.size(); ++i) {
        if (!root) {
            step.highlightedNode = nullptr;
            step.highlightedLine = 0;
            stepmanager.step.push_back(step);
            return;
        }
        step.highlightedLine = 1;
        stepmanager.step.push_back(step);
        ListNode* curr = root;
        step.highlightedLine = 2;
        stepmanager.step.push_back(step);
        while (curr && curr->val!=someList[i]) {
            step.highlightedNode = curr;
            step.highlightedLine = 3;
            stepmanager.step.push_back(step);
            curr = curr->next;
        }
        step.highlightedLine = 4;
        if (!curr) step.description.push_back("Number " + std::to_string(someList[i]) + " isn't belong to the Linked List");
        else step.description.push_back("Number " + std::to_string(someList[i]) + " has found in the Linked List");
        stepmanager.step.push_back(step);
    }
}