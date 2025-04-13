    #include "Graph.hpp"
    using namespace std;

    void Graph::GraphNode::applySpringForce() {
        if (!isDragging) {
            Vector2 diff = {fixedPosition.x - position.x, fixedPosition.y - position.y};
            velocity.x += diff.x * 0.05f;
            velocity.y += diff.y * 0.05f;
        }
    }

    void Graph::GraphNode::applyDragForce() {
        velocity.x *= (1-0.01f);
        velocity.y *= (1-0.01f);
    }

    void Graph::GraphNode::updatePosition() {
        position.x += velocity.x;
        position.y +=velocity.y;
    }

    void Graph::applyElectricForce() {
        for (int i = 0; i < vertex.size(); ++i) {
            for (int j = i + 1; j < vertex.size(); ++j) {
                Vector2 diff = {vertex[i]->position.x - vertex[j]->position.x, vertex[i]->position.y - vertex[j]->position.y };
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (distance > 100) continue;
                if (distance < 1.0f) distance = 1.0f;
                float force = 5000.0f / (distance * distance);
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
                    std::cout << "Node selected: " << i << std::endl; 
                    break;
                }
            }
        }
    
        if (dragging && selectedNode) {
            selectedNode->position = mousePos;
            selectedNode->fixedPosition = mousePos;
            std::cout << "Dragging node to: " << mousePos.x << ", " << mousePos.y << std::endl; // Debug
        }
    
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            std::cout << "Mouse released" << std::endl; 
            dragging = false;
            if (selectedNode) {
                selectedNode->isDragging = false;
                std::cout << "Node released" << std::endl; 
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
        std::uniform_real_distribution<float> distx(400.0f,1440.0f);
        std::uniform_real_distribution<float> disty(80.0f,720.0f);
        for (int i = 0; i < vertex.size(); ++i) {
            vertex[i]->position.x = distx(gen);
            vertex[i]->fixedPosition.x = vertex[i]->position.x;
            vertex[i]->position.y = disty(gen);
            vertex[i]->fixedPosition.y = vertex[i]->position.y;
        }
    }

    void Graph::init() {
        initView();
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
        if (option.isInitialize()) func = Function::INIT;
        if (option.isAdd()) func = Function::INSERT;
        if (option.isDelete()) func = Function::DELETE;
        if (option.isSearch()) func = Function::SEARCH;

        if (!box.isOpen && func != Function::NONE) {
            switch (func) {
                case Function::INIT:
                    exit();
                    initData();
                    func = Function::NONE;
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
        Step& currStep = stepmanager.step[stepmanager.currentStep];
        if (stepmanager.currentStep <= 0) return;
        Step& prevStep = stepmanager.step[stepmanager.currentStep - 1];
        std::unordered_map<int,GraphNode*> currNode;
        std::unordered_map<int,GraphNode*> prevNode;
        std::unordered_map<int,Edge*> currEdge;
        std::unordered_map<int,Edge*> prevEdge;
        for (int i = 0; i <currStep.nodeHighlight.size(); ++i) {
            currNode[currStep.nodeHighlight[i]->ID] = (GraphNode*)currStep.nodeHighlight[i];
        }
        for (int i = 0; i <prevStep.nodeHighlight.size(); ++i) {
            currNode[prevStep.nodeHighlight[i]->ID] = (GraphNode*)prevStep.nodeHighlight[i];
        }
        for (int i = 0; i < currStep.edgeHighlight.size(); ++i) {
            currEdge[currStep.edgeHighlight[i]->ID] = currStep.edgeHighlight[i];
        }
        for (int i = 0; i < prevStep.edgeHighlight.size(); ++i) {
            prevEdge[prevStep.edgeHighlight[i]->ID] = prevStep.edgeHighlight[i];
        }
        Animation anim;
        anim.type = AnimateType::HIGHLIGHT;
        for (auto it : currNode) {
            if (prevNode.find(it.first) == prevNode.end()) {
                it.second->alpha = 0.0f;
                anim.highlightedNode.push_back(it.second);
            }
        }
        if (anim.highlightedNode.size()) currStep.animQueue.addAnimation(anim);
        anim.highlightedNode.clear();
        for (auto it : currEdge) {
            if (prevEdge.find(it.first) == prevEdge.end()) {
                it.second->alpha = 0.0f;
                anim.highlightedEdge.push_back(it.second);
            }
        }
        if (anim.highlightedEdge.size()) currStep.animQueue.addAnimation(anim);
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
        code.codeline.clear();
        stepmanager.step.clear();
        stepmanager.currentStep = 0;
    }

    void Graph::initData() {
        if (box.adjMatrix.size() == 0) return;
        vertex.resize(box.adjMatrix.size(),nullptr);
        for (int i = 0; i < vertex.size(); ++i) {
            vertex[i] = new GraphNode;
            vertex[i]->val = i;
        }
        for (int i = 0; i < box.adjMatrix.size() - 1; ++i) {
            for (int j = i + 1; j < box.adjMatrix.size(); ++j) {
                if (box.adjMatrix[i][j]) {
                    Edge* line = new Edge;
                    line->endPoint1 = vertex[i];
                    line->endPoint2 = vertex[j];
                    edge.push_back(line);
                }
            }
        }
        do {
            generatePosition();
        } while (!checkValidPos());
    }

    void Graph::dijkstra() {
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
        
        int n = box.adjMatrix.size();
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
        cout << "toi day van dung" << endl;
        while (!pq.empty()){
            {
                step.highlightedLine = 3;
                stepmanager.step.push_back(step);
            }
            int u = pq.top().second;
            pq.pop();
            {
                step.highlightedLine = 4;
                step.nodeHighlight.push_back(vertex[u]);
                if (parent[u] != -1) {
                    for (auto e : edge) {
                        if (!e || !e->endPoint1 || !e->endPoint2) continue;
                        if ((e->endPoint1->ID == vertex[u]->ID && e->endPoint2->ID == vertex[parent[u]]->ID) ||
                            (e->endPoint1->ID == vertex[parent[u]]->ID && e->endPoint2->ID == vertex[u]->ID)) {
                            step.edgeHighlight.push_back(e);
                            break;
                        }
                    }
                }
                stepmanager.step.push_back(step);
            }

            for (auto x : adjList[u]){
                
                step.highlightedLine = 5;
                step.nodeHighlight.clear();
                step.edgeHighlight.clear();
                stepmanager.step.push_back(step);
                
                int v = x.first;
                int w = x.second;
                {
                    step.highlightedLine = 6;
                    step.nodeHighlight.push_back(vertex[v]);
                    for (auto e : edge) {
                        if (!e || !e->endPoint1 || !e->endPoint2) continue;
                        if ((e->endPoint1->ID == vertex[u]->ID && e->endPoint2->ID == vertex[v]->ID) ||
                            (e->endPoint1->ID == vertex[v]->ID && e->endPoint2->ID == vertex[u]->ID)) {
                            step.edgeHighlight.push_back(e);
                            break;
                        }
                    }
                    stepmanager.step.push_back(step);
                    step.nodeHighlight.pop_back();
                }

                if (dist[v] > dist[u] + w){
                    step.highlightedLine = 7;
                    stepmanager.step.push_back(step);
                    parent[v] = u;
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                    {
                        step.highlightedLine = 8;
                        step.nodeHighlight.push_back(vertex[v]);
                        stepmanager.step.push_back(step);
                        step.nodeHighlight.pop_back();
                    }
                }
                step.edgeHighlight.pop_back();
            }
        }
        step.highlightedLine = 9;
        stepmanager.step.push_back(step);
        cout << "chay het shortest" << endl;
    }

    void Graph::checkConnected() {
        DSU dsu(box.adjMatrix.size());
        int root = dsu.find(0);
        bool connected = true;
        for (int i = 0; i < dsu.parent.size(); i++){
            if (dsu.find(i) != root)
                connected = false;
        }
    }

    void Graph::prim() {
        // Kiểm tra đồ thị rỗng
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
            step.nodeHighlight.clear();
            if (vertex[src]) {  // Kiểm tra null pointer
                step.nodeHighlight.push_back(vertex[src]);
            }
            stepmanager.step.push_back(step);
        }
    
        while (!pq.empty()) {
            // Bước 3: Lấy đỉnh từ heap
            {
                step.highlightedLine = 2;
                step.nodeHighlight.clear();
                step.edgeHighlight.clear();
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
                step.nodeHighlight.clear();
                step.nodeHighlight.push_back(vertex[u]);
                
                // Hiển thị cạnh nếu có đỉnh cha hợp lệ
                if (p != -1 && p >= 0 && p < V && vertex[p]) {
                    for (auto e : edge) {
                        if (!e || !e->endPoint1 || !e->endPoint2) continue;
                        
                        if ((e->endPoint1->ID == vertex[u]->ID && e->endPoint2->ID == vertex[p]->ID) ||
                            (e->endPoint1->ID == vertex[p]->ID && e->endPoint2->ID == vertex[u]->ID)) {
                            step.edgeHighlight.push_back(e);
                            break;
                        }
                    }
                }
                stepmanager.step.push_back(step);
            }
    
            // Bước 5: Bỏ qua nếu đã thăm
            if (visited[u]) {
                step.highlightedLine = 4;
                step.nodeHighlight.pop_back();
                stepmanager.step.push_back(step);
                continue;
            }
    
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
                    step.nodeHighlight.clear();
                    step.nodeHighlight.push_back(vertex[u]);
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
                    step.nodeHighlight.push_back(vertex[v]);
                    
                    // Tìm và highlight cạnh
                    for (auto e : edge) {
                        if (!e || !e->endPoint1 || !e->endPoint2) continue;
                        
                        if ((e->endPoint1->ID == vertex[u]->ID && e->endPoint2->ID == vertex[v]->ID) ||
                            (e->endPoint1->ID == vertex[v]->ID && e->endPoint2->ID == vertex[u]->ID)) {
                            step.edgeHighlight.push_back(e);
                            break;
                        }
                    }
                    
                    pq.push({weight, v, u});
                    stepmanager.step.push_back(step);
                    
                    // Reset highlight sau khi thêm vào heap
                    step.edgeHighlight.clear();
                }
            }
        }
    
        // Bước 9: Hoàn thành
        step.highlightedLine = 9;
        stepmanager.step.push_back(step);
    
        cout << "Complete PRIM. Total weight: " << sum << endl;
    }

    bool Comparator(std::vector<int> a, std::vector<int> b){
        return a[2] < b[2];
    }

    void Graph::kruskal() {
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
            step.nodeHighlight.push_back(vertex[u]);
            step.nodeHighlight.push_back(vertex[v]);
            stepmanager.step.push_back(step);

            if (dsu.find(u) != dsu.find(v)){
                step.highlightedLine = 5;
                stepmanager.step.push_back(step);

                dsu.unite(u, v);
                sum += w;
                count++;
                {
                    step.highlightedLine = 6;
                    step.nodeHighlight = {vertex[u], vertex[v]};
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
            } else {
                step.nodeHighlight.pop_back();
                step.nodeHighlight.pop_back();
            }
        }
        step.highlightedLine = 8;
        step.description.push_back(std::to_string(sum));
        stepmanager.step.push_back(step);

        cout << "Complete Kruksal, total weight: " << sum << endl;
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
        int n = box.adjMatrix.size();
    
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {  // chỉ duyệt tam giác trên
                if (box.adjMatrix[i][j] > 0) {
                    edges.push_back({i, j, box.adjMatrix[i][j]});
                }
            }
        }
    }
    

    void Graph::edgesToAdjList(){
        if (edges.empty()) return;
        adjList.clear();
        adjList.resize(box.adjMatrix.size());

        for (auto e : edges){
            int u = e[0];
            int v = e[1];
            int w = e[2];

            adjList[u].push_back({v, w});
            adjList[v].push_back({u, w});
        }
    }

