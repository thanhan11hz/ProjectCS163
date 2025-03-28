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
    
}

void Graph::init() {
    initView();
}

void Graph::draw() {
    drawView();
}

void Graph::run() {
    eventView();
}

void Graph::exit() {
    
}

void Graph::initData() {

}

void Graph::insertData() {

}

void Graph::deleteData() {

}

void Graph::searchData() {
    
}