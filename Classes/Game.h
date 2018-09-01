#ifndef __ISOLAND_SCENE_H__
#define __ISOLAND_SCENE_H__

#include <vector>
#include "cocos2d.h"
#include "TileMap.h"
#include "Player.h"
#include "PathFinder.h"
#include "Worm.h"
#include "PauseMenu.h"
#include "EndMenu.h"
#include "GameEvent.h"

class Game : public cocos2d::Scene
{
private:
    TileMap* tileMap;
    PathFinder* pathFinder;
    Player* player;
    cocos2d::Vector<cocos2d::Node*> entities;
    std::deque<std::unique_ptr<GameEvent::Event>> gameEvents;
    Label* hud;
    void onMouseDown(cocos2d::Event *event);
    void onMouseUp(cocos2d::Event *event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    const cocos2d::Vec2 tileHit(Direction dir, const cocos2d::Vec2& pos) const;
    void clearCompleteGameEvent();

public:
    static cocos2d::Scene* createScene();
    bool init() override;
    CREATE_FUNC(Game);
    void update(float delta) override;
    void onEnter() override;
    void onExit() override;
    ~Game() override;
};

#endif // __ISOLAND_SCENE_H__
