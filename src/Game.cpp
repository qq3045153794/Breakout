#include "Game.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "irrKlang.h"
using namespace irrklang;
#include <algorithm>
//球的返回方向
enum Dirction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
typedef std::tuple<GLboolean, Dirction, glm::vec2> collision;
// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100, 20);
// 初始化当板的速率
const GLfloat PLAYER_VELOCITY(500.0f);
// 初始化小球速度
const glm::vec2 INTTIAL_BALL_VELOCITY(100.0, -350.0);
// 初始化小球半径
const GLfloat BALL_RADIUS = 12.5;

GLfloat shake_time = 0.0f;

GameObject* player;
BallObject* ball;
ParticleGenerator* particleGenerator;
PostProcessor* effects;
ISoundEngine* sound =  createIrrKlangDevice();

//判断球从AABB那个方向撞过来
Dirction  vectorDirction(glm::vec2 target) {
    std::vector<glm::vec2> tmp = {
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 0.0),
        glm::vec2(0.0, -1.0),
        glm::vec2(-1.0, 0.0),
    };
    GLfloat mx = 0.0;
    GLint pos = 0;
    for(int i=0; i < 4; i++) {
        if(glm::dot(tmp[i], glm::normalize(target)) > mx) {
            mx = dot(tmp[i], glm::normalize(target));
            pos = i;
        }
    }
    
    return static_cast<Dirction> (pos);
}

GLboolean CheckCollision(GameObject& one, GameObject& two) {
    //AA_BB碰撞
    GLfloat x1 = one.position.x;
    GLfloat x2 = one.position.x + one.size.x;
    GLfloat x3 = two.position.x;
    GLfloat x4 = two.position.x + two.size.x;
    GLfloat y1 = one.position.y;
    GLfloat y2 = one.position.y + one.size.y;
    GLfloat y3 = two.position.y;
    GLfloat y4 = two.position.y + two.size.y;
    //std::cout<<std::max(x1, x3)<<" "<<std::min(x2, x4)<<" "<<std::max(y1, y3)<<" "<<std::min(y2, y4)<<"\n";
    //std::cout<<"bool="<<((std::max(x1, x3)<=std::min(x2, x4))&&(std::max(y1, y3)<=std::min(y2, y4)))<<"\n";
    if((std::max(x1, x3)<=std::min(x2, x4))&&(std::max(y1, y3)<=std::min(y2, y4)))
        return GL_TRUE;
    return GL_FALSE;
}

collision CheckCollision(BallObject& one, GameObject& two) {
    glm::vec2 center(one.position + one.radius);
    glm::vec2 aabb_half_extents(two.size.x / 2, two.size.y / 2);
    glm::vec2 aabb_center(
        two.position.x + aabb_half_extents.x, 
        two.position.y + aabb_half_extents.y
    );
    glm::vec2 diff =center - aabb_center;
    glm::vec2 clamp = glm::clamp (diff, -aabb_half_extents, aabb_half_extents);
    glm::vec2 p = aabb_center + clamp;
    glm::vec2 p_vector = p - center;
    if(glm::length(p_vector) <= one.radius) {
        return collision(GL_TRUE, vectorDirction(p_vector), p_vector);
    }else {
        return collision(GL_FALSE, UP, glm::vec2(0.0, 0.0));
    }

}

GLboolean ShouldSpawn(GLuint chance) {
    GLuint random = rand() %chance;
    return random == 0;
}
void Game::SpawnPowerUps(GameObject& block)
{
    if (ShouldSpawn(75)) // 1/75的几率
        this->PowerUps.push_back(
             PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.position, ResourceManager::get_texture("tex_speed") 
         ));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(
            PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.position, ResourceManager::get_texture("tex_sticky")
        ));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(
            PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.position, ResourceManager::get_texture("tex_pass")
        ));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(
            PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.position,ResourceManager::get_texture("tex_size")     
        ));
    if (ShouldSpawn(15)) // 负面道具被更频繁地生成
        this->PowerUps.push_back(
            PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.position, ResourceManager::get_texture("tex_confuse")
        ));
    if (ShouldSpawn(15))
        this->PowerUps.push_back(
            PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.position, ResourceManager::get_texture("tex_chaos")
        ));
}

