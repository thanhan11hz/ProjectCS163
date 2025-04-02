#include "HTable.hpp"
using namespace std;

void HTable::init() {
    initView();
    stepmanager.currentStep = 0;
}

void HTable::draw() {
    drawView();
    // if (HSvalue.empty()) return;
    // if (!stepmanager.step.empty() && stepmanager.currentStep >= 0) {
    //     Step currStep = stepmanager.step[stepmanager.currentStep];
    //     std::vector<Node*> hash = currStep.tempTable;
    //     log.infor = currStep.description;
    //     code.lineHighlighted = currStep.highlightedLine;
    //     int offsetY = 200;
    //     int cellWidth = 80;
    //     int cellHeight = 60;
    //     int gap = 10;
    
    //     int whiteAreaX = 400;
    //     int whiteAreaWidth = 1440 - whiteAreaX;
    
    //     int totalWidth = box.primeNumber * cellWidth + (box.primeNumber - 1) * gap;
    //     int offsetX = whiteAreaX + (whiteAreaWidth - totalWidth) / 2;
    
    //     for (int i = 0; i < (int)hash.size(); ++i) {
    //         DrawRectangle(offsetX + i * (cellWidth + gap), offsetY, cellWidth, cellHeight, LIGHTGRAY);
    
    //         Vector2 textPos = {
    //             (float)(offsetX + i * (cellWidth + gap) + cellWidth / 2 - 10),
    //             (float)(offsetY + 20)
    //         };
    //         DrawTextEx(GetFontDefault(), std::to_string(i).c_str(), textPos, 20, 2, BLACK);
    
    //         ListNode* curr = (ListNode*)hash[i];
    //         ListNode* prev = nullptr;
    //         int nodeOffsetY = offsetY + cellHeight + 20;
    
    //         while (curr) {
    //             curr->position = {
    //                 (float)(offsetX + i * (cellWidth + gap) + cellWidth / 2),
    //                 (float)(nodeOffsetY)
    //             };
    //             if (curr->ID == currStep.highlightedNode) curr->drawHighlightNode();
    //             else curr->drawNode();
    //             if (prev) prev->drawEdge(curr);
    //             prev = curr;
    //             curr = curr->next;
    //             nodeOffsetY += 70;
    //         }
    //     }
    // } else {
    //     int offsetY = 200;
    //     int cellWidth = 80;
    //     int cellHeight = 60;
    //     int gap = 10;

    //     int whiteAreaX = 400;
    //     int whiteAreaWidth = 1440 - whiteAreaX;

    //     int totalWidth = box.primeNumber * cellWidth + (box.primeNumber - 1) * gap;
    //     int offsetX = whiteAreaX + (whiteAreaWidth - totalWidth) / 2;

    //     for (int i = 0; i < (int)HSvalue.size(); ++i) {
    //         DrawRectangle(offsetX + i * (cellWidth + gap), offsetY, cellWidth, cellHeight, LIGHTGRAY);

    //         Vector2 textPos = {
    //             (float)(offsetX + i * (cellWidth + gap) + cellWidth / 2 - 10),
    //             (float)(offsetY + 20)
    //         };
    //         DrawTextEx(GetFontDefault(), std::to_string(i).c_str(), textPos, 20, 2, BLACK);

    //         ListNode* curr = HSvalue[i];
    //         ListNode* prev = nullptr;
    //         int nodeOffsetY = offsetY + cellHeight + 20;

    //         while (curr) {
    //             curr->position = {
    //                 (float)(offsetX + i * (cellWidth + gap) + cellWidth / 2),
    //                 (float)(nodeOffsetY)
    //             };
    //             curr->drawNode();
    //             if (prev) prev->drawEdge(curr);
    //             prev = curr;
    //             curr = curr->next;
    //             nodeOffsetY += 70;
    //         }
    //     }
    // }
}

void HTable::run() {
    eventView();

    if (option.isInitialize()) {
        box.enteredValues = false;
        box.enteredPrime = false;
        func = Function::INIT;
    }
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
        box.enteredPrime = false;
        func = Function::SEARCH;
    }

    if (!box.isOpen && func != Function::NONE) {
        switch (func) {
            case Function::INIT:
                remove();
                initData();
                func = Function::NONE;
                break;
            case Function::INSERT:
                //remove();
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

void HTable::exit() {
    code.codeline.clear();
    box.someList.clear();
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
    }
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
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
    }
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
}

void HTable::copy(std::vector<ListNode*> source, std::vector<Node*> &des) {
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
        }
    }
    box.someList.clear();
}

