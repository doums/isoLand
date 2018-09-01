

#ifndef ISOLAND_PLAYER_H
#define ISOLAND_PLAYER_H

#include <vector>

#include "cocos2d.h"
#include "Direction.h"
#include "TileMap.h"
#include "GameEvent.h"

class Player : public cocos2d::Sprite {
public:
    enum class Event {
        up,
        down,
        left,
        right,
        rotateR,
        rotateL,
        leftClick,
        rightClick
    };

    enum Anim {
        walkNE = 0,
        walkNW,
        walkSE,
        walkSW,
        walkN,
        walkE,
        walkS,
        walkW,
        idleN,
        idleNE,
        idleE,
        idleSE,
        idleS,
        idleSW,
        idleW,
        idleNW,
        blowN,
        blowNE,
        blowE,
        blowSE,
        blowS,
        blowSW,
        blowW,
        blowNW,
        parryN,
        parryNE,
        parryE,
        parrySE,
        parryS,
        parrySW,
        parryW,
        parryNW
    };

    enum AnimGroup {
        walk = 1,
        idle,
        attack,
        parry
    };

    enum class State {
        idle,
        walk,
        rotate,
        attack,
        parry
    };

    typedef struct animation {
        std::string name;
        cocos2d::Animation* anim;
        Anim tag;
        AnimGroup flag;
        Direction direction;
    } animData;

private:
    std::vector<animData> animations;
    std::deque<std::unique_ptr<GameEvent::Event>>& gameEvents;
    Direction direction;
    Anim currentAnim;
    int hp = 100;
    float attackDuration = 0;
    bool attackSend = false;
    cocos2d::Vector<cocos2d::SpriteFrame*>
            getAnimation(const char *format, int frameStart, int count);
    void createAnimation(const std::string& name, int frameId, int frameNumber,
                         float delay, Anim, AnimGroup, Direction);
    std::vector<Event> events;
    float stepX;
    float stepY;
    float speed;
    TileMap* map;
    State state;
    void update(float delta) override;
    bool init() override;
    void newWalkAnim(Anim);
    void updateVertexZ(float dt);
    bool findEvent(Event);
    void setWalkAnim();
    int countDirectionalEvents();
    void move(float delta);
    void manageIdle();
    void manageRotate();
    void manageAttack();
    void manageParry();
    animData *findAnimDataByTag(Anim);

public:
    Player(std::deque<std::unique_ptr<GameEvent::Event>> &gameEvents);
    static Player* create(std::deque<std::unique_ptr<GameEvent::Event>>& e);
    void setDirection(Direction direction);
    void addEvent(Event);
    void removeEvent(Event);
    void setMap(TileMap *map);
    Direction getDirection() const;
    int getHp() const;
    void setHp(int hp);
    State getState() const;
};


#endif //ISOLAND_PLAYER_H
