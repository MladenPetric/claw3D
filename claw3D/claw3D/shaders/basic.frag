#version 330 core

out vec4 FragColor;

in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_ObjectColor;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPos;

uniform float u_Shininess; // npr 32.0

uniform float u_Alpha;

void main()
{
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(u_LightPos - v_FragPos);

    // ambient
    vec3 ambient = 0.15 * u_LightColor;

    // diffuse
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // specular (Blinn-Phong)
    vec3 V = normalize(u_ViewPos - v_FragPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), u_Shininess);
    vec3 specular = 0.35 * spec * u_LightColor;

    vec3 color = (ambient + diffuse) * u_ObjectColor + specular;
    FragColor = vec4(color, u_Alpha);



}
