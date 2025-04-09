#include "Animation.hpp"

void AnimationQueue::addAnimation(const Animation& anim) {
    animation.push(anim);
}

bool AnimationQueue::update(float delta) {
    if (animation.empty()) return true;
    Animation &currAnim = animation.front();
    currAnim.progress += delta / currAnim.duration;
    applyAnimation(currAnim);
    if (currAnim.progress >= 1.0f) {
        currAnim.progress = 1.0f;
        applyAnimation(currAnim);
        animation.pop();
        return animation.empty();
    }
    return false;
}

bool AnimationQueue::isComplete() {
    return animation.empty();
}

void AnimationQueue::clear() {
    while (!animation.empty()) animation.pop();
}

void AnimationQueue::applyAnimation(Animation &anim) {
    switch (anim.type) {
        case AnimateType::INSERTION: 
            for (auto node: anim.insertedNode) {
                node->alpha = anim.progress;
            }
            for (auto edge: anim.insertedEdge) {
                edge->alpha = anim.progress;
            }
            break;
        case AnimateType::DELETION:
            for (auto node: anim.deletedNode) {
                node->alpha = 1.0f - anim.progress;
            }
            for (auto edge: anim.deletedEdge) {
                edge->alpha = 1.0f - anim.progress;
            }
            break;
        case AnimateType::MOVEMENT:
            for (auto pair: anim.movedNode) {
                pair.first->position = lerp(pair.first->position,pair.second->position,anim.progress);
            }
            break;
        case AnimateType::HIGHLIGHT:
            for (auto node: anim.highlightedNode) {
                node->currentColor = lerpColor(node->currentColor,node->targetColor,anim.progress);
            }
            for (auto edge: anim.highlightedEdge) {
                edge->currentColor = lerpColor(edge->currentColor,edge->targetColor,anim.progress);
            }
        default:
            break;
    }
    
}

Vector2 AnimationQueue::lerp(const Vector2& a, const Vector2& b, float progress) {
    return {a.x + (b.x - a.x) * progress, a.y + (b.y - a.y) * progress};
}

Color AnimationQueue::lerpColor(Color a, Color b, float progress) {
    return (Color){
        (unsigned char)(a.r + (b.r - a.r) * progress),
        (unsigned char)(a.g + (b.g - a.g) * progress),
        (unsigned char)(a.b + (b.b - a.b) * progress),
        (unsigned char)(a.a + (b.a - a.a) * progress)
    };
}