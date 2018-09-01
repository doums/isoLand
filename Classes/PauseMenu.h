//
// Created by pierre on 18/01/18.
//

#ifndef ISOLAND_PAUSEMENU_H
#define ISOLAND_PAUSEMENU_H

#include "cocos2d.h"

USING_NS_CC;

class PauseMenu : public cocos2d::Scene {
private:
    void resumeCallBack(Ref* sender);
    void quitCallBack(Ref* sender);

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(PauseMenu);
};

#endif //ISOLAND_PAUSEMENU_H
