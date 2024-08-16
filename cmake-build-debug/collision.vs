#version 330 core
layout (location = 0) in vec4 aPos;
uniform mat4 matrix;
void main()
{
   gl_Position = matrix * vec4(aPos.x, aPos.y, aPos.z, aPos.w);
}