void HTable::insertData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "int index = value % prime;            ",
        "Node* newNode = new Node(value);      ",                              
        "if (!HSvalue[index])                  ",
        "   {HSvalue[index] = newNode; return;}",       
        "Node *curr = HSvalue[index];          ",                     
        "while (curr->next) curr = curr->next; ",                    
        "curr->next = newNode;                 "
    };
    Step step;
    for (int i = 0; i < box.someList.size(); ++i) {
        step.highlightedNode = -1;
        step.highlightedLine = 0;
        copy(HSvalue,step.tempTable);
        stepmanager.step.push_back(step);
        int index = box.someList[i] % box.primeNumber;
        ListNode* newNode = new ListNode(box.someList[i]);
        if (!HSvalue[index]) {
            HSvalue[index] = newNode;
            step.highlightedNode = HSvalue[index]->ID;
            step.highlightedLine = 2;
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
            continue;
        }
        ListNode* curr = HSvalue[index];
        while (curr->next) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 5;
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
            curr = curr->next;
        }
        curr->next = newNode;
        step.highlightedNode = curr->next->ID;
        step.highlightedLine = 6;
        copy(HSvalue,step.tempTable);
        stepmanager.step.push_back(step);
    }
    printHTable(HSvalue);
    box.someList.clear();
}

void HTable::deleteData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "int index = value % prime;                                              ",
        "if (!HSvalue[index]) return;                                            ",
        "Node *cur = HSvalue[index];                                             ",
        "if (HSvalue[index]->val == value)                                       ",
        "{ HSValue[index] = HSvalue[index]->next; return;                        ",
        "while (cur->next && cur->next->val != value) cur = cur->next;           ",                            
        "if (cur->next)                                                          ",
        "   {Node *del = cur->next;                                              ",
        "   cur->next = cur->next->next;                                         ",
        "   delete del;}                                                         "
    };
    Step step;
   printHTable(HSvalue);
    for (int i = 0; i < box.someList.size(); ++i) {
        step.highlightedNode = -1;
        step.highlightedLine = 0;
        copy(HSvalue,step.tempTable);
        stepmanager.step.push_back(step);
        int index = box.someList[i] % box.primeNumber;
        if (!HSvalue[index]) {
            step.highlightedLine = 1;
            step.description.push_back("Index " + std::to_string(index) + " does not has value");
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
            continue;
        }
    
        if (HSvalue[index]->val == box.someList[i]) {
            ListNode* tmp = HSvalue[index];
            HSvalue[index] = HSvalue[index]->next; 
            delete tmp;
            step.highlightedLine = 3;
            step.description.push_back("Index " + std::to_string(index) + " does not has value");
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
            continue;
        }
        
        ListNode* curr = HSvalue[index];
        while (curr->next && curr->next->val != box.someList[i]) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 5;
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
            curr = curr->next;
        }
    
        if (curr->next) {
            ListNode* tmp = curr->next;
            curr->next = curr->next->next;
            delete tmp;
            tmp = nullptr;
            step.highlightedNode = curr->ID;
            step.highlightedLine = 9;
            step.description.push_back("Delete " + to_string(box.someList[i]) + " successfully");
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
        } else {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 9;
            step.description.push_back(to_string(box.someList[i]) + " has not been inserted yet.");
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
        }
    }
    printHTable(HSvalue);
    box.someList.clear();
}

void HTable::searchData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "int index = value % prime;                           ",
        "if (!HSvalue[index]) return false;                   ",
        "Node *curr = HSvalue[index];                         ",
        "while (curr && curr->val != value) curr = curr->next;",
        "return curr ? true : false;                          "
    };
    Step step;
    for (int i = 0; i < box.someList.size(); ++i) {
        int index = box.someList[i] % box.primeNumber;
        step.highlightedNode = -1;
        step.highlightedLine = 0;
        copy(HSvalue,step.tempTable);
        stepmanager.step.push_back(step);
        if (!HSvalue[index]) {
            step.highlightedLine = 1;
            step.description.push_back("Index " + std::to_string(index) + " does not has value");
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
            continue;
        }
        ListNode* curr = HSvalue[index];
        while (curr && curr->val != box.someList[i]) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 3;
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
            curr = curr->next;
        }
        if (curr) {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 4;
            step.description.push_back("Value " + to_string(box.someList[i]) + " is found");
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
        } else {
            step.highlightedNode = -1;
            step.highlightedLine = 4;
            step.description.push_back("Value " + to_string(box.someList[i]) + " is not found");
            copy(HSvalue,step.tempTable);
            stepmanager.step.push_back(step);
        }
    }
    box.someList.clear();
}