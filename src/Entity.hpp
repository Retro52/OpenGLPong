//
// Created by Anton on 03.02.2023.
//

#ifndef OPENGLPONG_ENTITY_H
#define OPENGLPONG_ENTITY_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Entity
{
public:

    Entity() = default;
    virtual ~Entity() = default;

    /* returns transform matrix. Method is virtual because some figures may use different way of generating this matrix */
    [[nodiscard]] virtual glm::mat4 GetTransform() const
    {
        return glm::translate(glm::mat4(1.0F), glm::vec3(pos.x, pos.y, 0.0F)) * glm::scale(glm::mat4(1.0F), glm::vec3(scale.x, scale.y, 1.0F));
    }

    /* getters */
    [[nodiscard]] glm::vec2 GetPos() const { return pos; }
    [[nodiscard]] glm::vec2 GetVelocity () const { return velocity; }

    /**
     * Sets a position
     * @note try to avoid using this and interact with the position using velocity
     * @param newPosition self explanatory
     */
    void SetPosition(const glm::vec2& newPosition) { pos = newPosition; }
    void SetVelocity(const glm::vec2& newVelocity) { velocity = newVelocity; }

    virtual void Update(float deltaTime) = 0;
protected:
    glm::vec2 pos = { 0, 0 };
    glm::vec2 scale = { 0, 0 };
    glm::vec2 velocity = { 0, 0 };
};


#endif //OPENGLPONG_ENTITY_H
