/**
 * Path Finding using A* algorithm
 */

#ifndef ISOLAND_PATHFINDER_H
#define ISOLAND_PATHFINDER_H

#include <deque>
#include "cocos2d.h"
#include "TileMap.h"

class aStarNode {
private:
    cocos2d::Vec2       position;
    int                 gValue;       /*-- G mouvement cost from start point to that node --*/
    int                 hValue;       /*-- H estimated mouvement cost from that node to target --*/
    int                 fScore;
    aStarNode           *parent;

public:
    aStarNode(const cocos2d::Vec2 &position);
    int getGValue() const;
    int getHValue() const;
    int getFScore() const;
    const cocos2d::Vec2 &getPosition() const;
    bool isEqual(const aStarNode &node);
    void setParent(aStarNode *parent);
    aStarNode *getParent() const;
    void setGValue(int gValue);
    void setHValue(int hValue);
    void calcFScore();
};

class PathFinder {
private:
    const TileMap*                  map;
    std::deque<aStarNode *>         openList;
    std::deque<aStarNode *>         closedList;
    cocos2d::Vec2                   startPoint;
    cocos2d::Vec2                   targetPoint;
    int                             stepWidth;
    int                             stepHeight;
    int                             nDir;
    void sortByfScore();
    void addToOpenList(aStarNode *node);
    int costToMoveByOne(aStarNode *node1, aStarNode *node2);
    int calculateHScore(const cocos2d::Vec2 &fromCoord, const cocos2d::Vec2 &toCoord);
    std::vector<cocos2d::Vec2> getValidAdjacentNode(const cocos2d::Vec2 &pos);
    void clear();
    bool openListContains(const cocos2d::Vec2 &pos);
    bool closedListContains(const cocos2d::Vec2 &pos);
    aStarNode *findToOpenList(const cocos2d::Vec2 &coord);

public:
    std::vector<cocos2d::Vec2> findPath(const TileMap *tileMap,
                                         const cocos2d::Vec2 &startPoint,
                                         const cocos2d::Vec2 &targetPoint,
                                         int stepWidth,
                                         int stepHeight,
                                         int nDir);
};

#endif //ISOLAND_PATHFINDER_H