#include "Graph.hpp"
using namespace std;

void Graph::GraphNode::applySpringForce() {
    if (!isDragging) {
        Vector2 diff = {fixedPosition.x - position.x, fixedPosition.y - position.y};
        velocity.x += diff.x * 0.01f;
        velocity.y += diff.y * 0.01f;
    }
}

void Graph::GraphNode::applyDragForce() {
    velocity.x *= (1- 0.01f);
    velocity.y *= (1- 0.01f);
}

void Graph::GraphNode::updatePosition() {
    position.x += velocity.x;
    position.y += velocity.y;
}

void Graph::applyElectricForce() {
    for (int i = 0; i < vertex.size(); ++i) {
        for (int j = i + 1; j < vertex.size(); ++j) {
            Vector2 diff = {vertex[i]->position.x - vertex[j]->position.x, vertex[i]->position.y - vertex[j]->position.y };
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (distance > 100) continue;
            if (distance < 1.0f) distance = 1.0f;
            float force = 50000.0f / (distance * distance);
            Vector2 electricForce = {force * diff.x / distance, force * diff.y / distance};
            vertex[i]->position.x += electricForce.x;
            vertex[i]->position.y += electricForce.y;
            vertex[j]->position.x -= electricForce.x;
            vertex[j]->position.y -= electricForce.y;
        }
    }
}

void Graph::updateVertex() {
    handleMouse();
    applyElectricForce();
    for (int i = 0; i < vertex.size(); ++i) {
        if (!vertex[i]->isDragging) {
            vertex[i]->applySpringForce();
            vertex[i]->applyDragForce();
        }
        vertex[i]->updatePosition();
    }
}

void Graph::handleMouse() {
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (int i = 0; i < vertex.size(); ++i) {
            if (CheckCollisionPointCircle(mousePos, vertex[i]->position, 30)) {
                dragging = true;
                selectedNode = vertex[i];
                vertex[i]->isDragging = true;
                break;
            }
        }
    }

    if (dragging && selectedNode) {
        selectedNode->position = mousePos;
        selectedNode->fixedPosition = mousePos;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        dragging = false;
        if (selectedNode) {
            selectedNode->isDragging = false;
        }
        selectedNode = nullptr;
    }
}

bool Graph::checkValidPos() {
for (int i = 0; i < vertex.size() - 1; ++i) {
        for (int j = i + 1; j < vertex.size(); ++j) {
            Vector2 diff = {vertex[i]->position.x - vertex[j]->position.x, vertex[i]->position.y - vertex[j]->position.y};
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (distance < 100.0f) return false;
        }
}
return true;
}

void Graph::generatePosition() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distx(460.0f,1380.0f);
    std::uniform_real_distribution<float> disty(140.0f,660.0f);
    for (int i = 0; i < vertex.size(); ++i) {
        vertex[i]->position.x = distx(gen);
        vertex[i]->fixedPosition.x = vertex[i]->position.x;
        vertex[i]->position.y = disty(gen);
        vertex[i]->fixedPosition.y = vertex[i]->position.y;
    }
}

void Graph::init() {
    initView();
    box.adjMatrix.clear();
    stepmanager.currentStep = 0;
    SetMousePosition(780,400);
}

void Graph::draw() {
    drawView();
    if (vertex.size() == 0) return;
    if (!stepmanager.step.empty() && stepmanager.currentStep >= 0) {
        Step currStep = stepmanager.step[stepmanager.currentStep];
        log.infor = currStep.description;
        code.lineHighlighted = currStep.highlightedLine;
        BeginScissorMode(400,80,1040,640);
        BeginMode2D(camera);
        if (stepmanager.isTransitioning) {
            Animation currAnimation = currStep.animQueue.animation.front();
            Step& prevStep = stepmanager.step[stepmanager.currentStep - 1];
            if (currAnimation.type == AnimateType::HIGHLIGHT) {
                resetColorEdge();
                resetColorNode();
                drawEdge(edge);
                drawNode(vertex);
            } else {
                drawEdge(edge);
                drawNode(vertex);
            }
        } else {
            resetProgressNode(currStep);
            resetProgressEdge(currStep);
            resetColorEdge();
            resetColorNode();
            drawEdge(edge);
            drawNode(vertex);
        }
        EndMode2D();
        EndScissorMode();
    } else {
        BeginScissorMode(400,80,1040,640);
        BeginMode2D(camera);
        resetColorEdge();
        resetColorNode();
        drawEdge(edge);
        drawNode(vertex);
        EndMode2D();
        EndScissorMode();
    }
}

