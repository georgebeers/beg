#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec3 vCol;

uniform mat4 model;
uniform mat4 combined;

out vec3 vertNormal;
out vec3 vertColor;
out vec3 fragPosition;

void main()
{
    gl_Position = combined * model * vec4(vPos.x, vPos.y, vPos.z, 1.0);

    vertNormal = normalize(mat3(transpose(inverse(model))) * vNorm);
    vertColor = vCol;
    fragPosition = vec3(model * vec4(vPos, 1.0));
}