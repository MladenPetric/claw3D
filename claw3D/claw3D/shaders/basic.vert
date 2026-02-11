#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_FragPos;
out vec3 v_Normal;

void main()
{
    vec4 worldPos = u_Model * vec4(aPos, 1.0);
    v_FragPos = worldPos.xyz;

    mat3 normalMat = mat3(transpose(inverse(u_Model)));
    v_Normal = normalize(normalMat * aNormal);

    gl_Position = u_Projection * u_View * worldPos;
}
