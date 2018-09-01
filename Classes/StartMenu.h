//
// Created by pierre on 02/01/18.
//

#ifndef ISOLAND_STARTMENU_H
#define ISOLAND_STARTMENU_H

#include "cocos2d.h"
#include "Game.h"
#include "StartMenu.h"

class StartMenu : public cocos2d::Scene {
private:
    void playCallBack(Ref* sender);
    void quitCallBack(Ref* sender);

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(StartMenu);
};

#endif //ISOLAND_STARTMENU_H
