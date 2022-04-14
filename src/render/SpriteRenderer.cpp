#include "SpriteRenderer.h"


SpriteRenderer::SpriteRenderer (Shader& shader) 
    :shader(shader){
    this->initRenderData();
}
SpriteRenderer::~SpriteRenderer(){
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::initRenderData(){
    GLuint VBO;
    GLfloat vertices[]={
        // 位置     // 纹理
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1,&this->quadVAO);
    glGenBuffers(1,&VBO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D& texture,
                                glm::vec2 position,
                                glm::vec2 size,
                                GLfloat rotate,
                                glm::vec3 color){
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0);


    //先缩放，后旋转，最后平移，由于线性变换是从右到左，所以变换需要反过来
    
    model = glm::translate(model, glm::vec3(position, 0.0f));  
    
    
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f)); 

    this->shader.set_matrix4("model",model);
    this->shader.set_vector3f("sprite_color",color);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}