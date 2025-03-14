#include "SLList.hpp"

void SLList::init() {
    initView();
}

void SLList::draw() {
    drawView();
    if (!root) return;
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
}

void SLList::exit() {
    code.codeline.clear();
    box.value.clear();
    ListNode *curr = root, *prev = root;
    while (!prev) {
        delete prev;
        prev = curr;
        curr = curr->next;
    }
}

void SLList::initData() {
    if (box.value.size() == 0) return;
    root = new ListNode(box.value[0]);
    root->position = {480,640};
    ListNode* curr = root -> next;
    for (int i = 1; i < box.value.size(); ++i) {
        curr->next = new ListNode(box.value[i]);
        curr->next->position = {curr->position.x + 80, curr->position.y};
        curr = curr->next;
    }
}
void SLList::insertData() {
    if (box.value.size() == 0) return;
    code.codeline = {
        "if (!root) }          ",
        "   root = new Node;   ",
        "   root -> val = x;   ",
        "   return;        }   ",
        "Node* curr = root;    "
        "while (!curr->val)    ",
        "{ curr = curr->next; }",
        "curr->next = new Node;",
        "curr->next->val = x;  ",
    };
    Step step;
    if (!root) {
        root = new ListNode(box.value[0]);
    }
}
void SLList::deleteData() {
    
}
void SLList::searchData() {

}