void Graph::drawNode(std::vector<GraphNode*> vertex) {
    for (int i = 0; i < vertex.size(); ++i) {
        vertex[i]->drawAnimation();
    }
}

void Graph::drawEdge(std::vector<Edge*> edge) {
    for (int i = 0; i < edge.size(); ++i) {
        edge[i]->drawAnimation();
    }
}

void Graph::resetColorNode() {
    for (int i = 0; i < vertex.size(); ++i) {
        if (theme == colorType::HOT) {
            vertex[i]->currentColor = myColor1[0];
            vertex[i]->targetColor = myColor1[2];
        } else {
            vertex[i]->currentColor = myColor2[0];
            vertex[i]->targetColor = myColor2[2];
        }      
    }
}

void Graph::resetColorEdge() {
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

void Graph::resetProgressNode(Step step) {
    for (int i = 0; i < vertex.size(); ++i) {
        vertex[i]->progress = 0.0f;
    }

    for (int i = 0; i < step.nodeHighlight.size(); ++i) {
        step.nodeHighlight[i]->progress = 1.0f;
    }
}
    
void Graph::resetProgressEdge(Step step) {
    for (int i = 0; i < edge.size(); ++i) {
        edge[i]->progress = 0.0f;
    }
    for (int i = 0; i < step.edgeHighlight.size(); ++i) {
        step.edgeHighlight[i]->progress = 1.0f;
    }
}

void Graph::run() {
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
                dijkstra();
                func = Function::NONE;
                break;
            case Function::DELETE:
                remove();
                kruskal();
                func = Function::NONE;
                break;
            case Function::SEARCH:
                remove();
                prim();
                func = Function::NONE;
                break;
            default:
                break;
        }
    }
    stepmanager.speed = slider.speed; 

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
    
    updateVertex();

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

void Graph::prepareTransition() {
    if (stepmanager.currentStep <= 0) return;
    stepmanager.step[stepmanager.currentStep].animQueue.clear();
    
    Step& currStep = stepmanager.step[stepmanager.currentStep];
    Step& prevStep = stepmanager.step[stepmanager.currentStep - 1];
    
    Animation anim;
    anim.type = AnimateType::HIGHLIGHT;
    
    for (auto node : currStep.nodeHighlight) {
        if (std::find(prevStep.nodeHighlight.begin(), prevStep.nodeHighlight.end(), node) == prevStep.nodeHighlight.end()) {
            anim.highlightedNode.push_back(node);
            node->progress = 0.0f; 
        }
    }

    if (!anim.highlightedNode.empty()) {
        currStep.animQueue.addAnimation(anim);
        anim.highlightedNode.clear();
    }
    
    for (auto edge : currStep.edgeHighlight) {
        if (std::find(prevStep.edgeHighlight.begin(), 
                      prevStep.edgeHighlight.end(), edge) == prevStep.edgeHighlight.end()) {
            anim.highlightedEdge.push_back(edge);
            edge->progress = 0.0f;
        }
    }

    if (!anim.highlightedEdge.empty()) {
        currStep.animQueue.addAnimation(anim);
    }
    
    // Step& currStep = stepmanager.step[stepmanager.currentStep];
    // if (stepmanager.currentStep <= 0) return;
    // Step& prevStep = stepmanager.step[stepmanager.currentStep - 1];
    // std::unordered_map<int,GraphNode*> currNode;
    // std::unordered_map<int,GraphNode*> prevNode;
    // std::unordered_map<int,Edge*> currEdge;
    // std::unordered_map<int,Edge*> prevEdge;
    // for (int i = 0; i < currStep.nodeHighlight.size(); ++i) {
    //     currNode[currStep.nodeHighlight[i]->ID] = (GraphNode*)currStep.nodeHighlight[i];
    // }
    // for (int i = 0; i < prevStep.nodeHighlight.size(); ++i) {
    //     prevNode[prevStep.nodeHighlight[i]->ID] = (GraphNode*)prevStep.nodeHighlight[i];
    // }
    // for (int i = 0; i < currStep.edgeHighlight.size(); ++i) {
    //     currEdge[currStep.edgeHighlight[i]->ID] = currStep.edgeHighlight[i];
    // }
    // for (int i = 0; i < prevStep.edgeHighlight.size(); ++i) {
    //     prevEdge[prevStep.edgeHighlight[i]->ID] = prevStep.edgeHighlight[i];
    // }
    // Animation anim;
    // anim.type = AnimateType::HIGHLIGHT;
    // for (auto it : currNode) {
    //     if (prevNode.find(it.first) == prevNode.end()) {
    //         anim.highlightedNode.push_back(it.second);
    //     }
    // }
    // if (anim.highlightedNode.size()) currStep.animQueue.addAnimation(anim);
    // anim.highlightedNode.clear();
    // for (auto it : currEdge) {
    //     if (prevEdge.find(it.first) == prevEdge.end()) {
    //         anim.highlightedEdge.push_back(it.second);
    //     }
    // }
    // if (anim.highlightedEdge.size()) currStep.animQueue.addAnimation(anim);
}

