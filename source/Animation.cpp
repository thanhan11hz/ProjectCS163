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
            for (auto edge: anim.insertedEdge) {
                edge->alpha = 1.0f - anim.progress;
            }
            break;
        case AnimateType::MOVEMENT:
            for (auto pair: anim.movedNode) {
                pair.first->position = lerp(pair.first->position,pair.second->position,anim.progress);
            }
            break;
        default:
            break;
    }
    
}

Vector2 AnimationQueue::lerp(const Vector2& a, const Vector2& b, float progress) {
    return {a.x + (b.x - a.x) * progress, a.y + (b.y - a.y) * progress};
}