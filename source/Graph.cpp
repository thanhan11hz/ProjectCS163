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
        velocity.x *= (1-0.1f);
        velocity.y *= (1-0.1f);
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
        applyElectricForce();
        for (int i = 0; i < vertex.size(); ++i) {
            vertex[i]->applySpringForce();
            vertex[i]->applyDragForce();
            vertex[i]->updatePosition();
        }
    }

    void Graph::handleMouse() {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < vertex.size(); ++i) {
                if (CheckCollisionPointCircle(mousePos,vertex[i]->position,20)) {
                    dragging = true;
                    selectedNode = vertex[i];
                    vertex[i]->isDragging = true;
                    break;
                }
            }
        }

        if (dragging && selectedNode) {
            selectedNode->position = mousePos;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            dragging = false;
            selectedNode = nullptr;
            selectedNode->isDragging = false;
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
            vertex[i]->position.y = disty(gen);
        }
    }

    void Graph::init() {
        initView();
        stepmanager.currentStep = 0;
        SetMousePosition(780,400);
    }

    void Graph::draw() {
        drawView();
        // if (vertex.size() == 0) return;
        // if (!stepmanager.step.empty() && stepmanager.currentStep >= 0) {
        //     Step currStep = stepmanager.step[stepmanager.currentStep];
        //     log.infor = currStep.description;
        //     code.lineHighlighted = currStep.highlightedLine;
        //     drawView();
        //     for (int i = 0; i < edge.size(); ++i) {
        //         // if (edge[i]->ID == currStep.highlightedEdge) edge[i]->endPoint1->drawHighlightedEdge(edge[i]->endPoint2);
        //         // else edge[i]->endPoint1->drawEdge(edge[i]->endPoint2);
        //     }
        //     for (int i = 0; i < vertex.size(); ++i) {
        //         if (vertex[i]->ID == currStep.highlightedNode) vertex[i]->drawHighlightNode();
        //         else vertex[i]->drawNode();
        //     }
        // } else {
        //     for (int i = 0; i < edge.size(); ++i) {
        //         edge[i]->endPoint1->drawEdge(edge[i]->endPoint2);
        //     }
        //     for (int i = 0; i < vertex.size(); ++i) {
        //         vertex[i]->drawNode();
        //     }
        // }
    }

    void Graph::run() {
        eventView();
        
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
        code.codeline.clear();
        stepmanager.step.clear();
        stepmanager.currentStep = 0;
    }

    void Graph::initData() {
        do {
            generatePosition();
        } while (checkValidPos());
    }

    void Graph::dijkstra() {
        if (edges.empty()) return;
        EdgestoADJList();

        Step step;
        code.codeline = {
            "n = ADJmatrix.size(); priority_queue<...> pq; ",
            "vector<int> dist(n, INT_MAX);                 ",
            "pq.push({0, start}); dist[start]=0;           ",
            "while (!pq.empty()){                          ",
            "u = pq.top().second                           ",
            "for (auto x : ADJmatrix[u])                   ",
            "v=x.first; w = x.second                       ",
            "if (dist[v] > dist[u] + w)                    ",
            "dist[v] = dist[u] + w;                        ",
            "pq.push({dist[v], v});}}}                     ",
            "Complete !                                    ",
        };

        int n = box.ADJmatrix.size();
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
            step.nodeHightlight.push_back(vertex[src]);
            stepmanager.step.push_back(step);
        }

        while (!pq.empty()){
            {
                step.highlightedLine = 3;
                step.nodeHightlight.clear();
                step.edgeHighlight.clear();
                stepmanager.step.push_back(step);
            }
            int u = pq.top().second;
            pq.pop();
            {
                step.highlightedLine = 4;
                step.nodeHightlight.push_back(vertex[u]);
                stepmanager.step.push_back(step);
            }

            for (auto x : ADJList[u]){
                
            step.highlightedLine = 5;
            step.nodeHightlight.clear();
            step.edgeHighlight.clear();
            stepmanager.step.push_back(step);
                
                int v = x.first;
                int w = x.second;
                {
                    step.highlightedLine = 6;
                    step.nodeHightlight.push_back(vertex[v]);
                    for (auto e : edge) {
                        if ((e->endPoint1->ID == vertex[u]->ID && e->endPoint2->ID == vertex[v]->ID) ||
                            (e->endPoint1->ID == vertex[v]->ID && e->endPoint2->ID == vertex[u]->ID)) {
                            step.edgeHighlight.push_back(e);
                            break;
                        }
                    }
                    stepmanager.step.push_back(step);
                }

                if (dist[v] > dist[u] + w){
                step.highlightedLine = 7;
                stepmanager.step.push_back(step);

                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                    {
                        step.highlightedLine = 8;
                        step.nodeHightlight.clear();
                        step.edgeHighlight.clear();
                        step.nodeHightlight.push_back(vertex[v]);
                        stepmanager.step.push_back(step);
                    }
                }
            }
        }
        step.highlightedLine = 9;
        step.nodeHightlight.clear();
        step.edgeHighlight.clear();
        stepmanager.step.push_back(step);
        // in ra vector dist
    }

    void Graph::checkConnected() {
        DSU dsu(box.ADJmatrix.size());
        int root = dsu.find(0);
        bool connected = true;
        for (int i = 0; i < dsu.parent.size(); i++){
            if (dsu.find(i) != root)
                connected = false;
        }
    }

    void Graph::prim() {
        if (edges.empty()) return;
        EdgestoADJList();

        int V = vertex.size();

        Step step;
        code.codeline = {
            "n = vertex.size(); priority_queue<>pq;  ",
            "pq.push({0, start});                    ",
            "while (!pq.empty()) {                   ",
            "u = pq.top().second; w = pq.top().first ",
            "if (visited[u]) continue;               ",
            "visited[u] = true; sum += w;            ",
            "for (auto x : ADJList[u])               ",
            "v = x.first; w = x.second               ",
            "if (!visited[v]) pq.push({w, v});}}}    ",
            "Complete!                               ",
        };

        vector<bool> visisted(V, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        {
            step.highlightedLine = 0;
            stepmanager.step.push_back(step);
        }
        int sum = 0;
        int src = box.startedVertex;

        pq.push({0, src});
        {
            step.highlightedLine = 1;
            step.nodeHightlight.push_back(vertex[src]);
            stepmanager.step.push_back(step);
        }

        while (!pq.empty()){
            {
                step.highlightedLine = 2;
                step.nodeHightlight.clear();
                step.edgeHighlight.clear();
                stepmanager.step.push_back(step);
            }
            auto top = pq.top();
            pq.pop();

            int u = top.second;
            int w = top.first;
            {
                step.highlightedLine = 3;
                step.nodeHightlight.push_back(vertex[u]);
                stepmanager.step.push_back(step);
            }

            if (visisted[u] == true) continue;
            {
                step.highlightedLine = 4;
                stepmanager.step.push_back(step);
            }

            sum += w;
            visisted[u] = true;
            {
                step.highlightedLine = 5;
                stepmanager.step.push_back(step);
            }

            for (auto x : ADJList[u]){
                step.highlightedLine = 6;
                step.nodeHightlight = {vertex[u]};
                stepmanager.step.push_back(step);

                int v = x.first;
                int weight = x.second;
                {
                    step.highlightedLine = 7;
                    stepmanager.step.push_back(step);
                }

                if (!visisted[v]){
                    step.highlightedLine = 8;
                    step.nodeHightlight.push_back(vertex[v]);
                    for (auto e : edge){
                        if ((e->endPoint1->ID == vertex[u]->ID && e->endPoint2->ID == vertex[v]->ID) || (e->endPoint1->ID == vertex[v]->ID && e->endPoint2->ID == vertex[u]->ID)){
                            step.edgeHighlight.push_back(e);
                            break;
                        }
                    }
                    pq.push({weight, v});
                    stepmanager.step.push_back(step);
                }
            }
        }
        step.highlightedLine = 9;
        step.edgeHighlight.clear();
        step.nodeHightlight.clear();
        stepmanager.step.push_back(step);
    }

    bool Comparator(std::vector<int> a, std::vector<int> b){
        return a[2] < b[2];
    }

    void Graph::kruskal() {
        if (edges.empty()) return;

        Step step;
        code.codeline = {
            "sort(edges.begin(), edges.end(), Comparator);"
            "int count = 0; int sum = 0"
            "DSU dsu(V);"
            "for (auto e : edges){"
            "u = e[0]; v = e[1], w = e[2]"
            "if (dsu.find(u) != dsu.find(v)){"
            "unite(u,v); sum+=w; count++"
            "if (count == V) break;"
            "Complete !"
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
            step.nodeHightlight.push_back(vertex[u]);
            step.nodeHightlight.push_back(vertex[v]);
            stepmanager.step.push_back(step);

            if (dsu.find(u) != dsu.find(v)){
                step.highlightedLine = 5;
                stepmanager.step.push_back(step);

                dsu.unite(u, v);
                sum += w;
                count++;
                {
                    step.highlightedLine = 6;
                    step.nodeHightlight = {vertex[u], vertex[v]};
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
        step.nodeHightlight.clear();
        step.edgeHighlight.clear();
        step.highlightedLine = 8;
        stepmanager.step.push_back(step);
        // trả về kết quả là sum
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

    void Graph::ADJmatrixtoEdges(){
        edges.clear();
        int n = box.ADJmatrix.size();

        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                if (box.ADJmatrix[i][j] > 0){
                    edges.push_back({i, j, box.ADJmatrix[i][j]});
                }
            }
        }
    }

    void Graph::EdgestoADJList(){
        if (edges.empty()) return;
        ADJList.clear();

        for (auto e : edges){
            int u = e[0];
            int v = e[1];
            int w = e[2];

            ADJList[u].push_back({v, w});
            ADJList[v].push_back({u, w});
        }
    }

