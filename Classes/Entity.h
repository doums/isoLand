//
// Created by pierre on 02/04/18.
//

#ifndef ISOLAND_ENTITY_H
#define ISOLAND_ENTITY_H

class Entity {
    enum class EntityType {
        worm,
        hero
    };

private:
    EntityType type;
};

#endif //ISOLAND_ENTITY_H
