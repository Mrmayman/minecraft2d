//#ifndef ENTITY_H
//#define ENTITY_H

class Entity {
public:
    float x, y, speedX, speedY;
    int id;
    int16_t direction;

    void move();
};

//#endif  // ENTITY_H
