#version 330 core

layout (location = 0) in vec3 aPos;

uniform vec4 inputColor;
// uniform mat4 MVPprojections;
flat out vec4 pixelColor;

void main(){
  // gl_Position = MVPprojections * vec4(aPos, 1.0);
  gl_Position = vec4(aPos, 1.0);
  pixelColor = inputColor;
}
