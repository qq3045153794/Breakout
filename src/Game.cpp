#include "Game.h"

Game::Game(GLuint width, GLuint height) 
	: state(GAME_ACTIVE), keys(), width(width), height(height) { 
    
}

Game::~Game(){

}

void Game::init(){


    ResourceManager::load_shader("../demo1.vs",
                                 "../demo1.fs","sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
	ResourceManager::get_shader("sprite").use().set_integer("image",0);
    ResourceManager::get_shader("sprite").set_matrix4("projection", projection);

    ResourceManager::load_texture("../awesomeface.png","face");
    renderer = new SpriteRenderer(ResourceManager::get_shader("sprite"));
}

void Game::update(GLfloat dt) {

}


void Game::process_input(GLfloat dt) {

}

void Game::render(){
    renderer->DrawSprite(ResourceManager::get_texture("face"),
                         glm::vec2(200,200),
                         glm::vec2(300,400),
                         45.0f,
                         glm::vec3(0.0f,1.0f,0.0f));
}