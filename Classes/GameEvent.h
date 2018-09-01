//
// Created by pierre on 01/04/18.
//

#ifndef ISOLAND_GAMEEVENT_H
#define ISOLAND_GAMEEVENT_H

#include <cocos/2d/CCSprite.h>

namespace GameEvent {
    enum class Type {
        hit,
        attack
    };

    enum class State {
        complete,
        queued
    };

    typedef struct event_st {
        cocos2d::Sprite *entity;
        Type type;
        State state;
    } Event;
}

#endif //ISOLAND_GAMEEVENT_H
