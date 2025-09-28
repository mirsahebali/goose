#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;

out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;



void main(){
  float rotation_angle_deg = 180;

  gl_Position =  projection * view * model * transform * vec4(aPos, 1.0);
  TexCoord = aTextureCoord;
}
