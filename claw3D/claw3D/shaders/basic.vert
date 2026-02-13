#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;   

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;   

void main()
{
    vec4 worldPos = u_Model * vec4(aPos, 1.0);
    v_FragPos = worldPos.xyz;

    mat3 normalMat = mat3(transpose(inverse(u_Model)));
    v_Normal = normalize(normalMat * aNormal);

    v_TexCoord = aTexCoord;  

    gl_Position = u_Projection * u_View * worldPos;
}
