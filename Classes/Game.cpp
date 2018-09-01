#include "Game.h"

USING_NS_CC;

Scene* Game::createScene()
{
    return Game::create();
}

bool Game::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    pathFinder = new PathFinder;

    tileMap = TileMap::create("map/little_world.tmx");
    addChild(tileMap, 0);

    auto s = tileMap->getContentSize();
    tileMap->setPosition(Vec2(-s.width/2, 0));
    tileMap->getLayer("meta")->setVisible(false);

    player = Player::create(gameEvents);
    player->setAnchorPoint(Vec2(0.5, 0.25));
    player->setScale(0.7);
    player->setMap(tileMap);
    player->setPosition(tileMap->getLayer("on_the_ground")->getPositionAt(Vec2(48,48)));
    tileMap->addChild(player, tileMap->getLayer("on_the_ground")->getLocalZOrder());

    auto worm = Worm::create(gameEvents);
    worm->setAnchorPoint(Vec2(0.5, 0));
    worm->setPosition(tileMap->getLayer("on_the_ground")->getPositionAt(Vec2(46,46)));
    tileMap->addChild(worm, tileMap->getLayer("on_the_ground")->getLocalZOrder());
    entities.pushBack(worm);
    auto worm2 = Worm::create(gameEvents);
    worm2->setAnchorPoint(Vec2(0.5, 0));
    worm2->setPosition(tileMap->getLayer("on_the_ground")->getPositionAt(Vec2(40,46)));
    tileMap->addChild(worm2, tileMap->getLayer("on_the_ground")->getLocalZOrder());
    entities.pushBack(worm2);
    auto worm3 = Worm::create(gameEvents);
    worm3->setAnchorPoint(Vec2(0.5, 0));
    worm3->setPosition(tileMap->getLayer("on_the_ground")->getPositionAt(Vec2(45,38)));
    tileMap->addChild(worm3, tileMap->getLayer("on_the_ground")->getLocalZOrder());
    entities.pushBack(worm3);

    auto KeyboardListener = EventListenerKeyboard::create();
    KeyboardListener->onKeyPressed = CC_CALLBACK_2(Game::onKeyPressed, this);
    KeyboardListener->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(KeyboardListener, this);
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseUp = CC_CALLBACK_1(Game::onMouseUp, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(Game::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    scheduleUpdate();
    tileMap->runAction(Follow::create(player));

/*    auto anchorPoint = DrawNode::create();
    anchorPoint->setAnchorPoint(Vec2(0.5, 0.5));
    anchorPoint->drawDot(Point(
            player->getAnchorPointInPoints().x,
            player->getAnchorPointInPoints().y),
                         2, Color4F(1.0, 0.0, 0.0, 1.0));
    player->addChild(anchorPoint);*/

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    hud = Label::createWithTTF("hp " + std::to_string(player->getHp()), "fonts/Anonymous Pro.ttf", 16);
    hud->setPosition(visibleSize.width - 100, 20);
    hud->setColor(Color3B::WHITE);
    addChild(hud);

    return true;
}

void Game::update(float delta) {
    hud->setString("hp " + std::to_string(player->getHp()));
    clearCompleteGameEvent();
    auto it = entities.begin();
    while (it != entities.end()) {
        Worm *worm =  dynamic_cast<Worm*>(*it);
        if (worm->getHp() <= 0)
            it = entities.erase(it);
        else
            it++;
    }

    if (entities.empty())
        Director::getInstance()->replaceScene(EndMenu::create(true));
    if (player->getHp() <= 0)
        Director::getInstance()->replaceScene(EndMenu::create(false));

    for (auto& e : gameEvents) {
        if (e->state == GameEvent::State::queued
            && e->entity == player
            && e->type == GameEvent::Type::attack) {
            auto targetPos = tileHit(player->getDirection(), tileMap->mapPointToTileCoordinates(player->getPosition()));
            for (auto entity : entities) {
                if (tileMap->mapPointToTileCoordinates(entity->getPosition()) == targetPos) {
                    Worm *worm =  dynamic_cast<Worm*>(entity);
                    worm->setHp(worm->getHp() - 25);
                    gameEvents.emplace_front(new GameEvent::Event{worm, GameEvent::Type::hit, GameEvent::State::queued});
                    if (worm->getHp() <= 0)
                        worm->removeFromParentAndCleanup(true);
                }
            }
            e->state = GameEvent::State::complete;
        }
        if (e->state == GameEvent::State::queued
            && e->entity != player
            && e->type == GameEvent::Type::attack) {
            Worm *worm =  dynamic_cast<Worm*>(e->entity);
            auto targetPos = tileHit(worm->getDirection(), tileMap->mapPointToTileCoordinates(worm->getPosition()));
            if (tileMap->mapPointToTileCoordinates(player->getPosition()) == targetPos) {
                gameEvents.emplace_front(new GameEvent::Event{player, GameEvent::Type::hit, GameEvent::State::queued});
                if (player->getState() != Player::State::parry)
                    player->setHp(player->getHp() - 20);
            }
            e->state = GameEvent::State::complete;
        }
    }
}

void Game::onEnter() {
    Scene::onEnter();
    Director::getInstance()->getRunningScene()->resume();
    Director::getInstance()->setProjection(Director::Projection::_2D);
    Director::getInstance()->setDepthTest(true);
}

void Game::onExit() {
    Director::getInstance()->setProjection(Director::Projection::DEFAULT);
    Director::getInstance()->setDepthTest(false);
    cocos2d::Scene::onExit();
}

Game::~Game() {
    delete pathFinder;
}

void Game::onMouseDown(cocos2d::Event *event) {
    EventMouse* e = (EventMouse*)event;
    switch (e->getMouseButton()) {
        case EventMouse::MouseButton::BUTTON_UNSET:break;
        case EventMouse::MouseButton::BUTTON_LEFT:
            player->addEvent(Player::Event::leftClick);
            break;
        case EventMouse::MouseButton::BUTTON_RIGHT:
            player->addEvent(Player::Event::rightClick);
            break;
        case EventMouse::MouseButton::BUTTON_MIDDLE:break;
        case EventMouse::MouseButton::BUTTON_4:break;
        case EventMouse::MouseButton::BUTTON_5:break;
        case EventMouse::MouseButton::BUTTON_6:break;
        case EventMouse::MouseButton::BUTTON_7:break;
        case EventMouse::MouseButton::BUTTON_8:break;
    }
}

void Game::onMouseUp(cocos2d::Event *event) {
    EventMouse* e = (EventMouse*)event;
    switch (e->getMouseButton()) {
        case EventMouse::MouseButton::BUTTON_UNSET:break;
        case EventMouse::MouseButton::BUTTON_LEFT:
            player->removeEvent(Player::Event::leftClick);
            break;
        case EventMouse::MouseButton::BUTTON_RIGHT:
            player->removeEvent(Player::Event::rightClick);
            break;
        case EventMouse::MouseButton::BUTTON_MIDDLE:break;
        case EventMouse::MouseButton::BUTTON_4:break;
        case EventMouse::MouseButton::BUTTON_5:break;
        case EventMouse::MouseButton::BUTTON_6:break;
        case EventMouse::MouseButton::BUTTON_7:break;
        case EventMouse::MouseButton::BUTTON_8:break;
    }
}

void Game::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
            player->addEvent(Player::Event::up);
            break;
        case EventKeyboard::KeyCode::KEY_S:
            player->addEvent(Player::Event::down);
            break;
        case EventKeyboard::KeyCode::KEY_A:
            player->addEvent(Player::Event::left);
            break;
        case EventKeyboard::KeyCode::KEY_D:
            player->addEvent(Player::Event::right);
            break;
        case EventKeyboard::KeyCode::KEY_Q:
            player->addEvent(Player::Event::rotateL);
            break;
        case EventKeyboard::KeyCode::KEY_E:
            player->addEvent(Player::Event::rotateR);
            break;
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            Director::getInstance()->pushScene(PauseMenu::createScene());
            break;
        default:
            break;
    }
}

