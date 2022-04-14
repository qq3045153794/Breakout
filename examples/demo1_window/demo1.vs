#version 330 core
layout (location = 0) in vec4 a_ver_tex;

out vec2 tex_coords;
uniform mat4 model;
uniform mat4 projection;
void main(){
    tex_coords=a_ver_tex.zw;
    
    gl_Position=projection * model *  vec4(a_ver_tex.xy,0.0,1.0);
}