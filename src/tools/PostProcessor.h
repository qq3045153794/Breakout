#ifndef _POST_PROCESSOR_H_
#define _POST_PROCESSOR_H_
#include "glad/glad.h"
#include <glm/glm.hpp>

#include "Texture2D.h"
#include <SpriteRenderer.h>
#include "Shader.h"

class PostProcessor {
public:
    Shader post_processing_shader;
    Texture2D texture;
    GLuint width, height;
    GLboolean confuse, chaos, shake;
    PostProcessor(Shader shader, GLuint width, GLuint height);

    void begin_render();
    void end_render();
    void render(GLfloat time); 
private:
    GLuint MSFBO,FBO;
    GLuint RBO;
    GLuint VAO;

    void init_render_data();
};

#endif