void Graph::exit() {
    code.codeline.clear();
    box.someList.clear();
    log.infor.clear();
    for (int i = 0; i < vertex.size(); ++i) {
        delete vertex[i];
        vertex[i] = nullptr;
    }
    for (int i = 0; i < edge.size(); ++i) {
        delete edge[i];
        edge[i] = nullptr;
    }
    edges.clear();
    adjList.clear();
    vertex.clear();
    edge.clear();
    selectedNode = nullptr;
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
}

void Graph::remove() {
    for (int i = 0; i < vertex.size(); ++i) {
        vertex[i]->progress = 0.0f;
    }
    for (int i = 0; i < edge.size(); ++i) {
        edge[i]->progress = 0.0f;
    }
    edges.clear();
    adjList.clear();
    code.codeline.clear();
    stepmanager.step.clear();
    stepmanager.currentStep = 0;
}

void Graph::initData() {
    if (box.adjMatrix.size() == 0) return;
    vertex.resize(box.adjMatrix.size(),nullptr);
    for (int i = 0; i < vertex.size(); ++i) {
        vertex[i] = new GraphNode(i);
    }
    for (int i = 0; i < box.adjMatrix.size() - 1; ++i) {
        for (int j = i + 1; j < box.adjMatrix.size(); ++j) {
            if (box.adjMatrix[i][j]) {
                Edge* line = new Edge(vertex[i], vertex[j]);
                line->weight = box.adjMatrix[i][j];
                edge.push_back(line);
            }
        }
    }
    do {
        generatePosition();
    } while (!checkValidPos());
    box.adjMatrix.clear();
}

void Graph::dijkstra() {
    if (!checkConnected()) {
        log.infor.push_back("The graph must be connected to implement this function");
        return;
    } 
    adjMatrixToEdges();
    cout << "Thanh cong, number of edges: " << edges.size() << endl;
    if (edges.empty()) return;
    if (vertex.empty()){
        cout << "Has no vertex" << endl;
        return;
    }
    cout << "number of edges: " << edges.size() << endl;
    edgesToAdjList();
    cout << "Run" << endl;

    Step step;
    code.codeline = {
        "Create priority queue pq              ",
        "Initialize dist[0...n-1] ← ∞          ",
        "pq.push( (0, start) ); dist[start] ← 0",
        "while pq is not empty do              ",
        "u ← pq.top().second                   ",
        "for each (v, w) in adjMatrix[u] do    ",
        "if dist[v] > dist[u] + w then         ",
        "dist[v] ← dist[u] + w                 ",
        "pq.push( (dist[v], v) )               ",
        "Complete !                            "
    };
    
    int n = vertex.size();
    vector<int> parent(n,0);
    parent[box.startedVertex] = -1;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    {
        step.highlightedLine = 0;
        stepmanager.step.push_back(step);
    }

    vector<int> dist(n, INT_MAX);
    {
        step.highlightedLine = 1;
        stepmanager.step.push_back(step);
    }
    int src = box.startedVertex;
    pq.push({0, src});
    dist[src] = 0;
    {
        step.highlightedLine = 2;
        step.nodeHighlight.push_back(vertex[src]);
        stepmanager.step.push_back(step);
    }

    while (!pq.empty()){
        {
            step.highlightedLine = 3;
            stepmanager.step.push_back(step);
        }
        int u = pq.top().second;
        pq.pop();
        {
            step.highlightedLine = 4;
            step.description.push_back("The shortest path from " + to_string(src) + " -> " + to_string(u) + " : " + to_string(dist[u]));
            step.nodeHighlight.push_back(vertex[u]);
            if (parent[u] != -1) {
                for (auto &e : edge) {
                    if (!e || !e->endPoint1 || !e->endPoint2) continue;
                    if (e->endPoint1->ID == vertex[u]->ID && e->endPoint2->ID == vertex[parent[u]]->ID) {
                        std::swap(e->endPoint1, e->endPoint2);
                        step.edgeHighlight.push_back(e);
                        break;
                    }
                    
                    if (e->endPoint1->ID == vertex[parent[u]]->ID && e->endPoint2->ID == vertex[u]->ID) {
                        step.edgeHighlight.push_back(e);
                        break;
                    }
                }
            }
            stepmanager.step.push_back(step);
        }

        for (auto x : adjList[u]){
            
            step.highlightedLine = 5;
            stepmanager.step.push_back(step);
            
            int v = x.first;
            int w = x.second;
            {
                step.highlightedLine = 6;
                stepmanager.step.push_back(step);
            }

            if (dist[v] > dist[u] + w){
                step.highlightedLine = 7;
                stepmanager.step.push_back(step);
                parent[v] = u;
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
                {
                    step.highlightedLine = 8;
                    stepmanager.step.push_back(step);
                }
            }
        }
    }
    step.highlightedLine = 9;
    stepmanager.step.push_back(step);
}

