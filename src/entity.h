#ifndef ENTITY_H
#define ENTITY_H

class Entity {
  public:
    float x, y;
    float speedX = 0;
    float speedY = 0;
    int id;
    int16_t direction;
    int width = 64;
    int height = 64;

    virtual void tick();
    virtual void move();
};

class EntityPlayer : public Entity {
  public:
    EntityPlayer() {
        width = 32;
        height = 128;
    }

    void move() override;
};

#endif // ENTITY_H
