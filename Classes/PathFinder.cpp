

#include "PathFinder.h"

USING_NS_CC;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///     aStarNode class                                                                                              ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


aStarNode::aStarNode(const cocos2d::Vec2 &position) :
        position(position),
        gValue(0),
        hValue(0),
        parent(nullptr)
{}

int aStarNode::getGValue() const {
    return gValue;
}

int aStarNode::getHValue() const {
    return hValue;
}

int aStarNode::getFScore() const {
    return fScore;
}

void aStarNode::calcFScore() {
    fScore = gValue + hValue;
}

bool aStarNode::isEqual(const aStarNode &node) {
    return node.getPosition() == position;
}

const cocos2d::Vec2 &aStarNode::getPosition() const {
    return position;
}

void aStarNode::setParent(aStarNode *parent) {
    aStarNode::parent = parent;
}

aStarNode *aStarNode::getParent() const {
    return parent;
}

void aStarNode::setGValue(int gValue) {
    aStarNode::gValue = gValue;
}

void aStarNode::setHValue(int hValue) {
    aStarNode::hValue = hValue;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///     PathFinder class                                                                                             ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void PathFinder::sortByfScore() {
    std::sort(openList.begin(), openList.end(),
              [] (aStarNode *a, aStarNode *b) -> bool {
                  return a->getFScore() < b->getFScore();
              });
}

void PathFinder::addToOpenList(aStarNode *node) {
    openList.push_back(node);
    sortByfScore();
}

int PathFinder::costToMoveByOne(aStarNode *node1, aStarNode *node2) {
    if (nDir == 8
        && node1->getPosition().x != node2->getPosition().x
        && node1->getPosition().y != node2->getPosition().y)
        return 14;
    return 10;
}

int PathFinder::calculateHScore(const cocos2d::Vec2 &fromCoord, const cocos2d::Vec2 &toCoord) {
    return static_cast<int>(std::abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y));
}

std::vector<cocos2d::Vec2> PathFinder::getValidAdjacentNode(const cocos2d::Vec2 &pos) {
    std::vector<cocos2d::Vec2>      adjNodes;

    bool top = false;
    bool bottom = false;
    bool right = false;
    bool left = false;
    auto p = Vec2(pos.x, pos.y - 1);
    if (map->isWalkable(p)) {
        adjNodes.push_back(p);
        top = true;
    }
    p = Vec2(pos.x, pos.y + 1);
    if (map->isWalkable(p)) {
        adjNodes.push_back(p);
        bottom = true;
    }
    p = Vec2(pos.x - 1, pos.y);
    if (map->isWalkable(p)) {
        adjNodes.push_back(p);
        right = true;
    }
    p = Vec2(pos.x + 1, pos.y);
    if (map->isWalkable(p)) {
        adjNodes.push_back(p);
        left = true;
    }

    if (nDir == 8) {
        p = Vec2(pos.x - 1, pos.y - 1);
        if (map->isWalkable(p) && top && right)
            adjNodes.push_back(p);
        p = Vec2(pos.x + 1, pos.y + 1);
        if (map->isWalkable(p) && bottom && left)
            adjNodes.push_back(p);
        p = Vec2(pos.x - 1, pos.y + 1);
        if (map->isWalkable(p) && bottom && right)
            adjNodes.push_back(p);
        p = Vec2(pos.x + 1, pos.y - 1);
        if (map->isWalkable(p) && top && left)
            adjNodes.push_back(p);
    }

    return adjNodes;
}

std::vector<cocos2d::Vec2>
PathFinder::findPath(const TileMap *tileMap, const cocos2d::Vec2 &start, const cocos2d::Vec2 &target,
                     int stepX, int stepY, int n) {
    clear();
    map = tileMap;
    startPoint = start;
    targetPoint = target;
    stepWidth = stepX;
    stepHeight = stepY;
    nDir = n;
    bool pathFind = false;
    std::vector<Vec2>   path;

    addToOpenList(new aStarNode(startPoint));
    while (!openList.empty()) {
        auto currentNode = openList.front();
        closedList.push_back(currentNode);
        openList.pop_front();
        if (currentNode->getPosition() == targetPoint) {
            pathFind = true;
            auto pathNode = currentNode;
            while (pathNode->getParent() != nullptr) {
                path.push_back(pathNode->getPosition());
                pathNode = pathNode->getParent();
            }
            log("pathFinder: Ok path found !");
            break;
        }
        auto adjNodes = getValidAdjacentNode(currentNode->getPosition());
        for (auto coord : adjNodes) {
            auto adjNode = new aStarNode(coord);
            if (closedListContains(coord)) {
                delete adjNode;
                continue;
            }
            int moveCost = costToMoveByOne(currentNode, adjNode);
            if (!openListContains(coord)) {
                adjNode->setParent(currentNode);
                adjNode->setGValue(currentNode->getGValue() + moveCost);
                adjNode->setHValue(calculateHScore(adjNode->getPosition(), targetPoint));
                adjNode->calcFScore();
                addToOpenList(adjNode);
            }
            else {
                delete adjNode;
                auto node = findToOpenList(coord);
                if (currentNode->getGValue() + moveCost < node->getGValue()) {
                    node->setGValue(currentNode->getGValue() + moveCost);
                    node->calcFScore();
                    node->setParent(currentNode);
                    sortByfScore();
                }
            }
        }
    }
    if (!pathFind) {
        log("no path found");
        path.clear();
        return path;
    }
    path.push_back((new aStarNode(startPoint))->getPosition());
    return path;
}

void PathFinder::clear() {
    closedList.clear();
    openList.clear();
    map = nullptr;
}

bool PathFinder::openListContains(const cocos2d::Vec2 &pos) {
    if (openList.empty())
        return false;
    auto it = std::find_if(openList.begin(), openList.end(),
                           [&pos] (aStarNode *node) {
                               return pos == node->getPosition();
                           });
    return it != openList.end();
}

bool PathFinder::closedListContains(const cocos2d::Vec2 &pos) {
    if (closedList.empty())
        return false;
    auto it = std::find_if(closedList.begin(), closedList.end(),
                           [&pos] (aStarNode *node) {
                               return pos == node->getPosition();
                           });
    return it != closedList.end();
}

aStarNode *PathFinder::findToOpenList(const cocos2d::Vec2 &coord) {
    if (openList.empty())
        return nullptr;
    auto it = std::find_if(openList.begin(), openList.end(),
                           [&coord] (aStarNode *node) {
                               return coord == node->getPosition();
                           });
    if (it != openList.end())
        return (*it);
    return nullptr;
}