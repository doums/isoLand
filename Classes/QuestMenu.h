//
// Created by pierre on 02/04/18.
//

#ifndef ISOLAND_QUESTMENU_H
#define ISOLAND_QUESTMENU_H

#include "cocos2d.h"

USING_NS_CC;

class QuestMenu : public cocos2d::Scene {
private:
    void acceptCallBack(Ref* sender);

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(QuestMenu);
};


#endif //ISOLAND_QUESTMENU_H
