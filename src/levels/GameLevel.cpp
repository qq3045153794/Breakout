
#include "GameLevel.h"


void GameLevel::load(const GLchar* file,
                     GLuint level_width,
                     GLuint level_height){
    
    std::vector<std::vector<GLuint>> tile_data;

    std::ifstream level_file(file);
    
    if(file) {
        std::string line;
        GLuint num;
        
        while(std::getline(level_file, line)) {

            std::stringstream level_sstream(line);
            std::vector<GLuint> row; 
            while(level_sstream>>num) 
                row.push_back(num);
            tile_data.push_back(row);
        }
        level_file.close();
        if(tile_data.size()>0){
            init(tile_data,level_width,level_height);
        }
    }
    else {
        level_file.close();
        fprintf(stderr, "Error: leval_file failed, FATH = (%s)\n",file);
        exit(1);
    }
}

void GameLevel::init(std::vector<std::vector<GLuint>> tile_data,
                     GLuint level_width,
                     GLuint level_height){
    GLuint row = tile_data.size();
    GLuint col = tile_data[0].size();

    GLfloat unit_height=static_cast<GLfloat>(level_height)/row;
    GLfloat unit_width=static_cast<GLfloat>(level_width)/col;
    

    for(GLuint i = 0; i < row; i++) {
        for(GLuint j = 0; j < col; j++) {
            
            switch (tile_data[i][j]) {
                case 1 : {
                    GameObject object(
                        glm::vec2(j * unit_width, i * unit_height),
                        glm::vec2(unit_width, unit_height),
                        ResourceManager::get_texture("block_solid"),
                        glm::vec3(0.8f, 0.8f, 0.7f)
                    );
                    object.is_solid=GL_TRUE;
                    this->bricks.push_back(object);
                }
                case 2 : {
                    GameObject object(
                        glm::vec2(j * unit_width, i * unit_height),
                        glm::vec2(unit_width, unit_height),
                        ResourceManager::get_texture("block"),
                        glm::vec3(0.2f, 0.6f, 1.0f)
                    );
                    object.is_solid=false;
                    this->bricks.push_back(object);
                    break;

                }
                case 3 : {
                    GameObject object(
                        glm::vec2(j * unit_width, i * unit_height),
                        glm::vec2(unit_width, unit_height),
                        ResourceManager::get_texture("block"),
                        glm::vec3(0.0f, 0.7f ,0.0f)
                    );
                    object.is_solid=false;
                    this->bricks.push_back(object);
                    break;
                }
                case 4 : {
                    GameObject object(
                        glm::vec2(j * unit_width, i * unit_height),
                        glm::vec2(unit_width, unit_height),
                        ResourceManager::get_texture("block"),
                        glm::vec3(0.8f, 0.8f, 0.4f)
                    );
                    object.is_solid=false;
                    this->bricks.push_back(object);
                    break;
                }
                case 5 :{   
                    GameObject object(
                        glm::vec2(j * unit_width, i * unit_height),
                        glm::vec2(unit_width, unit_height),
                        ResourceManager::get_texture("block"),
                        glm::vec3(1.0f, 0.5f, 0.0f)
                    );
                    object.is_solid=false;
                    this->bricks.push_back(object);
                    break;
                }
                default :{
                    //为空，不画
                }
            }
        }
    }                     
}

void GameLevel::draw(SpriteRenderer& renderer) {
    for(GameObject& obj : this->bricks) {
        obj.Draw(renderer);
    }
}

GLboolean GameLevel::is_completed() {
    for(GameObject& obj : this->bricks) {
        if(!obj.is_solid&&!obj.destroyed) {
            return GL_FALSE;
        }
    }
    return GL_TRUE;
}
