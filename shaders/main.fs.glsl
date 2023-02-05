#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

struct Material
{
    vec3 color;
    sampler2D texture;
    bool isTexturePresent;
};

uniform Material material;

void main()
{
    if (material.isTexturePresent)
    {
        vec4 texFetch = texture(material.texture, TexCoords);
        FragColor = texFetch;
        return;
    }

    FragColor = vec4(material.color, 1.0F);
}