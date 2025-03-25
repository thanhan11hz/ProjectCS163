#include "HTable.hpp"
using namespace std;

void HTable::init() {
    initView();
    stepmanager.currentStep = 0;
}

void HTable::draw() {
    drawView(); // Giao diện chung

    if (HSvalue.empty()) return;

    int offsetY = 200;
    int cellWidth = 80;
    int cellHeight = 60;
    int gap = 10;

    int whiteAreaX = 400;
    int whiteAreaWidth = 1440 - whiteAreaX;

    int totalWidth = box.primeNumber * cellWidth + (box.primeNumber - 1) * gap;
    int offsetX = whiteAreaX + (whiteAreaWidth - totalWidth) / 2;

    for (int i = 0; i < (int)HSvalue.size(); ++i) {
        DrawRectangle(offsetX + i * (cellWidth + gap), offsetY, cellWidth, cellHeight, LIGHTGRAY);

        Vector2 textPos = {
            (float)(offsetX + i * (cellWidth + gap) + cellWidth / 2 - 10),
            (float)(offsetY + 20)
        };
        DrawTextEx(GetFontDefault(), std::to_string(i).c_str(), textPos, 20, 2, BLACK);

        ListNode* curr = HSvalue[i];
        ListNode* prev = nullptr;
        int nodeOffsetY = offsetY + cellHeight + 20;

        while (curr) {
            curr->position = {
                (float)(offsetX + i * (cellWidth + gap) + cellWidth / 2),
                (float)(nodeOffsetY)
            };
            curr->drawNode();
            if (prev) prev->drawEdge(curr);
            prev = curr;
            curr = curr->next;
            nodeOffsetY += 70;
        }
    }
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
        std::cout<<1;
        stepmanager.nextStep();
        draw();
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(800 / stepmanager.speed)));
    } else {
        stepmanager.isPlaying = false;
        panel.isPlaying = false; 
    }
}

void HTable::exit() {
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
}

void HTable::remove(){
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
}


void HTable::initData() {
    if (box.someList.empty()) return;

    // Đảm bảo HSvalue có kích thước đúng với primeNumber
    if (HSvalue.size() != box.primeNumber) {
        HSvalue.resize(box.primeNumber, nullptr); // Điều chỉnh kích thước của HSvalue
    }

    // Chèn dữ liệu vào HSvalue
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

    // Xóa danh sách đầu vào sau khi chèn xong
    box.someList.clear();
}

void HTable::insertData() {
    // Kiểm tra nếu danh sách đầu vào rỗng
    if (box.someList.empty()) {
        std::cout << "Debug: No values to insert. box.someList is empty." << std::endl;
        return;
    }

    // Kiểm tra nếu primeNumber chưa được nhập
    if (box.primeNumber == 0) {
        std::cerr << "Debug: Prime number is not set. Please initialize with a prime number first!" << std::endl;
        log.infor.push_back("Please enter a prime number first!");
        return;
    }

    // Đảm bảo HSvalue có kích thước đúng với primeNumber
    if (HSvalue.size() != box.primeNumber) {
        std::cout << "Debug: Resizing HSvalue to size " << box.primeNumber << std::endl;
        HSvalue.resize(box.primeNumber, nullptr);
    }

    // Duyệt qua từng giá trị cần chèn
    for (int val : box.someList) {
        int index = val % box.primeNumber; // Tính chỉ mục đúng

        // Tạo node mới
        ListNode* newNode = new ListNode(val);
        if (!newNode) {
            std::cerr << "Debug: Memory allocation failed for value " << val << std::endl;
            continue; // Bỏ qua nếu không thể cấp phát bộ nhớ
        }

        // Chèn vào bucket tương ứng
        if (!HSvalue[index]) {
            // Nếu bucket rỗng, chèn node mới vào đầu
            HSvalue[index] = newNode;
        } else {
            // Nếu bucket không rỗng, duyệt đến cuối danh sách và chèn
            ListNode* curr = HSvalue[index];
            while (curr->next) {
                curr = curr->next;
            }
            curr->next = newNode;
        }

        // Debug: In ra thông tin chèn
        std::cout << "Debug: Inserted " << val << " at index " << index << std::endl;
    }

    // Xóa danh sách đầu vào sau khi chèn xong
    box.someList.clear();
}

