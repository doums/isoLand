//
// Created by pierre on 02/04/18.
//

#ifndef ISOLAND_ENDMENU_H
#define ISOLAND_ENDMENU_H

#include "cocos2d.h"

USING_NS_CC;

class EndMenu : public Scene {
private:
    void backToStartMenuCallBack(Ref* sender);
    bool win;

public:
    EndMenu(bool win);
    static EndMenu* create(bool win);
    virtual bool init();
};


#endif //ISOLAND_ENDMENU_H
