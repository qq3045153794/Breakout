#include "BallObject.h"
BallObject::BallObject() 
    : GameObject(), radius(12.5f), stuck(true)  { }

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
    :  GameObject(pos,glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), 
     radius(radius), 
     stuck(true) { }
glm::vec2 BallObject::move(GLfloat dt, GLuint window_width) {

    if(!this->stuck) {
        this->position = this->position + dt * this->velocity;
        if(this->position.x < 0.0) {
            this->velocity.x = -this->velocity.x;
            this->position.x =0.0;
        }else if(this->position.x + this->size.x > window_width) {
            this->velocity.x = -this->velocity.x;
            this->position.x = window_width - size.x;
        }else if(this->position.y < 0.0) {
            this->velocity.y = -this->velocity.y;
            this->position.y = 0.0;
        }
    }
    return this->position;
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity) {
    this->position = position;
    this->velocity = velocity;
    this->stuck = true;
}
