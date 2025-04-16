#include "SLList.hpp"

void SLList::init()
{
    initView();
    root = nullptr;
    stepmanager.currentStep = 0;
    SetMousePosition(780, 400);
    box.someList.clear();
}

void SLList::draw()
{
    drawView();
    if (!root)
        return;
    if (!stepmanager.step.empty() && stepmanager.currentStep >= 0)
    {
        Step currStep = stepmanager.step[stepmanager.currentStep];
        log.infor = currStep.description;
        code.lineHighlighted = currStep.highlightedLine;
        BeginScissorMode(400, 80, 1040, 640);
        BeginMode2D(camera);
        if (stepmanager.isTransitioning)
        {
            Animation currAnimation = currStep.animQueue.animation.front();
            Step &prevStep = stepmanager.step[stepmanager.currentStep - 1];
            if (currAnimation.type == AnimateType::DELETION)
            {
                resetColorNode((ListNode *)prevStep.tempRoot);
                resetColorEdge(prevStep.tempEdge);
                drawEdge(prevStep.tempEdge);
                drawNode((ListNode *)prevStep.tempRoot, currStep.highlightedNode);
            }
            else if (currAnimation.type == AnimateType::MOVEMENT)
            {
                resetColorNode((ListNode *)prevStep.tempRoot);
                resetColorEdge(prevStep.tempEdge);
                drawEdge(prevStep.tempEdge);
                drawNode((ListNode *)prevStep.tempRoot, currStep.highlightedNode);
            }
            else if (currAnimation.type == AnimateType::INSERTION)
            {
                resetColorNode((ListNode *)currStep.tempRoot);
                resetColorEdge(currStep.tempEdge);
                drawEdge(currStep.tempEdge);
                drawNode((ListNode *)currStep.tempRoot, currStep.highlightedNode);
            }
        }
        else
        {
            calculatePosition((ListNode *)currStep.tempRoot);
            resetAlphaEdge(currStep.tempEdge);
            resetAlphaNode((ListNode *)currStep.tempRoot);
            resetColorNode((ListNode *)currStep.tempRoot);
            resetColorEdge(currStep.tempEdge);
            drawEdge(currStep.tempEdge);
            drawNode((ListNode *)currStep.tempRoot, currStep.highlightedNode);
            code.lineHighlighted = -1;
        }
        EndMode2D();
        EndScissorMode();
    }
    else
    {
        BeginScissorMode(400, 80, 1040, 640);
        BeginMode2D(camera);
        calculatePosition(root);
        resetColorNode(root);
        resetColorEdge(edge);
        drawEdge(edge);
        drawNode(root, -1);
        EndMode2D();
        EndScissorMode();
    }
}

void SLList::calculatePosition(ListNode *head)
{
    int count = 0;
    ListNode *curr = head;
    while (curr)
    {
        count++;
        curr = curr->next;
    }
    if (count <= 11)
    {
        float distx = (1020 - (90 * count - 30)) / 2.0f;
        ListNode *curr = head;
        for (int i = 0; i < count; ++i)
        {
            curr->position = {450 + distx + 90 * i, 400};
            curr = curr->next;
        }
        return;
    }
    curr = head;
    float disty = (640 - (90 * ceil(count / 11) - 30)) / 2.0f;
    for (int i = 0; i < count; ++i)
    {
        int currRow = i / 11;
        int currColumn;
        if (currRow % 2 == 0)
            currColumn = i % 11;
        else
            currColumn = 10 - i % 11;
        curr->position = {480.0f + 90 * currColumn, 110 + disty + 90 * currRow};
        curr = curr->next;
    }
}

void SLList::drawNode(ListNode *head, int highlight)
{
    if (!head)
        return;
    ListNode *curr = head;
    while (curr)
    {
        if (curr->ID == highlight)
            curr->drawHighlight();
        else
            curr->draw();
        curr = curr->next;
    }
}

void SLList::drawEdge(std::vector<Edge *> edge)
{
    for (int i = 0; i < edge.size(); ++i)
    {
        edge[i]->draw();
    }
}

void SLList::resetAlphaNode(ListNode *head)
{
    if (!head)
        return;
    ListNode *curr = head;
    while (curr)
    {
        curr->alpha = 1.0f;
        curr = curr->next;
    }
}

void SLList::resetAlphaEdge(std::vector<Edge *> edge)
{
    for (int i = 0; i < edge.size(); ++i)
    {
        edge[i]->alpha = 1.0f;
    }
}

void SLList::resetColorNode(ListNode *head)
{
    if (!head)
        return;
    ListNode *curr = head;
    while (curr)
    {
        if (theme == colorType::HOT)
        {
            curr->currentColor = myColor1[0];
            curr->targetColor = myColor1[2];
        }
        else
        {
            curr->currentColor = myColor2[0];
            curr->targetColor = myColor2[2];
        }
        curr = curr->next;
    }
}

