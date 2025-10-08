#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texture_coordinates>
out vec2 TextureCoord;

uniform mat4 projections;

void main(){
  gl_Position = projections * vec4(vertex.xy, 0.0, 1.0);
  TextureCoord = vertex.zw;
}

