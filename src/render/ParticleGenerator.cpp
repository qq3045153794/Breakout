#include "ParticleGenerator.h"


ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint nr_particles):
    shader(shader),texture(texture),nr_particles(nr_particles) {
    this->init_particle();
    this->last_pos = 0;
}

void ParticleGenerator::init_particle() {
    for(int i=0; i < nr_particles; i++) {
        this->particles.push_back(Particle());
    }
     
    GLfloat particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    }; 
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glBindVertexArray(this->vao);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
}

void ParticleGenerator::update(GLfloat dt, GameObject* ball, GLuint nr_new_particles, glm::vec2 offset) {
    for(int i = 0; i < nr_new_particles; i++) {
        GLuint idx = this->first_resest();
        this->reset(idx, ball, offset);
    }

    for(int i=0; i < this->nr_particles; i++) {
        auto& t = this->particles[i];
        t.lift -= dt;
        if (t.lift > 0.0f){
            t.color.a -= 2.5 * dt;
            t.position -= dt * t.velocity;
        }
    }
}
GLuint ParticleGenerator::first_resest() {
    for(GLuint i = this->last_pos; i < this->nr_particles; i++) {
        if(this->particles[i].lift <= 0.0){ 
            last_pos = i;
            return i;
        }
    }
    for(GLuint i = 0 ;i <last_pos; i++) {
        if(this->particles[i].lift <=0.0){
            last_pos = i;
            return i;
        }
    }
    last_pos = 0;
    return 0;
}
void ParticleGenerator::reset(GLuint idx, GameObject* ball, glm::vec2 offset) {
    GLfloat rand_position = (rand()%100 - 50) / 10.0;
    GLfloat rand_color = 0.5 + (rand()%100) / 100.0;
    this->particles[idx].position = ball->position + rand_position + offset;
    this->particles[idx].color = glm::vec4(glm::vec3(rand_color), 1.0);
    this->particles[idx].lift = 1.0;
    this->particles[idx].velocity = ball->velocity * 0.1f;
}
void ParticleGenerator::draw() {
    //达到平滑的发热效果
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    for(const auto& it : this->particles) {
        if(it.lift > 0.0) {
            this->shader.set_vector2f("offset", it.position);
            this->shader.set_vector4f("color", it.color);
            this->texture.bind();
            glBindVertexArray(this->vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}