void SLList::resetColorEdge(std::vector<Edge *> edge)
{
    for (int i = 0; i < edge.size(); ++i)
    {
        if (theme == colorType::HOT)
        {
            edge[i]->currentColor = myColor1[1];
            edge[i]->targetColor = myColor1[2];
        }
        else
        {
            edge[i]->currentColor = myColor2[1];
            edge[i]->targetColor = myColor2[2];
        }
    }
}

void SLList::run()
{
    eventView();

    if (option.isInitialize())
    {
        func = Function::INIT;
        box.IniFunction = true;
    }
    if (!box.IniFunction)
    {
        if (option.isAdd())
            func = Function::INSERT;
        if (option.isDelete())
            func = Function::DELETE;
        if (option.isSearch())
            func = Function::SEARCH;
        if (option.isUpdate())
            func = Function::UPDATE;
        
    }

    if (!box.isOpen && func != Function::NONE)
    {
        switch (func)
        {
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
        case Function::UPDATE:
            remove();
            updateData();
            func = Function::NONE;
            break;
        default:
            break;
        }
    }

    stepmanager.speed = slider.speed;

    if (stepmanager.isTransitioning)
    {
        stepmanager.updateTransitionProgress();
        if (stepmanager.isTransitionComplete())
            stepmanager.finishTransition();
    }
    else if (panel.isForwardPressed())
    {
        stepmanager.isPlaying = false;
        panel.isPlaying = false;
        if (stepmanager.currentStep < stepmanager.step.size() - 1)
        {
            stepmanager.nextStep();
            prepareTransition();
        }
    }
    else if (panel.isRewindPressed())
    {
        stepmanager.isPlaying = false;
        panel.isPlaying = false;
        stepmanager.prevStep();
    }
    if (panel.isPausePressed())
    {
        stepmanager.isPlaying = false;
        panel.isPlaying = false;
        accumulatedTime = 0.0f;
    }
    if (panel.isPlayPressed())
    {
        stepmanager.isPlaying = true;
        panel.isPlaying = true;
    }
    auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastUpdateTime).count();
    lastUpdateTime = now;
    if (stepmanager.isPlaying && stepmanager.currentStep < stepmanager.step.size() - 1)
    {
        accumulatedTime += deltaTime * stepmanager.speed * 2; // x2 speed
        while (accumulatedTime >= stepDuration && stepmanager.isPlaying)
        {
            accumulatedTime -= stepDuration;
            if (stepmanager.isTransitioning)
            {
                stepmanager.updateTransitionProgress();
                if (stepmanager.isTransitionComplete())
                    stepmanager.finishTransition();
            }
            else
            {
                stepmanager.nextStep();
                prepareTransition();
            }
        }
    }
    else
    {
        stepmanager.isPlaying = false;
        panel.isPlaying = false;
    }
}

