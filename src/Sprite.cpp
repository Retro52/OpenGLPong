//
// Created by Anton on 02.02.2023.
//

#include "Sprite.h"

void Sprite::UpdateArrays(std::vector<float> newVertices, std::vector<int> newIndices)
{
    indices = std::move(newIndices);
    vertices = std::move(newVertices);

    RefillBuffers();
}

void Sprite::RefillBuffers()
{
    if (vertices.empty())
    {
        return;
    }

    glBindVertexArray(VAO); // binding buffers

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW); // connecting data

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW); // connecting data for squares

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr); // memory for vertex
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Quad::Draw(const Shader &shader) const
{
    shader.setVec3("material.color", defaultColor);
    shader.setBool("material.isTexturePresent", texture != nullptr);

    if (texture)
    {
        shader.setInt("material.texture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetId());
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glActiveTexture(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}