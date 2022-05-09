#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_
#include "GameObject.h"

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject {
public:
    std::string type;
    GLfloat duration;
    GLboolean activated;
    PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
        :GameObject(position, SIZE, texture, color, VELOCITY),
        type(type), duration(duration), activated(){ }
};
#endif