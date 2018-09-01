

#ifndef ISOLAND_TILEMAP_H
#define ISOLAND_TILEMAP_H

#include "cocos2d.h"

class TileMap : public cocos2d::TMXTiledMap {
public:
    static TileMap* create(const std::string &fileName);
    cocos2d::Vec2 mapPointToTileCoordinates(const cocos2d::Vec2 &mapPoint) const;
    bool isWalkable(const cocos2d::Vec2 &tileCoord) const;
    bool isWalkablePoint(const cocos2d::Vec2 &mapPoint) const;
    bool containsPoint(const cocos2d::Vec2 &tileCoord) const;
    cocos2d::Vec2 getPointPositionAt(const std::string &layerName, const cocos2d::Vec2 &tileCoord) const;
    cocos2d::Vec2 getObjectPoint(const std::string& object) const;
};

#endif //ISOLAND_TILEMAP_H