bool Graph::checkConnected() {
    if (vertex.empty() || vertex.size() == 0) return false;
    
    DSU dsu(vertex.size());
    
    for (int i = 0; i < edge.size(); ++i) {
        if (edge[i] && edge[i]->endPoint1 && edge[i]->endPoint2) {
            dsu.unite(edge[i]->endPoint1->val,edge[i]->endPoint2->val);
        }
    }

    int root = dsu.find(0);
    for (int i = 1; i < vertex.size(); ++i) {
        if (dsu.find(i) != root) {
            return false; 
        }
    }

    return true;
}

void Graph::prim() {
    if (!checkConnected()) {
        log.infor.push_back("The graph must be connected to implement this function");
        return;
    } 
    adjMatrixToEdges();
    if (edges.empty() || vertex.empty()) {
        cout << "Error: Graph has no edges" << endl;
        return;
    }
    edgesToAdjList();

    // Kiểm tra số đỉnh hợp lệ
    int V = vertex.size();
    if (V == 0) {
        cout << "Error: Graph has no vertices" << endl;
        return;
    }

    // Kiểm tra đỉnh bắt đầu hợp lệ
    if (box.startedVertex < 0 || box.startedVertex >= V) {
        cout << "Error: Invalid start vertex (" << box.startedVertex << ")" << endl;
        return;
    }

    // Khởi tạo các bước như ban đầu
    Step step;
    code.codeline = {
        "Initialize min-heap pq                           ",
        "pq.push({0, start})                              ",
        "while pq is not empty do                         ",
        "u ← pq.top().second; w ← pq.top().first; pq.pop()",
        "if visited[u] is true then continue              ",
        "sum += w; mark visited[u] as true                ",
        "for each (v, weight) in adjList[u] do            ",
        "if visited[v] is false then                      ",
        "pq.push({weight, v})                             ",
        "Complete !                                       "
    };

    vector<bool> visited(V, false);
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
    
    // Bước 1: Khởi tạo min-heap
    {
        step.highlightedLine = 0;
        stepmanager.step.push_back(step);
    }

    int sum = 0;
    int src = box.startedVertex;

    // Bước 2: Thêm đỉnh nguồn vào heap
    pq.push({0, src, -1});
    {
        step.highlightedLine = 1;
        if (vertex[src]) {  // Kiểm tra null pointer
            step.nodeHighlight.push_back(vertex[src]);
        }
        stepmanager.step.push_back(step);
    }

    while (!pq.empty()) {
        // Bước 3: Lấy đỉnh từ heap
        {
            step.highlightedLine = 2;
            stepmanager.step.push_back(step);
        }

        auto top = pq.top();
        pq.pop();

        int u = get<1>(top);
        int w = get<0>(top);
        int p = get<2>(top);

        // Kiểm tra đỉnh u hợp lệ
        if (u < 0 || u >= V || !vertex[u]) continue;

        // Bước 4: Hiển thị đỉnh đang xét
        {
            step.highlightedLine = 3;
            step.nodeHighlight.push_back(vertex[u]);
        }

        if (visited[u]) {
            step.highlightedLine = 4;
            stepmanager.step.push_back(step);
            continue;
        }

        // Hiển thị cạnh nếu có đỉnh cha hợp lệ
        if (p != -1 && p >= 0 && p < V && vertex[p]) {
            for (auto e : edge) {
                if (!e || !e->endPoint1 || !e->endPoint2) continue;
                
                if (e->endPoint1->ID == vertex[u]->ID && e->endPoint2->ID == vertex[p]->ID) {
                    std::swap(e->endPoint1,e->endPoint2);
                    step.edgeHighlight.push_back(e);
                    break;
                }

                if (e->endPoint1->ID == vertex[p]->ID && e->endPoint2->ID == vertex[u]->ID) {
                    step.edgeHighlight.push_back(e);
                    break;
                }
                
            }
        }
        stepmanager.step.push_back(step);

        // Bước 6: Đánh dấu đã thăm
        sum += w;
        visited[u] = true;
        {
            step.highlightedLine = 5;
            stepmanager.step.push_back(step);
        }

        // Bước 7: Duyệt các đỉnh kề
        for (auto x : adjList[u]) {
            int v = x.first;
            int weight = x.second;

            // Kiểm tra đỉnh kề hợp lệ
            if (v < 0 || v >= V || !vertex[v]) continue;

            // Bước 7.1: Hiển thị đang xét đỉnh kề
            {
                step.highlightedLine = 6;
                stepmanager.step.push_back(step);
            }

            // Bước 7.2: Kiểm tra đã thăm
            {
                step.highlightedLine = 7;
                stepmanager.step.push_back(step);
            }

            // Bước 8: Thêm vào heap nếu chưa thăm
            if (!visited[v]) {
                step.highlightedLine = 8;
                pq.push({weight, v, u});
                stepmanager.step.push_back(step);
            }
        }
    }

    step.highlightedLine = 9;
    step.description.push_back("The minimum weight: " + to_string(sum));
    stepmanager.step.push_back(step);
}

