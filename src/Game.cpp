#include "Game.h"

// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100, 20);
// 初始化当班的速率
const GLfloat PLAYER_VELOCITY(500.0f);

GameObject* player;


Game::Game(GLuint width, GLuint height) 
	: state(GAME_ACTIVE), keys(), width(width), height(height) { 
    
}

Game::~Game(){

}

void Game::init(){


    
    //加载shader
    ResourceManager::load_shader("../demo1.vs",
                                 "../demo1.fs",
                                 "sprite");
    
    
    glm::mat4 projection = glm::ortho(0.0f, 
                                      static_cast<GLfloat>(this->width), 
                                      static_cast<GLfloat>(this->height), 
                                      0.0f, -1.0f, 1.0f);
	ResourceManager::get_shader("sprite").use().set_integer("image",0);
    ResourceManager::get_shader("sprite").set_matrix4("projection", projection);

    
    //加载纹理
    ResourceManager::load_texture("../awesomeface.png",
                                  "face");
    ResourceManager::load_texture("../block.png",
                                  "block");
    ResourceManager::load_texture("../block_solid.png",
                                  "block_solid");
    ResourceManager::load_texture("../background.jpg",
                                  "background");
    ResourceManager::load_texture("../paddle.png", 
                                  "paddle");
    
    //加载关卡
    GameLevel one; one.load("../1.lvl",
                            this->width,
                            this->height * 0.5);
    GameLevel two; two.load("../2.lvl",
                            this->width,
                            this->height * 0.5);
    GameLevel three; three.load("../3.lvl",
                                this->width,
                                this->height * 0.5);
    GameLevel four; four.load("../4.lvl",
                             this->width,
                             this->height * 0.5);

    this->levels.push_back(one);
    this->levels.push_back(two);
    this->levels.push_back(three);
    this->levels.push_back(four);    
    this->level = 0;

    renderer = new SpriteRenderer(ResourceManager::get_shader("sprite"));

    glm::vec2 playerPos = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, 
                                    this->height - PLAYER_SIZE.y);
    player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::get_texture("paddle"));
}

void Game::update(GLfloat dt) {

}


void Game::process_input(GLfloat dt) {
    if (this->state == GAME_ACTIVE) {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // 移动挡板
        if (this->keys[GLFW_KEY_A]) {
            if (player->position.x >= 0)
                player->position.x -= velocity;
        }
        if (this->keys[GLFW_KEY_D])
        {
            if (player->position.x <= this->width - player->size.x)
                player->position.x += velocity;
        }
    }
}

void Game::render(){
    
    
    if(this->state == GAME_ACTIVE) {

    
                                                  
        renderer->DrawSprite(ResourceManager::get_texture("background"),
                         glm::vec2(0, 0),
                         glm::vec2(this->width, this->height),
                         0.0f
                         );
        this->levels[level].draw(*renderer);
        player->Draw(*renderer);
    }
    
}