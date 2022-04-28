#ifndef _PARTICLEGENERATOR_H_
#define _PARTICLEGENERATOR_H_
#include "ResourceManager.h"
#include "GameObject.h"
#include <iostream>
#include <vector>
struct  Particle {
    glm::vec2 position,velocity;
    glm::vec4 color;
    GLfloat lift;
    Particle():
        position(0.0f),velocity(0.0f),color(1.0f),lift(0.0f){ }
};


class ParticleGenerator {
public:
    ParticleGenerator(Shader shader, Texture2D texture, GLuint nr_particle);
    void init_particle();
    void update(GLfloat dt, GameObject* ball, GLuint nr_new_particles, glm::vec2 offset);
    GLuint first_resest();
    void reset(GLuint idx, GameObject* ball, glm::vec2 offset);
    void draw();
    
private:
    Shader shader;
    Texture2D texture;    
    GLuint last_pos = 0;
    GLuint vao;
    GLuint vbo;
    GLuint nr_particles;
    std::vector<Particle> particles;
};
#endif