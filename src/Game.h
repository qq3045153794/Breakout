#ifndef _GAME_H_
#define _GAME_H_
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ResourceManager.h"
#include "Shader.h"
#include "Texture2D.h"
#include "SpriteRenderer.h"

enum GameState{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game{
public:
    
    GameState state;
    GLboolean keys[1024];
    
    GLuint width,height;
    //render
    SpriteRenderer* renderer;


    Game(GLuint width,GLuint height);
    ~Game();

    void init();
    void process_input(GLfloat dt);
    void update(GLfloat dt);
    void render();

};
#endif