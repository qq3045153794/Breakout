#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Texture2D.h"
#include "SpriteRenderer.h"


class GameObject{
public:
    // Object state 位置，大小，速度
    glm::vec2   position, size, velocity;
    glm::vec3   color;
    GLfloat     rotation;
    GLboolean   is_solid;
    GLboolean   destroyed;
    // Render state
    Texture2D   sprite;	
    // Constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, 
               glm::vec2 size, 
               Texture2D sprite, 
               glm::vec3 color = glm::vec3(1.0f), 
               glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer);
};

#endif