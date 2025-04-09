#ifndef Animation_hpp
#define Animation_hpp

#include <iostream>
#include <vector>
#include <queue>
#include "Node.hpp"
#include "Edge.hpp"

class Animation {
    public:
        AnimateType type;
        std::vector<Node*> deletedNode;
        std::vector<Edge*> deletedEdge;
        std::vector<Node*> insertedNode;
        std::vector<Edge*> insertedEdge;
        std::vector<Node*> highlightedNode;
        std::vector<Edge*> highlightedEdge;
        std::vector<std::pair<Node*,Node*>> movedNode;
        float progress = 0.0f;
        float duration = 0.5f;
        Animation() {}
};

class AnimationQueue {
    public:
        std::queue<Animation> animation;
        void addAnimation(const Animation& anim);
        bool update(float delta);
        bool isComplete();
        void clear();
        void applyAnimation(Animation &anim);
        Vector2 lerp(const Vector2& a, const Vector2& b, float progress);
        Color lerpColor(Color a, Color b, float progress);
};

#endif