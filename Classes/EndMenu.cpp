//
// Created by pierre on 02/04/18.
//

#include "EndMenu.h"
#include "StartMenu.h"

EndMenu *EndMenu::create(bool win) {
    EndMenu *pRet = new (std::nothrow) EndMenu(win);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in EndMenu.cpp\n");
}

bool EndMenu::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    Vector<MenuItem*> menuItems;

    auto quest = Label::createWithTTF("", "fonts/Anonymous Pro.ttf", 18);
    if (quest == nullptr) {
        problemLoading("'fonts/Anonymous Pro.ttf'");
    }
    else {
        if (win)
            quest->setString("Quest complete!");
        else
            quest->setString("Game over!");
        quest->setPosition(visibleSize.width / 2, static_cast<float>(visibleSize.height / 2));
        addChild(quest);
    }
    auto okItem = MenuItemLabel::create(Label::createWithTTF("ok", "fonts/Anonymous Pro.ttf", 18),
                                        CC_CALLBACK_1(EndMenu::backToStartMenuCallBack, this));
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
                      (visibleSize.height / 2) - (quest->getBoundingBox().size.height + 10));
    return true;
}

void EndMenu::backToStartMenuCallBack(Ref *sender) {
    Director::getInstance()->replaceScene(StartMenu::createScene());
}

EndMenu::EndMenu(bool win) : win(win) {}


