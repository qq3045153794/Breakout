#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

#include <glad/glad.h>

class Texture2D
{
public:
    //纹理句柄
    GLuint ID;
    // 纹理的宽高，即分辨率
    GLuint width, height; 
    //纹理对象格式
    GLuint internal_format;
    //加载图像的格式
    GLuint image_format; 
    //S 轴环绕模式
    GLuint wrap_s; 
    //T 轴环绕模式
    GLuint wrap_t; 
    //纹理像素 < 屏幕像素时的过滤模式
    GLuint filter_min; 
    //纹理像素 > 屏幕像素时的过滤模式
    GLuint filter_max; 
    // 默认构造
    Texture2D();
    //Generates texture from image data
    void generate(GLuint width, GLuint height, unsigned char* data);
    //将纹理绑定为当前活动的 GL_TEXTURE_2D 纹理对象
    void bind() const;
};

#endif