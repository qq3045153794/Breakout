#version 330 core
layout (location=0) out vec4 frag_color;
        
in vec2 tex_coords;

uniform vec3 sprite_color;
uniform sampler2D image;


void main(){

    frag_color=vec4(sprite_color,1.0) * vec4(texture(image,tex_coords).rgb,1.0);
}