bool Comparator(std::vector<int> a, std::vector<int> b){
    return a[2] < b[2];
}

void Graph::kruskal() {
    if (!checkConnected()) {
        log.infor.push_back("The graph must be connected to implement this function");
        return;
    } 
    adjMatrixToEdges();
    if (edges.empty()) {
        cout << "HAS NO EDGES" << endl;
        return;
    }

    Step step;
    code.codeline = {
        "Sort all edges by increasing weight",
        "count ← 0; sum ← 0                 ",
        "Initialize DSU with V nodes        ",
        "for each edge (u, v, w) in edges do",
        "if find(u) ≠ find(v) then          ",
        "Unite the sets of u and v          ",
        "sum ← sum + w                      ",
        "count ← count + 1                  ",
        "if count = V then break            ",
        "Complete !                         "
    };

    sort(edges.begin(), edges.end(), Comparator);
    {
        step.highlightedLine = 0;
        stepmanager.step.push_back(step);
    }

    int count = 0;
    int sum = 0;
    {
        step.highlightedLine = 1;
        stepmanager.step.push_back(step); 
    }

    int V = vertex.size();

    DSU dsu(V);
    {
        step.highlightedLine = 2;
        stepmanager.step.push_back(step);
    }
    for (auto e : edges){
        step.highlightedLine = 3;
        stepmanager.step.push_back(step);

        int u = e[0];
        int v = e[1];
        int w = e[2];

        step.highlightedLine = 4;
        stepmanager.step.push_back(step);

        if (dsu.find(u) != dsu.find(v)){
            step.highlightedLine = 5;
            stepmanager.step.push_back(step);

            dsu.unite(u, v);
            sum += w;
            count++;
            {
                step.highlightedLine = 6;
                step.nodeHighlight.push_back(vertex[u]);
                step.nodeHighlight.push_back(vertex[v]);
                for (auto e : edge){
                    if ((e->endPoint1->ID == vertex[u]->ID && e->endPoint2->ID == vertex[v]->ID) || (e->endPoint1->ID == vertex[v]->ID && e->endPoint2->ID == vertex[u]->ID)){
                        step.edgeHighlight.push_back(e);
                        break;
                    }
                }
                stepmanager.step.push_back(step);
            }
            if (count == V - 1) {
                step.highlightedLine = 7;
                stepmanager.step.push_back(step);
                break;
            }
        }
    }
    step.highlightedLine = 8;
    step.description.push_back("The minimum weight: " + to_string(sum));
    stepmanager.step.push_back(step);
}

int Graph::DSU::find(int a){
    if (a == parent[a]) return a;
    return parent[a] = find(parent[a]);
}

void Graph::DSU::unite(int a, int b){
    a = find(a);
    b = find(b);
    if (a != b){
        if (size[a] < size[b])
            std::swap(a, b);
        parent[b] = a;
        size[a] += size[b];
    }
}

void Graph::adjMatrixToEdges() {
    edges.clear();
    for (int i = 0; i < edge.size(); ++i) {
        edges.push_back({edge[i]->endPoint1->val,edge[i]->endPoint2->val,edge[i]->weight});
    }
}


void Graph::edgesToAdjList(){
    if (edge.empty()) return;
    adjList.clear();
    adjList.resize(vertex.size());

    for (auto e : edge){
        int u = e->endPoint1->val;
        int v = e->endPoint2->val;
        int w = e->weight;

        adjList[u].push_back({v, w});
        adjList[v].push_back({u, w});
    }
}

