#include "ResourceManager.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::map<std::string, Shader>     ResourceManager::shaders;
std::map<std::string, Texture2D>  ResourceManager::texture2Ds;
Shader ResourceManager::load_shader(const GLchar* vshader_file, 
                                    const GLchar* fshader_file, 
                                    const GLchar* gshader_file, 
                                    std::string name){
    
    shaders[name] = load_shader_from_file(vshader_file,fshader_file,gshader_file);
    return shaders[name];
}

Shader ResourceManager::load_shader(const GLchar* vshader_file, 
                           const GLchar* fshader_file, 
                           std::string name){
    shaders[name] = load_shader_from_file(vshader_file,fshader_file);
    return shaders[name];                           
}

Shader& ResourceManager::get_shader(std::string name){
    return shaders[name];
}

Texture2D ResourceManager::load_texture(const GLchar *file,  
                                        std::string name){
    texture2Ds[name] = load_texture_from_file(file);
    return texture2Ds[name];
}


Texture2D& ResourceManager::get_texture(std::string name){
    return texture2Ds[name];
}

void ResourceManager::clear(){
    for(auto &iter :shaders){
        glDeleteProgram(iter.second.ID);
    }
    for(auto &iter :texture2Ds){
        glDeleteTextures(1,&iter.second.ID);
    }
    shaders.clear();
    texture2Ds.clear();
}

Shader ResourceManager::load_shader_from_file(const GLchar* vshader_file,
                                              const GLchar* fshader_file){
    Shader shader;
    std::string vertex_code;
    std::string fragment_code;
    

    //读文件
    std::ifstream vertex_file;
    std::ifstream fragment_file;
    

    vertex_file .exceptions(std::ifstream::failbit|std::ifstream::badbit);    
    fragment_file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    
    try{
        vertex_file.open(vshader_file);
        fragment_file.open(fshader_file);
        std::stringstream vertex_stream;
        std::stringstream fragment_stream;
        

        vertex_stream<<vertex_file.rdbuf();
        fragment_stream<<fragment_file.rdbuf();

        vertex_code=vertex_stream.str();
        fragment_code=fragment_stream.str();
        
        vertex_file.close();
        fragment_file.close();

    }catch(std::ifstream::failure e){
        std::cout << "ERROR::SHADER::FILE_NO_SUCCESFULLY_READ" << std::endl;
    }                                              
    shader.compile(vertex_code.c_str(),fragment_code.c_str());
    return shader;
}


Shader ResourceManager::load_shader_from_file(const GLchar* vshader_file,
                                              const GLchar* fshader_file,
                                              const GLchar* gshader_file){
    Shader shader;
    std::string vertex_code;
    std::string fragment_code;
    std::string geometry_code;

    //读文件
    std::ifstream vertex_file;
    std::ifstream fragment_file;
    std::ifstream geometry_file;

    vertex_file .exceptions(std::ifstream::failbit|std::ifstream::badbit);    
    fragment_file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    geometry_file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    
    try{
        vertex_file.open(vshader_file);
        fragment_file.open(fshader_file);
        geometry_file.open(gshader_file);
        std::stringstream vertex_stream;
        std::stringstream fragment_stream;
        std::stringstream geometry_stream;

        vertex_stream<<vertex_file.rdbuf();
        fragment_stream<<fragment_file.rdbuf();
        geometry_stream<<geometry_file.rdbuf();

        vertex_code=vertex_stream.str();
        fragment_code=fragment_stream.str();
        geometry_code=fragment_stream.str();
        vertex_file.close();
        fragment_file.close();
        geometry_file.close();

    }catch(std::ifstream::failure e){
        std::cout << "ERROR::SHADER::FILE_NO_SUCCESFULLY_READ" << std::endl;
    }                                              
    shader.compile(vertex_code.c_str(),fragment_code.c_str(),geometry_code.c_str());
    return shader;
}

Texture2D ResourceManager::load_texture_from_file(const GLchar *file){
    Texture2D texture2D;
    int width, height, nrComponents;
    unsigned char *image = stbi_load(file, &width, &height, &nrComponents, 0);
    if(image){
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1){
            texture2D.internal_format = texture2D.image_format = GL_RED;
        }
        else if (nrComponents == 3){
            texture2D.internal_format = texture2D.image_format = GL_RGB;
        }
        else if (nrComponents == 4){
            texture2D.internal_format = texture2D.image_format = GL_RGBA;
        }
        texture2D.generate(width,height,image);
        stbi_image_free(image);
    }else{
        std::cout << "Texture failed to load at path: " << file << std::endl;
        stbi_image_free(image);
    }
    return texture2D;
}
