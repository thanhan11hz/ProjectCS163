#include "Graph.hpp"

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
    stepmanager.currentStep = 0;
    SetMousePosition(780,400);
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
    
}

void Graph::checkConnected() {
    
}

void Graph::prim() {

}

void Graph::kruskal() {

}