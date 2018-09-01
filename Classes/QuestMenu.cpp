//
// Created by pierre on 02/04/18.
//

#include "QuestMenu.h"
#include "Game.h"

Scene *QuestMenu::createScene() {
    return QuestMenu::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in QuestMenu.cpp\n");
}

bool QuestMenu::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    Vector<MenuItem*> menuItems;

    auto quest = Label::createWithTTF("Welcome to your epic and chivalrous journey!\nHere is your quest.\nYou are a valiant knight. And like any knight you kill monsters.\nDefend your land and kill 3 worms !", "fonts/Anonymous Pro.ttf", 18);
    if (quest == nullptr) {
        problemLoading("'fonts/Anonymous Pro.ttf'");
    }
    else {
        quest->setPosition(visibleSize.width / 2, static_cast<float>(visibleSize.height / 2));
        addChild(quest);
    }
    auto okItem = MenuItemLabel::create(Label::createWithTTF("ok", "fonts/Anonymous Pro.ttf", 18),
                                          CC_CALLBACK_1(QuestMenu::acceptCallBack, this));
    if (okItem == nullptr ||
        okItem->getContentSize().width <= 0 ||
        okItem->getContentSize().height <= 0) {
        problemLoading("'fonts/Anonymous Pro.ttf'");
    }
    else
        menuItems.pushBack(okItem);

    auto menu = Menu::createWithArray(menuItems);
    addChild(menu);
    menu->setPosition(visibleSize.width / 2,
                        (visibleSize.height / 2) - quest->getBoundingBox().size.height);
    return true;
}

void QuestMenu::acceptCallBack(Ref *sender) {
    Director::getInstance()->replaceScene(Game::createScene());
}
