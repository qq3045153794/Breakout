
#ifndef _GameLevel_h_
#define _GameLevel_h_

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

#include <iostream>
#include <vector>
class GameLevel{
public:
    std::vector<GameObject> bricks;
    
    void load(const GLchar* file,
              GLuint level_width,
              GLuint level_height);
    
    void draw(SpriteRenderer& renderer);

    GLboolean is_completed();


private:
    void init(std::vector<std::vector<GLuint>> tile_data,
              GLuint level_width,
              GLuint level_height);
    
};
#endif