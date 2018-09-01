//
// Created by pierre on 30/03/18.
//

#ifndef ISOLAND_WORM_H
#define ISOLAND_WORM_H

#include <cocos/2d/CCSprite.h>
#include "Direction.h"
#include "TileMap.h"
#include "GameEvent.h"

class Worm : public cocos2d::Sprite {
public:
    enum class State {
        bite,
        buryOut,
        buryBack
    };

    enum Anim {
        buryOut,
        buryBack,
        bite
    };

    typedef struct animation {
        std::string name;
        cocos2d::Animation* anim;
        Anim tag;
    } animData;

private:
    std::vector<animData> animations;
    std::deque<std::unique_ptr<GameEvent::Event>>& gameEvents;
    Direction direction;
    State state;
    Anim currentAnim;
    float biteDuration = 0;
    int hp = 100;
    cocos2d::Vector<cocos2d::SpriteFrame*>
    getAnimation(const char *format, int frameStart, int count);
    void createAnimation(const std::string& name, int frameId, int frameNumber,
                         float delay, Anim);
    void update(float delta) override;
    bool init() override;
    void updateVertexZ(float dt);
    void runAnimation(Anim anim);
    void pushAttackEvent();

public:
    Worm(std::deque<std::unique_ptr<GameEvent::Event>> &gameEvents);
    static Worm* create(std::deque<std::unique_ptr<GameEvent::Event>> &gameEvents);
    int getHp() const;
    void setHp(int hp);
    Direction getDirection() const;
};

#endif //ISOLAND_WORM_H
