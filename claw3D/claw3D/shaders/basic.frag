#version 330 core

out vec4 FragColor;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;   

uniform vec3 u_ObjectColor;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_AmbientColor;
uniform float u_AmbientStrength;
uniform vec3 u_ViewPos;

uniform float u_Shininess;
uniform float u_Alpha;

uniform sampler2D u_Texture;
uniform int u_UseTexture;

void main()
{
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(u_LightPos - v_FragPos);

    vec3 baseColor = u_ObjectColor;

    if (u_UseTexture == 1)
    {
        baseColor = texture(u_Texture, v_TexCoord).rgb;
    }

    vec3 ambient = u_AmbientStrength * u_AmbientColor;

    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * u_LightColor;

    vec3 V = normalize(u_ViewPos - v_FragPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), u_Shininess);
    vec3 specular = 0.35 * spec * u_LightColor;

    vec3 color = (ambient + diffuse) * baseColor + specular;

    FragColor = vec4(color, u_Alpha);
}