void ActivatePowerUp(PowerUp &powerUp)
{
    // 根据道具类型发动道具
    if (powerUp.type == "speed")
    {
        ball->velocity *= 1.2;
    }
    else if (powerUp.type == "sticky")
    {
        ball->sticky = GL_TRUE;
        player->color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp.type == "pass-through")
    {
        ball->pass_through = GL_TRUE;
        ball->color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp.type == "pad-size-increase")
    {
        player->size.x += 50;
    }
    else if (powerUp.type == "confuse")
    {
        if (!effects->chaos)
            effects->confuse = GL_TRUE; // 只在chaos未激活时生效，chaos同理
    }
    else if (powerUp.type == "chaos")
    {
        if (!effects->confuse)
            effects->chaos = GL_TRUE;
    }
} 

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
    ResourceManager::load_shader("../paricle.vs","../paricle.fs","paricle");
    ResourceManager::load_shader("../frame.vs", "../frame.fs", "frame");
    glm::mat4 projection = glm::ortho(0.0f, 
                                      static_cast<GLfloat>(this->width), 
                                      static_cast<GLfloat>(this->height), 
                                      0.0f, -1.0f, 1.0f);
	ResourceManager::get_shader("sprite").use().set_integer("image",0);
    ResourceManager::get_shader("sprite").set_matrix4("projection", projection);

    ResourceManager::get_shader("paricle").use().set_integer("image",0);
    ResourceManager::get_shader("paricle").set_matrix4("projection", projection);

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
    ResourceManager::load_texture("../particle.png", 
                                  "particle");
    
    ResourceManager::load_texture("../powerup_speed.png", 
                                  "tex_speed");                              
    ResourceManager::load_texture("../powerup_confuse.png", 
                                  "tex_confuse");
    ResourceManager::load_texture("../powerup_increase.png", 
                                  "tex_size");
    ResourceManager::load_texture("../powerup_passthrough.png", 
                                  "tex_pass");
    ResourceManager::load_texture("../powerup_sticky.png", 
                                  "tex_sticky");
    ResourceManager::load_texture("../powerup_chaos.png", 
                                  "tex_chaos");
                                  
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

    //加载挡板
    glm::vec2 player_pos = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, 
                                    this->height - PLAYER_SIZE.y);
    player = new GameObject(player_pos, PLAYER_SIZE, ResourceManager::get_texture("paddle"));
    //加载小球
    glm::vec2 ballPos =player_pos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS , -2 * BALL_RADIUS);
    ball = new BallObject(ballPos, BALL_RADIUS, INTTIAL_BALL_VELOCITY, ResourceManager::get_texture("face"));
    //加载粒子发射器
    particleGenerator = new  ParticleGenerator(ResourceManager::get_shader("paricle"), ResourceManager::get_texture("particle"), 500);
    //后期处理
    effects = new PostProcessor(ResourceManager::get_shader("frame"), this->width, this->height);

    sound->play2D("../breakout.mp3",GL_TRUE);
}

void Game::update(GLfloat dt) {
    ball->move(dt, this->width);  
    particleGenerator->update(dt, ball, 2, glm::vec2( ball->radius / 2.0));
    this->doCollision(); 
    this->UpdatePowerUps(dt);
    if(ball->position.y >= this->height){
        this->reset_level();
        this->reset_player();
    }

    if(shake_time > 0.0f) {
        shake_time -=dt;
        if(shake_time <= 0.0f)
            effects->shake = false;
    }
}

void Game::process_input(GLfloat dt) {
    if (this->state == GAME_ACTIVE) {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // 移动挡板
        if (this->keys[GLFW_KEY_A]) {
            if (player->position.x >= 0){
                player->position.x -= velocity;
                if(ball->stuck) 
                    ball->position.x -= velocity;
            }
        }
        if (this->keys[GLFW_KEY_D]) {
            if (player->position.x <= this->width - player->size.x) {
                player->position.x += velocity;
                if(ball->stuck)
                    ball->position.x += velocity;
            }
        }
    }
    if (this->keys[GLFW_KEY_SPACE])
        ball->stuck = false;
}

void Game::render(){
    if(this->state == GAME_ACTIVE) {        
        
        effects->begin_render();
            renderer->DrawSprite(ResourceManager::get_texture("background"),
                            glm::vec2(0, 0),
                            glm::vec2(this->width, this->height),
                            0.0f
                            );
            this->levels[level].draw(*renderer);
            player->Draw(*renderer);
            ball->Draw(*renderer);
            particleGenerator->draw();
            for (PowerUp &powerUp : this->PowerUps) if (!powerUp.destroyed) powerUp.Draw(*renderer);
        effects->end_render();
        effects->render(glfwGetTime());
        
    }
}

