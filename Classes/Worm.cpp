//
// Created by pierre on 30/03/18.
//

#include <iostream>
#include "Worm.h"

USING_NS_CC;

Worm *Worm::create(std::deque<std::unique_ptr<GameEvent::Event>> &gameEvents) {
    Worm *pRet = new (std::nothrow) Worm(gameEvents);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool Worm::init() {
    direction = Direction::East;
    state = State::buryOut;
    currentAnim = Anim::buryOut;
    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("plist/worm.plist");
    Sprite::initWithSpriteFrameName("worm_0000");

    createAnimation("buryOut", 0, 8, 0.1, Anim::buryOut);
    createAnimation("bite", 8, 6, 0.1, Anim::bite);
    createAnimation("buryBack", 14, 8, 0.1, Anim::buryBack);

    scheduleUpdate();
    schedule(CC_SCHEDULE_SELECTOR(Worm::updateVertexZ));

    return true;
}

void Worm::createAnimation(const std::string &name, int frameId, int frameNumber, float delay, Worm::Anim animId) {
    auto anim = Animation::createWithSpriteFrames(getAnimation("worm_%04d", frameId, frameNumber), delay);
    anim->retain();
    animations.push_back({name, anim, animId});
}

cocos2d::Vector<cocos2d::SpriteFrame *> Worm::getAnimation(const char *format, int frameStart, int count) {
    auto spriteCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[100] = {0};
    for (int i = 1; i <= count; i++) {
        sprintf(str, format, frameStart);
        animFrames.pushBack(spriteCache->getSpriteFrameByName(str));
        frameStart++;
    }
    return animFrames;
}

void Worm::update(float delta) {
    if (biteDuration > 0) {
        biteDuration -= delta;
        return;
    }
    if (biteDuration > 0 && biteDuration < 4) {
        //todo hit logic once
    }

    if (getNumberOfRunningActions() == 0) {
        switch (state) {
            case State::bite:
                state = State::buryBack;
                runAnimation(Anim::buryBack);
                break;
            case State::buryOut:
                state = State::bite;
                pushAttackEvent();
                runAnimation(Anim::bite);
                break;
            case State::buryBack:
                state = State::buryOut;
                runAnimation(Anim::buryOut);
                break;
        }
    }

    for (auto& event: gameEvents) {
        if (event->entity == this
            && event->type == GameEvent::Type::hit
            && event->state == GameEvent::State::queued) {
            auto color = getColor();
            auto tintBy = TintBy::create(0.3, 120.0f, 232.0f, 254.0f);
            auto colorBack = CallFunc::create([&color, this]() {
                setColor(Color3B::WHITE);
            });
            auto seq = Sequence::create(tintBy, colorBack, nullptr);
            runAction(seq);
            event->state = GameEvent::State::complete;
        }
    }
}

void Worm::updateVertexZ(float dt) {
    auto p = getPosition();
    p = CC_POINT_POINTS_TO_PIXELS(p);
    float newZ = -(p.y + 32) / 16 + 1;
    setPositionZ(newZ);
}

void Worm::runAnimation(Anim animId) {
    auto anim = std::find_if(animations.begin(), animations.end(), [this, animId](animData it) {
        return it.tag == animId;
    });
    if (anim != animations.end() && currentAnim != animId) {
        stopAllActions();
        auto idleAnim = Animate::create(anim->anim);
        idleAnim->setTag(animId);
        runAction(idleAnim);
        currentAnim = animId;
    }
}

int Worm::getHp() const {
    return hp;
}

void Worm::setHp(int hp) {
    Worm::hp = hp;
}

Worm::Worm(std::deque<std::unique_ptr<GameEvent::Event>> &gameEvents) : gameEvents(gameEvents) {}

void Worm::pushAttackEvent() {
    auto result = std::find_if(gameEvents.begin(), gameEvents.end(), [this](std::unique_ptr<GameEvent::Event>& it) {
        return it->entity == this
               && it->state == GameEvent::State::queued
               && it->type == GameEvent::Type::attack;
    });
    if (result == gameEvents.end()) {
        gameEvents.emplace_front(new GameEvent::Event{this, GameEvent::Type::attack, GameEvent::State::queued});
    }
}

Direction Worm::getDirection() const {
    return direction;
}