void HTable::deleteData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "int index = value % prime;                               ",
        "if (HSvalue[index] == nullptr) continue;                 ",                                            
        "else{   Node *cur = HSvalue[index];                      ",
        "    while (cur && cur->next && cur->next->val != value){ ",
        "        cur = cur->next;}                                ",
        "    if (cur && cur->next){                               ",
        "        Node *del = cur->next;                           ",
        "        cur->next = cur->next->next;                     ",
        "        delete del;}                                     "
    };

    for (int i = 0; i < box.someList.size(); i++){
        Step step;

        int index = box.someList[i] % box.primeNumber;
        step.highlightedLine = 0;
        step.description.push_back("Calculated index: " + std::to_string(index));

        {   vector<ListNode *> tmp1;
            copy(HSvalue, tmp1);
            for (auto ln : tmp1){
                step.tempTable.push_back(static_cast<Node *>(ln));
            }
        }
        stepmanager.step.push_back(step);


        if (HSvalue[index] == nullptr) {

            step.highlightedLine = 1;
            step.description.push_back("Index " + std::to_string(index) + " does not has value");
            vector<ListNode *> tmp2;
            copy(HSvalue, tmp2);
            for (auto ln :tmp2){
                step.tempTable.push_back(static_cast<Node *>(ln));
            }
            stepmanager.step.push_back(step);

            continue;
        }
        else {
            ListNode *cur = HSvalue[index];

            step.highlightedLine = 2;
            vector<ListNode *> tmp3;
            copy(HSvalue, tmp3);
            for (auto ln : tmp3){
                step.tempTable.push_back(static_cast<Node *>(ln));
            }
            stepmanager.step.push_back(step);


            while (cur && cur->next && cur->next->val != box.someList[i]){
                step.highlightedLine = 3;
                vector<ListNode *> tmp4;
                copy(HSvalue, tmp4);
                for (auto ln : tmp4){
                    step.tempTable.push_back(static_cast<Node *>(ln));
                }
                stepmanager.step.push_back(step);

                cur = cur->next;
                step.highlightedLine = 4;
                vector<ListNode *> tmp5;
                copy(HSvalue, tmp5);
                for (auto ln : tmp5){
                    step.tempTable.push_back(static_cast<Node *>(ln));
                }
                stepmanager.step.push_back(step);
            }
            if (cur != nullptr && cur->next != nullptr){

                {   step.highlightedLine = 5;
                    vector<ListNode *> tmp6;
                    copy(HSvalue, tmp6);
                    for (auto ln : tmp6){
                        step.tempTable.push_back(static_cast<Node *>(ln));
                    }
                    stepmanager.step.push_back(step);
                }   

                ListNode *del = cur->next;

                {    step.highlightedLine = 6;
                    vector<ListNode *> tmp7;
                    copy(HSvalue, tmp7);
                    for (auto ln : tmp7){
                        step.tempTable.push_back(static_cast<Node *>(ln));
                    }
                    stepmanager.step.push_back(step);
                }

                cur->next = cur->next->next;
                
                {   step.highlightedLine = 7;
                    vector<ListNode *> tmp8;
                    copy(HSvalue, tmp8);
                    for (auto ln : tmp8){
                        step.tempTable.push_back(static_cast<Node *>(ln));
                    }
                    stepmanager.step.push_back(step);
                }

                ListNode *del2 = cur->next;
                {   step.highlightedLine = 8;
                    vector<ListNode *> tmp9;
                    copy(HSvalue, tmp9);
                    for (auto ln : tmp9){
                        step.tempTable.push_back(static_cast<Node *>(ln));
                    }
                    stepmanager.step.push_back(step);
                }

                delete del2;
                {   step.highlightedLine = 9;
                    step.description.push_back("Delete " + to_string(box.someList[i]) + " successflly");
                    vector<ListNode *> tmp10;
                    copy(HSvalue, tmp10);
                    for (auto ln : tmp10){
                        step.tempTable.push_back(static_cast<Node *>(ln));
                    }
                    stepmanager.step.push_back(step);
                }
            }
            else if (cur == nullptr){
                step.highlightedLine = -1;
                step.description.push_back(to_string(box.someList[i]) + " has not been inserted yet.");
                vector<ListNode *> tmp11;
                copy(HSvalue, tmp11);
                for (auto ln : tmp11){
                    step.tempTable.push_back(static_cast<Node *>(ln));
                }
                stepmanager.step.push_back(step);
            }
        }
    }
    box.someList.clear();
}

