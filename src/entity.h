//#ifndef ENTITY_H
//#define ENTITY_H

class Entity {
public:
    float x, y, speedX, speedY;
    int id;
    int16_t direction;

    virtual void move();
};


class EntityPlayer : public Entity {
public:
    // Additional member variables or functions specific to PlayerEntity

    void move() override;  // Override the move() method
};

//#endif  // ENTITY_H
