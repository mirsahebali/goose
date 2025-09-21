#version 330 core
#define PI radian(180)

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float offset_x;

void main(){
  float rotation_angle_deg = 180;

  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  ourColor = aColor;
  TexCoord = aTextureCoord;
}