void SLList::remove()
{
    code.codeline.clear();
    for (int i = 0; i < stepmanager.step.size(); ++i)
    {
        ListNode *curr = (ListNode *)stepmanager.step[i].tempRoot;
        while (curr)
        {
            ListNode *nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        stepmanager.step[i].tempRoot = nullptr;
        for (int j = 0; j < stepmanager.step[i].tempEdge.size(); ++j)
        {
            delete stepmanager.step[i].tempEdge[j];
        }
    }
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
}

void SLList::exit()
{
    code.codeline.clear();
    log.infor.clear();
    ListNode *curr = root;
    while (curr)
    {
        ListNode *nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    root = nullptr;
    for (int j = 0; j < edge.size(); ++j)
    {
        delete edge[j];
        edge[j] = nullptr;
    }
    for (int i = 0; i < stepmanager.step.size(); ++i)
    {
        ListNode *curr = (ListNode *)stepmanager.step[i].tempRoot;
        while (curr)
        {
            ListNode *nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        stepmanager.step[i].tempRoot = nullptr;
        for (int j = 0; j < stepmanager.step[i].tempEdge.size(); ++j)
        {
            delete stepmanager.step[i].tempEdge[j];
            stepmanager.step[i].tempEdge[j] = nullptr;
        }
    }
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
    edge.clear();
}

void SLList::copyNode(ListNode *source, Node *&des)
{
    if (!source)
    {
        des = nullptr;
        return;
    }
    ListNode *alias = nullptr;
    alias = new ListNode(source->val);
    alias->ID = source->ID;
    ListNode *currSource = source->next;
    ListNode *currDes = alias;
    while (currSource)
    {
        currDes->next = new ListNode(currSource->val);
        currDes = currDes->next;
        currDes->ID = currSource->ID;
        currSource = currSource->next;
    }
    currDes->next = nullptr;
    des = (Node *)alias;
}

void SLList::copyEdge(std::vector<Edge *> source, std::vector<Edge *> &des, ListNode *head)
{
    if (source.size() == 0)
    {
        des.clear();
        return;
    }
    des.resize(source.size());
    for (int i = 0; i < source.size(); ++i)
    {
        des[i] = new Edge(findNodebyID(head, source[i]->endPoint1->ID), findNodebyID(head, source[i]->endPoint2->ID));
        des[i]->ID = source[i]->ID;
    }
}

SLList::ListNode *SLList::findNodebyID(ListNode *head, int ID)
{
    if (!head)
        return nullptr;
    ListNode *curr = head;
    while (curr)
    {
        if (curr->ID == ID)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

Edge *SLList::findEdgebyEndPoint(std::vector<Edge *> list, int endPoint2ID)
{
    if (list.size() == 0)
        return nullptr;
    for (int i = 0; i < list.size(); ++i)
    {
        if (list[i]->endPoint2->ID == endPoint2ID)
            return list[i];
    }
    return nullptr;
}

void SLList::prepareTransition()
{
    Step &currStep = stepmanager.step[stepmanager.currentStep];
    if (stepmanager.currentStep <= 0)
        return;
    Step &prevStep = stepmanager.step[stepmanager.currentStep - 1];
    calculatePosition((ListNode *)currStep.tempRoot);
    calculatePosition((ListNode *)prevStep.tempRoot);
    std::unordered_map<int, ListNode *> currNode;
    std::unordered_map<int, ListNode *> prevNode;
    std::unordered_map<int, Edge *> currEdge;
    std::unordered_map<int, Edge *> prevEdge;
    ListNode *curr = (ListNode *)currStep.tempRoot;
    while (curr)
    {
        currNode[curr->ID] = curr;
        curr = curr->next;
    }
    curr = (ListNode *)prevStep.tempRoot;
    while (curr)
    {
        prevNode[curr->ID] = curr;
        curr = curr->next;
    }
    for (int i = 0; i < currStep.tempEdge.size(); ++i)
    {
        currEdge[currStep.tempEdge[i]->ID] = currStep.tempEdge[i];
    }
    for (int i = 0; i < prevStep.tempEdge.size(); ++i)
    {
        prevEdge[prevStep.tempEdge[i]->ID] = prevStep.tempEdge[i];
    }
    Animation anim;
    anim.type = AnimateType::DELETION;
    for (auto it : prevNode)
    {
        if (currNode.find(it.first) == currNode.end())
        {
            anim.deletedNode.push_back(it.second);
        }
    }
    for (auto it : prevEdge)
    {
        if (currEdge.find(it.first) == currEdge.end())
        {
            anim.deletedEdge.push_back(it.second);
        }
    }
    if (anim.deletedEdge.size() || anim.deletedNode.size())
        currStep.animQueue.addAnimation(anim);
    anim.type = AnimateType::MOVEMENT;
    for (auto it : prevNode)
    {
        if (currNode.find(it.first) != currNode.end())
        {
            anim.movedNode.push_back({it.second, currNode.find(it.first)->second});
        }
    }
    if (anim.movedNode.size())
        currStep.animQueue.addAnimation(anim);
    anim.type = AnimateType::INSERTION;
    for (auto it : currNode)
    {
        if (prevNode.find(it.first) == prevNode.end())
        {
            it.second->alpha = 0.0f;
            anim.insertedNode.push_back(it.second);
        }
    }
    for (auto it : currEdge)
    {
        if (prevEdge.find(it.first) == prevEdge.end())
        {
            it.second->alpha = 0.0f;
            anim.insertedEdge.push_back(it.second);
        }
    }
    if (anim.insertedNode.size() || anim.insertedEdge.size())
        currStep.animQueue.addAnimation(anim);
}

void SLList::safeRemoveEdge(int nodeID)
{
    Edge *edgeToRemove = findEdgebyEndPoint(edge, nodeID);
    if (edgeToRemove)
    {
        auto it = find(edge.begin(), edge.end(), edgeToRemove);
        if (it != edge.end())
        {
            delete *it;
            edge.erase(it);
        }
    }
}

void SLList::initData()
{
    if (box.someList.empty())
        return;
    root = new ListNode(box.someList[0]);
    ListNode *curr = root;
    for (int i = 1; i < box.someList.size(); ++i)
    {
        curr->next = new ListNode(box.someList[i]);
        Edge *line = new Edge(curr, curr->next);
        edge.push_back(line);
        curr = curr->next;
    }
    box.someList.clear();
}

void SLList::insertData()
{
    if (box.someList.empty())
        return;
    code.codeline = {
        "if !root: root = new Node(x); return",
        "curr = root                         ",
        "while curr.next: curr = curr.next   ",
        "curr.next = new Node(x)             "};
    for (int i = 0; i < box.someList.size(); ++i)
    {
        Step step;
        step.highlightedNode = -1;
        step.highlightedLine = 0;
        copyNode(root, step.tempRoot);
        copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
        stepmanager.step.push_back(step);
        if (!root)
        {
            root = new ListNode(box.someList[i]);
            step.highlightedLine = 0;
            step.highlightedNode = root->ID;
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
            continue;
        }

        step.highlightedNode = root->ID;
        step.highlightedLine = 1;
        copyNode(root, step.tempRoot);
        copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
        stepmanager.step.push_back(step);

        ListNode *curr = root;
        while (curr && curr->next)
        {
            curr = curr->next;
            step.highlightedNode = curr->ID;
            step.highlightedLine = 2;
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
        }

        if (curr)
        {
            curr->next = new ListNode(box.someList[i]);
            Edge *line = new Edge(curr, curr->next);
            edge.push_back(line);
            step.highlightedNode = curr->next->ID;
            step.highlightedLine = 3;
            step.description.push_back("Insert number " + std::to_string(box.someList[i]) + " successfully");
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
        }
    }
    box.someList.clear();
}

void SLList::deleteData()
{
    if (box.someList.empty())
        return;

    code.codeline = {
        "if empty list: return              ",
        "set curr = root, prev = null       ",
        "while curr exists and curr.val ≠ x:",
        "    prev = curr                    ",
        "    curr = curr.next               ",
        "if curr not found: return          ",
        "if deleting head (prev == null):   ",
        "    root = root.next               ",
        "    delete curr                    ",
        "else: prev.next = curr.next        ",
        "      delete curr                  "};
    for (int i = 0; i < box.someList.size(); ++i)
    {
        Step step;
        if (!root)
        {
            step.highlightedNode = -1;
            step.highlightedLine = 0;
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
            continue;
        }
        copyNode(root, step.tempRoot);
        copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
        step.highlightedLine = 1;
        step.highlightedNode = root->ID;
        stepmanager.step.push_back(step);

        ListNode *curr = root;
        ListNode *prev = nullptr;
        while (curr && curr->val != box.someList[i])
        {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 2;
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
            step.highlightedNode = curr->ID;
            step.highlightedLine = 3;
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
            prev = curr;
            step.highlightedLine = 4;
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
            curr = curr->next;
        }

        if (!curr)
        {
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            step.highlightedLine = 5;
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " is not in the list");
            stepmanager.step.push_back(step);
            continue;
        }

        if (!prev)
        {
            safeRemoveEdge(root->next->ID);
            root = root->next;
            delete curr;
            curr = root;
            step.highlightedLine = 8;
            step.highlightedNode = curr->ID;
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            step.description.push_back("Delete number " + std::to_string(box.someList[i]) + " successfully");
            stepmanager.step.push_back(step);
        }
        else
        {
            safeRemoveEdge(curr->ID);
            safeRemoveEdge(curr->next->ID);
            prev->next = curr->next;
            if (curr->next)
            {
                Edge *line = new Edge(prev, curr->next);
                edge.push_back(line);
            }
            delete curr;
            curr = prev;
            step.highlightedLine = 10;
            step.highlightedNode = prev->ID;
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
        }
    }
    box.someList.clear();
}

void SLList::searchData()
{
    if (box.someList.empty())
        return;

    code.codeline = {
        "if list empty: return false        ",
        "curr = root                        ",
        "while curr exists and curr.val ≠ x:",
        "   curr = curr.next                ",
        "return curr exists                 "};
    for (int i = 0; i < box.someList.size(); ++i)
    {
        Step step;
        if (!root)
        {
            step.highlightedNode = -1;
            step.highlightedLine = 0;
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " hasn't found");
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
            continue;
        }

        copyNode(root, step.tempRoot);
        copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
        step.highlightedLine = 1;
        stepmanager.step.push_back(step);

        ListNode *curr = root;
        while (curr && curr->val != box.someList[i])
        {
            step.highlightedNode = curr->ID;
            step.highlightedLine = 2;
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
            step.highlightedNode = curr->ID;
            step.highlightedLine = 3;
            copyNode(root, step.tempRoot);
            copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
            stepmanager.step.push_back(step);
            curr = curr->next;
        }

        step.highlightedLine = 4;
        copyNode(root, step.tempRoot);
        copyEdge(edge, step.tempEdge, (ListNode *)step.tempRoot);
        if (!curr)
        {
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " hasn't found");
        }
        else
        {
            step.highlightedNode = curr->ID;
            step.description.push_back("Number " + std::to_string(box.someList[i]) + " has found");
        }
        stepmanager.step.push_back(step);
    }
    box.someList.clear();
}

void SLList::updateData(){

}