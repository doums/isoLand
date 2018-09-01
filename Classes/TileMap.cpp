

#include "TileMap.h"

USING_NS_CC;

TileMap *TileMap::create(const std::string &TMXfile) {
    TileMap *ret = new (std::nothrow) TileMap();
    if (ret->initWithTMXFile(TMXfile))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

cocos2d::Vec2 TileMap::mapPointToTileCoordinates(const cocos2d::Vec2 &wPos) const {
    float mw = getMapSize().width;
    float mh = getMapSize().height;
    float tw = getTileSize().width;
    float th = getTileSize().height;
    float isox = floorf(mh - wPos.y / th + wPos.x / tw - mw / 2);
    float isoy = floorf(mh - wPos.y / th - wPos.x / tw + mw / 2 - 1 / 2);
    return Vec2(isox, isoy);
}

bool TileMap::isWalkable(const cocos2d::Vec2 &tileCoord) const {
    if (!containsPoint(tileCoord))
        return false;
    auto metaLayer = getLayer("meta");
    auto metaTile = getPropertiesForGID(metaLayer->getTileGIDAt(tileCoord));
    if (!metaTile.isNull())
        for (const auto& value : metaTile.asValueMap())
            return !(value.first == "collidable" && value.second.asString() == "true");
    return true;
}

bool TileMap::containsPoint(const cocos2d::Vec2 &tilePos) const {
    return tilePos.x >= 0 && tilePos.x < getMapSize().width
           && tilePos.y >= 0 && tilePos.y < getMapSize().height;
}

cocos2d::Vec2 TileMap::getPointPositionAt(const std::string &layerName, const cocos2d::Vec2 &tileCoord) const {
    auto layer = getLayer(layerName);
    if (layer == nullptr)
        return Vec2(0, 0);
    if (tileCoord.x >= getMapSize().width
        || tileCoord.y >= getMapSize().height
        || tileCoord.x < 0
        || tileCoord.y < 0)
        return Vec2(0, 0);
    auto tilePos = layer->getPositionAt(tileCoord);
    tilePos.x += getTileSize().width / 2;
    tilePos.y += getTileSize().height / 2;
    return tilePos;
}

bool TileMap::isWalkablePoint(const Vec2 &mapPoint) const {
    return isWalkable(mapPointToTileCoordinates(mapPoint));
}

cocos2d::Vec2 TileMap::getObjectPoint(const std::string& object) const {
    auto spawnObject = getObjectGroup("objects")->getObject(object);
    float x = spawnObject["x"].asFloat() + spawnObject["width"].asFloat();
    float y = spawnObject["y"].asFloat() + spawnObject["height"].asFloat() / 2;
    return Vec2(x, y);
}