void HTable::searchData() {
    if (box.someList.empty()) return;
    code.codeline = {
        "int index = value % prime;                                   ",
        "if (HSvalue[index] == nullptr) continue;                     ",
        "else { ListNode *cur = HSvalue[index];                       ",
        "while (cur && cur->val != box.someList[i]) cur = cur->next;  ",
        "return cur ? true : false                                    "
    };

    for (int i = 0; i < box.someList.size(); i++){
        Step step;

        int index = box.someList[i] % box.primeNumber;

        {   step.highlightedLine = 0;
            step.description.push_back("Calculate index: " + to_string(index));
            vector<ListNode *> tmp0;
            copy(HSvalue, tmp0);
            for (auto ln : tmp0){
                step.tempTable.push_back(static_cast<Node *>(ln));
            }
            stepmanager.step.push_back(step);
        }

        if (HSvalue[index] == nullptr) {

            {   step.highlightedLine = 1;
                step.description.push_back("Index " + to_string(index) + "does not keep value");
                vector<ListNode *> tmp1;
                copy(HSvalue, tmp1);
                for (auto ln : tmp1){
                    step.tempTable.push_back(static_cast<Node *>(ln));
                }
                stepmanager.step.push_back(step);
            }
            continue;
        }
        else {
            ListNode *cur = HSvalue[index];

            {   step.highlightedLine = 2;
                vector<ListNode *> tmp2;
                copy(HSvalue, tmp2);
                for (auto ln : tmp2){
                    step.tempTable.push_back(static_cast<Node *>(ln));
                }
                stepmanager.step.push_back(step);
            }

            while (cur != nullptr && cur->val != box.someList[i]){
                cur = cur->next;

                {   step.highlightedLine = 3;
                    vector<ListNode *> tmp3;
                    copy(HSvalue, tmp3);
                    for (auto ln : tmp3){
                        step.tempTable.push_back(static_cast<Node *>(ln));
                    }
                    stepmanager.step.push_back(step);
                }
            }
            bool found = false;
            if (cur != nullptr){ 
                bool found = true;  
            }

            {   step.highlightedLine = 4;
                if (found){
                    step.description.push_back("Value " + to_string(box.someList[i]) + " is found");
                }
                else {
                    step.description.push_back("Value " + to_string(box.someList[i]) + " is not found");
                }
                vector<ListNode *> tmp4;
                copy(HSvalue, tmp4);
                for (auto ln : tmp4){
                    step.tempTable.push_back(static_cast<Node *>(ln));
                }
                stepmanager.step.push_back(step);
            }
            
        }
    }
    box.someList.clear();
}

void HTable::copy(const std::vector<ListNode*> source, std::vector<ListNode*>& des) {
    des.clear();
    des.resize(source.size(), nullptr); // Đảm bảo kích thước đúng

    std::cout << "Debug: Copying HSvalue to tempTable. Source size: " << source.size() << std::endl;

    for (int i = 0; i < (int)source.size(); i++) {
        if (source[i]) {
            std::cout << "Debug: Copying linked list at index " << i << std::endl;
            std::cout << "Debug: Source node value: " << source[i]->val << ", ID: " << source[i]->ID << std::endl;
            copyList(source[i], des[i]); // Sao chép danh sách liên kết tại bucket i
        } else {
            std::cout << "Debug: Source at index " << i << " is nullptr." << std::endl;
        }
    }
}

void HTable::copyList(ListNode* source, ListNode*& des) {
    if (!source) {
        std::cout << "Debug: Source is nullptr. Setting des to nullptr." << std::endl;
        des = nullptr;
        return;
    }

    // Tạo node mới và sao chép giá trị
    des = new ListNode(source->val); // Tạo bản sao độc lập
    des->position = source->position;
    des->ID = source->ID;

    std::cout << "Debug: Copied node with value " << des->val << " and ID " << des->ID << std::endl;

    // Sao chép các node tiếp theo
    ListNode* currSource = source->next;
    ListNode* currDes = des;
    while (currSource) {
        std::cout << "Debug: Copying next node with value " << currSource->val << ", ID: " << currSource->ID << std::endl;

        ListNode* newNode = new ListNode(currSource->val); // Tạo bản sao độc lập
        newNode->position = currSource->position;
        newNode->ID = currSource->ID;

        currDes->next = newNode;
        currDes = currDes->next;
        currSource = currSource->next;

        std::cout << "Debug: Copied node with value " << newNode->val << " and ID " << newNode->ID << std::endl;
    }
    currDes->next = nullptr; // Đảm bảo kết thúc danh sách
}