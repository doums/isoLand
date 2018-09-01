

#include <iostream>
#include "Player.h"

USING_NS_CC;

Player *Player::create(std::deque<std::unique_ptr<GameEvent::Event>>& e) {
    Player *pRet = new (std::nothrow) Player(e);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool Player::init() {
    direction = Direction::NorthEast;
    state = State::idle;
    currentAnim = Anim::idleNE;
    auto spriteCache = SpriteFrameCache::getInstance();
    speed = 10;
    spriteCache->addSpriteFramesWithFile("plist/hero.plist");
    switch (direction) {
        case Direction::NorthEast:
            Sprite::initWithSpriteFrameName("0157");
            break;
        case Direction::NorthWest:
            Sprite::initWithSpriteFrameName("0257");
            break;
        case Direction::SouthEast:
            Sprite::initWithSpriteFrameName("0057");
            break;
        case Direction::SouthWest:
            Sprite::initWithSpriteFrameName("0357");
            break;
        case Direction::North:
            Sprite::initWithSpriteFrameName("0207");
            break;
        case Direction::West:
            Sprite::initWithSpriteFrameName("0307");
            break;
        case Direction::East:
            Sprite::initWithSpriteFrameName("0107");
            break;
        case Direction::South:
            Sprite::initWithSpriteFrameName("0000");
            break;
    }
    createAnimation("walkN", 216, 8, 0.1, Anim::walkN, AnimGroup::walk, Direction::North);
    createAnimation("walkS", 9, 8, 0.1, Anim::walkS, AnimGroup::walk, Direction::South);
    createAnimation("walkE", 116, 8, 0.1, Anim::walkE, AnimGroup::walk, Direction::East);
    createAnimation("walkW", 316, 8, 0.1, Anim::walkW, AnimGroup::walk, Direction::West);
    createAnimation("walkNE", 166, 8, 0.1, Anim::walkNE, AnimGroup::walk, Direction::NorthEast);
    createAnimation("walkNW", 266, 8, 0.1, Anim::walkNW, AnimGroup::walk, Direction::NorthWest);
    createAnimation("walkSE", 66, 8, 0.1, Anim::walkSE, AnimGroup::walk, Direction::SouthEast);
    createAnimation("walkSW", 366, 8, 0.1, Anim::walkSW, AnimGroup::walk, Direction::SouthWest);
    createAnimation("idleN", 207, 9, 0.1, Anim::idleN, AnimGroup::idle, Direction::North);
    createAnimation("idleNE", 157, 9, 0.1, Anim::idleNE, AnimGroup::idle, Direction::NorthEast);
    createAnimation("idleE", 107, 9, 0.1, Anim::idleE, AnimGroup::idle, Direction::East);
    createAnimation("idleSE", 57, 9, 0.1, Anim::idleSE, AnimGroup::idle, Direction::SouthEast);
    createAnimation("idleS", 0, 9, 0.1, Anim::idleS, AnimGroup::idle, Direction::South);
    createAnimation("idleSW", 357, 9, 0.1, Anim::idleSW, AnimGroup::idle, Direction::SouthWest);
    createAnimation("idleW", 307, 9, 0.1, Anim::idleW, AnimGroup::idle, Direction::West);
    createAnimation("idleNW", 257, 9, 0.1, Anim::idleNW, AnimGroup::idle, Direction::NorthWest);
    createAnimation("blowN", 245, 8, 0.06, Anim::blowN, AnimGroup::attack, Direction::North);
    createAnimation("blowNE", 195, 8, 0.06, Anim::blowNE, AnimGroup::attack, Direction::NorthEast);
    createAnimation("blowE", 145, 8, 0.06, Anim::blowE, AnimGroup::attack, Direction::East);
    createAnimation("blowSE", 95, 8, 0.06, Anim::blowSE, AnimGroup::attack, Direction::SouthEast);
    createAnimation("blowS", 45, 8, 0.06, Anim::blowS, AnimGroup::attack, Direction::South);
    createAnimation("blowSW", 395, 8, 0.06, Anim::blowSW, AnimGroup::attack, Direction::SouthWest);
    createAnimation("blowW", 345, 8, 0.06, Anim::blowW, AnimGroup::attack, Direction::West);
    createAnimation("blowNW", 295, 8, 0.06, Anim::blowNW, AnimGroup::attack, Direction::NorthWest);
    createAnimation("parryN", 253, 4, 0.1, Anim::parryN, AnimGroup::parry, Direction::North);
    createAnimation("parryNE", 203, 4, 0.1, Anim::parryNE, AnimGroup::parry, Direction::NorthEast);
    createAnimation("parryE", 153, 4, 0.1, Anim::parryE, AnimGroup::parry, Direction::East);
    createAnimation("parrySE", 103, 4, 0.1, Anim::parrySE, AnimGroup::parry, Direction::SouthEast);
    createAnimation("parryS", 53, 4, 0.1, Anim::parryS, AnimGroup::parry, Direction::South);
    createAnimation("parrySW", 403, 4, 0.1, Anim::parrySW, AnimGroup::parry, Direction::SouthWest);
    createAnimation("parryW", 353, 4, 0.1, Anim::parryW, AnimGroup::parry, Direction::West);
    createAnimation("parryNW", 303, 4, 0.1, Anim::parryNW, AnimGroup::parry, Direction::NorthWest);

    scheduleUpdate();
    schedule(CC_SCHEDULE_SELECTOR(Player::updateVertexZ));

    return true;
}

void Player::createAnimation(const std::string &name, int frameId, int frameNumber, float delay,
                             Player::Anim animId, Player::AnimGroup group, Direction dir) {
    auto anim = Animation::createWithSpriteFrames(getAnimation("%04d", frameId, frameNumber), delay);
    anim->retain();
    animations.push_back({name, anim, animId, group, dir});
}

void Player::setDirection(Direction direction) {
    Player::direction = direction;
}

Vector<SpriteFrame*> Player::getAnimation(const char *format, int frameStart, int count) {
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

void Player::addEvent(Player::Event event) {
    auto it = std::find(events.begin(), events.end(), event);
    if (it == events.end())
        events.push_back(event);
}

void Player::removeEvent(Player::Event event) {
    auto it = std::find(events.begin(), events.end(), event);
    if (it != events.end())
        events.erase(it);
}

void Player::update(float delta) {
    if (!attackSend && attackDuration > 0 && attackDuration < 0.24) {
        gameEvents.emplace_front(new GameEvent::Event{this, GameEvent::Type::attack, GameEvent::State::queued});
        attackSend = true;
    }
    if (attackDuration > 0) {
        attackDuration -= delta;
        return;
    }
    if (attackDuration <= 0)
        attackSend = false;
    if (events.empty())
        state = State::idle;
    if (findEvent(Event::rotateR) || findEvent(Event::rotateL))
        state = State::rotate;
    if (countDirectionalEvents() > 0)
        state = State::walk;
    if (findEvent(Event::leftClick))
        state = State::attack;
    if (findEvent(Event::rightClick))
        state = State::parry;

    switch (state) {
        case State::idle:
            manageIdle();
            break;
        case State::walk:
            setWalkAnim();
            move(delta);
            break;
        case State::rotate:
            manageRotate();
            break;
        case State::attack:
            manageAttack();
            break;
        case State::parry:
            manageParry();
            break;
    }

    for (auto& event: gameEvents) {
        if (event->entity == this
            && event->type == GameEvent::Type::hit
            && event->state == GameEvent::State::queued) {
            if (state != State::parry) {
                auto color = getColor();
                auto tintBy = TintBy::create(0.3, 120.0f, 232.0f, 254.0f);
                auto colorBack = CallFunc::create([&color, this]() {
                    setColor(Color3B::WHITE);
                });
                auto seq = Sequence::create(tintBy, colorBack, nullptr);
                runAction(seq);
            }
            event->state = GameEvent::State::complete;
        }
    }
}

void Player::setWalkAnim() {
    if (countDirectionalEvents() == 1) {
        for (auto event : events)
            switch (event) {
                case Event::up:
                    newWalkAnim(Anim::walkNE);
                    direction = Direction::NorthEast;
                    break;
                case Event::down:
                    newWalkAnim(Anim::walkSW);
                    direction = Direction::SouthWest;
                    break;
                case Event::left:
                    newWalkAnim(Anim::walkNW);
                    direction = Direction::NorthWest;
                    break;
                case Event::right:
                    newWalkAnim(Anim::walkSE);
                    direction = Direction::SouthEast;
                    break;
                default:
                    break;
            }
    }
    if (countDirectionalEvents() == 2) {
        if (findEvent(Event::up) && findEvent(Event::left)) {
            newWalkAnim(Anim::walkN);
            direction = Direction::North;
        }
        if (findEvent(Event::up) && findEvent(Event::right)) {
            newWalkAnim(Anim::walkE);
            direction = Direction::East;
        }
        if (findEvent(Event::down) && findEvent(Event::left)) {
            newWalkAnim(Anim::walkW);
            direction = Direction::West;
        }
        if (findEvent(Event::down) && findEvent(Event::right)) {
            newWalkAnim(Anim::walkS);
            direction = Direction::South;
        }
    }
}

void Player::setMap(TileMap *map) {
    Player::map = map;
    stepX = map->getTileSize().width;
    stepY = map->getTileSize().height;
}

void Player::updateVertexZ(float dt) {
    auto p = getPosition();
    p = CC_POINT_POINTS_TO_PIXELS(p);
    float newZ = -(p.y + 32) / 16 + 1;
    setPositionZ(newZ);
}

bool Player::findEvent(Player::Event event) {
    return std::find(events.begin(), events.end(), event) != events.end();
}

int Player::countDirectionalEvents() {
    auto i = 0;
    for (auto event : events)
        if (event == Event::up || event == Event::right || event == Event::left || event == Event::down)
            i++;
    return i;
}

void Player::newWalkAnim(Player::Anim animId) {
    auto anim = std::find_if(animations.begin(), animations.end(),[this, animId](animData it) {
        return it.tag == animId;
    });
    if (anim != animations.end() && currentAnim != animId) {
        stopAllActions();
        auto idleAnim = RepeatForever::create(Animate::create(anim->anim));
        idleAnim->setTag(animId);
        idleAnim->setFlags(AnimGroup::walk);
        runAction(idleAnim);
        currentAnim = animId;
    }
}

void Player::manageIdle() {
    auto anim = std::find_if(animations.begin(), animations.end(),[this](animData it) {
        return direction == it.direction && it.flag == AnimGroup::idle;
    });
    if (anim != animations.end() && currentAnim != anim->tag) {
        stopAllActions();
        auto idleAnim = RepeatForever::create(Animate::create(anim->anim));
        idleAnim->setTag(anim->tag);
        idleAnim->setFlags(AnimGroup::idle);
        runAction(idleAnim);
        currentAnim = anim->tag;
    }
}

void Player::manageRotate() {
    if (findEvent(Event::rotateR)) {
        direction++;
        removeEvent(Event::rotateR);
    }
    if (findEvent(Event::rotateL)) {
        direction--;
        removeEvent(Event::rotateL);
    }
}

void Player::move(float delta) {
    bool incX = false;
    bool incY = false;
    bool decX = false;
    bool decY = false;

    for (auto event : events) {
        switch (event) {
            case Event::up:
                if (!incY)
                    incY = true;
                if (!incX)
                    incX = true;
                break;
            case Event::down:
                if (!decY)
                    decY = true;
                if (!decX)
                    decX = true;
                break;
            case Event::left:
                if (!decX)
                    decX = true;
                if (!incY)
                    incY = true;
                break;
            case Event::right:
                if (!incX)
                    incX = true;
                if (!decY)
                    decY = true;
                break;
            default: break;
        }
    }
    if (incX || incY || decX || decY) {
        auto newPos = getPosition();
        if (incX)
            newPos.x += stepX * delta;
        if (incY)
            newPos.y += stepY * delta;
        if (decX)
            newPos.x -= stepX * delta;
        if (decY)
            newPos.y -= stepY * delta;
        if (map->isWalkablePoint(newPos))
            setPosition(newPos);
        /*     else {
                 newPos = getPosition();
                 if (incX && incY && !decX && decY) {
                     newPos.x += (stepX / 2) * delta;
                     newPos.y += (stepY / 2) * delta;
                     if (!map->isWalkablePoint(newPos)) {
                         newPos = getPosition();
                         newPos.x += (stepX / 2) * delta;
                         newPos.y -= (stepY / 2) * delta;
                     }
                 }
                 else if (incX && !incY && decX && decY) {
                     newPos.x -= (stepX / 2) * delta;
                     newPos.y -= (stepY / 2) * delta;
                     if (!map->isWalkablePoint(newPos)) {
                         newPos = getPosition();
                         newPos.x += (stepX / 2) * delta;
                         newPos.y -= (stepY / 2) * delta;
                     }
                 }
                 else if (!incX && incY && decX && decY) {
                     newPos.x -= (stepX / 2) * delta;
                     newPos.y += (stepY / 2) * delta;
                     if (!map->isWalkablePoint(newPos)) {
                         newPos = getPosition();
                         newPos.x -= (stepX / 2) * delta;
                         newPos.y -= (stepY / 2) * delta;
                     }
                 }
                 else if (incX && incY && decX && !decY) {
                     newPos.x += (stepX / 2) * delta;
                     newPos.y += (stepY / 2) * delta;
                     if (!map->isWalkablePoint(newPos)) {
                         newPos = getPosition();
                         newPos.x -= (stepX / 2) * delta;
                         newPos.y += (stepY / 2) * delta;
                     }
                 }
                 if (map->isWalkablePoint(newPos))
                     setPosition(newPos);*/
    }
}

Player::animData* Player::findAnimDataByTag(Anim tag) {
    auto result = std::find_if(animations.begin(), animations.end(),[tag](animData it) { return tag == it.tag; });
    return result != animations.end() ? &(*result) : nullptr;
}

void Player::manageAttack() {
    auto anim = std::find_if(animations.begin(), animations.end(),[this](animData it) {
        return direction == it.direction && it.flag == AnimGroup::attack;
    });
    if (anim != animations.end() && currentAnim != anim->tag) {
        stopAllActions();
        auto attackAnim = Animate::create(anim->anim);
        attackAnim->setTag(anim->tag);
        attackAnim->setFlags(AnimGroup::attack);
        runAction(attackAnim);
        currentAnim = anim->tag;
        removeEvent(Event::leftClick);
        attackDuration = 0.48f;
    }
}

void Player::manageParry() {
    auto anim = std::find_if(animations.begin(), animations.end(),[this](animData it) {
        return direction == it.direction && it.flag == AnimGroup::parry;
    });
    if (anim != animations.end() && currentAnim != anim->tag) {
        stopAllActions();
        auto parryAnim = Animate::create(anim->anim);
        parryAnim->setTag(anim->tag);
        parryAnim->setFlags(AnimGroup::parry);
        runAction(parryAnim);
        currentAnim = anim->tag;
    }
}

Direction Player::getDirection() const {
    return direction;
}

Player::Player(std::deque<std::unique_ptr<GameEvent::Event>> &gameEvents) : gameEvents(gameEvents) {

}

int Player::getHp() const {
    return hp;
}

void Player::setHp(int hp) {
    Player::hp = hp;
}

Player::State Player::getState() const {
    return state;
}
