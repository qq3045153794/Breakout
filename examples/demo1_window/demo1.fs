#version 330 core
layout (location=0) out vec4 frag_color;
        
in vec2 tex_coords;

uniform vec3 sprite_color;
uniform sampler2D image;


void main(){
    vec4 tex_color = vec4(sprite_color,1.0) * texture(image,tex_coords);
    if(tex_color.a < 0.1)
        discard;
    
    frag_color=tex_color;
}