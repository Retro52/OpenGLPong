//
// Created by Anton on 02.02.2023.
//

#ifndef OPENGLPONG_SPRITE_H
#define OPENGLPONG_SPRITE_H

#include <utility>
#include <vector>
#include <iostream>

#include "glew.h"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Sprite
{
public:
    explicit Sprite(std::vector<float> vertices = {}, std::vector<int> indices = {}) : vertices(std::move(vertices)), indices(std::move(indices))
    {
        glGenVertexArrays(1, &VAO); // generating vertex arrays
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        RefillBuffers();
    }

    virtual ~Sprite()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    virtual void Draw(const Shader& shader) const = 0;

protected:
    void UpdateArrays(std::vector<float> newVertices = {}, std::vector<int> newIndices = {});

private:
    void RefillBuffers();

protected:
    std::vector<int> indices;
    std::vector<float> vertices;
    unsigned int VAO = 0, VBO = 0, EBO = 0;
};

class Quad : public Sprite
{
public:
    explicit Quad() : Sprite()
    {
        UpdateArrays(
            std::vector<float>(
                {
                    - 1.0F / 2, + 1.0F / 2, 0.0f, 0.0f,
                    - 1.0F / 2, - 1.0F / 2, 0.0f, 1.0f,
                    + 1.0F / 2, - 1.0F / 2, 1.0f, 1.0f,

                    - 1.0F / 2, + 1.0F / 2, 0.0f, 0.0f,
                    + 1.0F / 2, - 1.0F / 2, 1.0f, 1.0f,
                    + 1.0F / 2, + 1.0F / 2, 1.0f, 0.0f
                }
            )
        );
    }

    explicit Quad(std::shared_ptr<Texture> quadTexture) : Quad()
    {
        texture = std::move(quadTexture);
    }

    /**
     * Draws quad applying some simple material. Notice that shader should be bound before the draw call
     * @param shader shader to apply
     */
    void Draw(const Shader& shader) const override;

    void AddOrReplaceTexture(std::shared_ptr<Texture> attachmentTexture)
    {
        texture = std::move(attachmentTexture);
    }

protected:
    glm::vec3 defaultColor = glm::vec3(0.0F);
private:
    std::shared_ptr<Texture> texture = nullptr;
};


#endif //OPENGLPONG_SPRITE_H
