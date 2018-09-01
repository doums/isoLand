//
// Created by pierre on 18/01/18.
//

#include "PauseMenu.h"

USING_NS_CC;

cocos2d::Scene *PauseMenu::createScene() {
    return PauseMenu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in PauseMenu.cpp\n");
}

bool PauseMenu::init() {
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    Vector<MenuItem*> menuItems;

    auto gameTitle = Label::createWithTTF("isoLand", "fonts/Anonymous Pro.ttf", 24);
    if (gameTitle == nullptr)
    {
        problemLoading("'fonts/Anonymous Pro.ttf'");
    }
    else {
        gameTitle->setPosition(visibleSize.width / 2,
                               static_cast<float>(visibleSize.height / 1.2));
        addChild(gameTitle);
    }
    auto playItem = MenuItemLabel::create(Label::createWithTTF("resume", "fonts/Anonymous Pro.ttf", 18),
                                          CC_CALLBACK_1(PauseMenu::resumeCallBack, this));
    if (playItem == nullptr ||
        playItem->getContentSize().width <= 0 ||
        playItem->getContentSize().height <= 0) {
        problemLoading("'fonts/Anonymous Pro.ttf'");
    }
    else {
        playItem->setPosition(playItem->getPosition().x,
                              playItem->getPosition().y + 50);
        menuItems.pushBack(playItem);
    }

    auto quitItem = MenuItemLabel::create(Label::createWithTTF("quit", "fonts/Anonymous Pro.ttf", 18),
                                          CC_CALLBACK_1(PauseMenu::quitCallBack, this));
    if (quitItem == nullptr ||
        quitItem->getContentSize().width <= 0 ||
        quitItem->getContentSize().height <= 0) {
        problemLoading("'fonts/Anonymous Pro.ttf'");
    }
    else
        menuItems.pushBack(quitItem);

    auto menu = Menu::createWithArray(menuItems);
    addChild(menu);

    return true;
}

void PauseMenu::resumeCallBack(Ref *sender) {
    log("game resume");
    Director::getInstance()->popScene();
}

void PauseMenu::quitCallBack(Ref *sender) {
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