//碰撞处理
void Game::doCollision() {
    std::cout<<"pass="<<ball->pass_through<<"\n";
    for(auto& box : this->levels[this->level].bricks) {
        if(!box.destroyed) {
            auto res = CheckCollision(*ball, box);
            if(std::get<0>(res)) {
                std::cout<<"destroyed"<<"\n";
                if(!box.is_solid){
                    box.destroyed = GL_TRUE;
                    this->SpawnPowerUps(box);
                }else {
                    shake_time = 0.05f;
                    effects->shake = true;
                }
                auto dir = std::get<1>(res);
                auto v = std::get<2>(res);
                if(box.is_solid || !ball->pass_through){
                    if(dir == LEFT || dir == RIGHT) {
                        ball->velocity.x = -ball->velocity.x;
                        GLfloat dif = ball->radius - std::abs(v.x);
                        if(dir == LEFT) {
                            ball->position.x += dif;
                        }else {
                            ball->position.x -= dif;
                        }
                    }else {
                        ball->velocity.y = -ball->velocity.y;
                        GLfloat dif = ball->radius - std::abs(v.y);
                        if(dif == UP) {
                            ball->position.y -= dif;
                        }else {
                            ball->position.y += dif;
                        }
                    }
                }
            }
        }
    }

    //玩家与板子
    auto ans = CheckCollision(*ball, *player);
    if(!ball->stuck && std::get<0>(ans)) {
        GLfloat center =player->position.x + player->size.x/2.0;
        GLfloat dis = ball->position.x + ball->radius - center;
        GLfloat percentage = dis / (player->size.x / 2.0);
    
        GLfloat strength = 2.0;
        glm::vec2 old_velocity = ball->velocity;
        ball->velocity.x = INTTIAL_BALL_VELOCITY.x  * percentage * strength;
        ball->velocity.y = -std::fabs(ball->velocity.y);
        ball->velocity = glm::normalize(ball->velocity) * glm::length(old_velocity);

        ball->stuck = ball->sticky;
    }

    for (PowerUp& powerUp : this->PowerUps) {
        if (!powerUp.destroyed) {
            if (powerUp.position.y >= this->height)
                powerUp.destroyed = GL_TRUE;

            if (CheckCollision(*player, powerUp)) {   
                // 道具与挡板接触，激活它！
                ActivatePowerUp(powerUp);
                powerUp.destroyed = GL_TRUE;
                powerUp.activated = GL_TRUE;
            }
        }
    } 
}

GLboolean isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
    for (const PowerUp &powerUp : powerUps)
    {
        if (powerUp.activated)
            if (powerUp.type == type)
                return GL_TRUE;
    }
    return GL_FALSE;
}  

void Game::UpdatePowerUps(GLfloat dt) {
    for (PowerUp &powerUp : this->PowerUps) {
        powerUp.position += powerUp.velocity * dt;
        if (powerUp.activated) {
            powerUp.duration -= dt;

            if (powerUp.duration <= 0.0f) {
                // 之后会将这个道具移除
                powerUp.activated = GL_FALSE;
                // 停用效果
                if (powerUp.type == "sticky") {
                    if (!isOtherPowerUpActive(this->PowerUps, "sticky")) {   
                        // 仅当没有其他sticky效果处于激活状态时重置，以下同理
                        ball->sticky = GL_FALSE;
                        player->color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.type == "pass-through") {
                    if (!isOtherPowerUpActive(this->PowerUps, "pass-through"))  {
                        ball->pass_through = GL_FALSE;
                        ball->color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.type == "confuse") {
                    if (!isOtherPowerUpActive(this->PowerUps, "confuse")) {
                        effects->confuse = GL_FALSE;
                    }
                }
                else if (powerUp.type == "chaos") {
                    if (!isOtherPowerUpActive(this->PowerUps, "chaos")) {
                        effects->chaos = GL_FALSE;
                    }
                }                
            }
        }
    }
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUp &powerUp) { return powerUp.destroyed && !powerUp.activated; }
    ), this->PowerUps.end());
}  

void Game::reset_level() {
    if (this->level == 0) 
        this->levels[0].load("../1.lvl", this->width, this->height * 0.5f);
    else if (this->level == 1)
        this->levels[1].load("../2.lvl", this->width, this->height * 0.5f);
    else if (this->level == 2)
        this->levels[2].load("../3.lvl", this->width, this->height * 0.5f);
    else if (this->level == 3)
        this->levels[3].load("../4.lvl", this->width, this->height * 0.5f);
    std::cout<<"sz="<<this->levels[0].bricks.size()<<"\n";
    this->level = 0;
}

void Game::reset_player() {
    //加载挡板
    glm::vec2 player_pos = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, 
                                    this->height - PLAYER_SIZE.y);
    player->size = PLAYER_SIZE;
    player->position = player_pos;
    //加载小球
    glm::vec2 ball_pos =player_pos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS , -2 * BALL_RADIUS);
    ball->reset(ball_pos, INTTIAL_BALL_VELOCITY);
}