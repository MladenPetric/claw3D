#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTex;

void main()
{
    vec4 col = texture(uTex, TexCoord);
    col.a = 0.7;
    FragColor = col;
}