void Game::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
            player->removeEvent(Player::Event::up);
            break;
        case EventKeyboard::KeyCode::KEY_S:
            player->removeEvent(Player::Event::down);
            break;
        case EventKeyboard::KeyCode::KEY_A:
            player->removeEvent(Player::Event::left);
            break;
        case EventKeyboard::KeyCode::KEY_D:
            player->removeEvent(Player::Event::right);
            break;
        case EventKeyboard::KeyCode::KEY_Q:
            player->removeEvent(Player::Event::rotateL);
            break;
        case EventKeyboard::KeyCode::KEY_E:
            player->removeEvent(Player::Event::rotateR);
            break;
        default:
            break;
    }
}

const cocos2d::Vec2 Game::tileHit(Direction dir, const cocos2d::Vec2 &pos) const {
    switch (dir) {
        case Direction::North:
            return Vec2(pos.x - 1, pos.y - 1);
        case Direction::NorthEast:
            return Vec2(pos.x, pos.y - 1);
        case Direction::East:
            return Vec2(pos.x + 1, pos.y - 1);
        case Direction::SouthEast:
            return Vec2(pos.x + 1, pos.y);
        case Direction::South:
            return Vec2(pos.x + 1, pos.y + 1);
        case Direction::SouthWest:
            return Vec2(pos.x, pos.y + 1);
        case Direction::West:
            return Vec2(pos.x - 1, pos.y + 1);
        case Direction::NorthWest:
            return Vec2(pos.x - 1, pos.y);
    }
}

void Game::clearCompleteGameEvent() {
    auto it = gameEvents.begin();
    while (it != gameEvents.end()) {
        if ((*it)->state == GameEvent::State::complete)
            it = gameEvents.erase(it);
        else
            it++;
    }
}
