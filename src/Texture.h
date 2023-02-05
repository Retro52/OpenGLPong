//
// Created by Anton on 02.02.2023.
//

#ifndef OPENGLPONG_TEXTURE_H
#define OPENGLPONG_TEXTURE_H

#include <string>
#include <memory>
#include <iostream>

#include "glew.h"
#include "Utils.hpp"

class Texture
{
public:
    Texture(Texture&& ) = default;
    Texture(const Texture& ) = default;

    /**
     * Loads texture and creates all OpenGL needed data
     * @param path texture path
     * @param gamma gamma correction (not implemented)
     */
    explicit Texture(const std::string& path);

    /**
     * Creates an empty texture
     * @param width texture width
     * @param height texture height
     * @param format texture format, GL_RGBA by default
     * @param internalFormat texture internal format, GL_RGBA by default
     * @param pixelType texture pixel tipe, GL_UNSIGNED_BYTE by default
     * @param repeat if texture should be repeated or clamped to the border, true by default
     */
    Texture(GLsizei width, GLsizei height, unsigned int format = GL_RGBA, unsigned int internalFormat = GL_RGBA, unsigned int pixelType = GL_UNSIGNED_BYTE, bool repeat = true);

    /**
     * A simple wrapper to create a texture not using constructor
     * @param width texture width
     * @param height texture height
     * @param format texture format, GL_RGBA by default
     * @param internalFormat texture internal format, GL_RGBA by default
     * @param pixelType texture pixel tipe, GL_UNSIGNED_BYTE by default
     * @param repeat if texture should be repeated or clamped to the border, true by default
     * @return created texture
     */
    template<typename... Args>
    static std::shared_ptr<Texture> CreateTexture(Args&&... args)
    {
        try
        {
            return std::make_shared<Texture>(std::forward<Args>(args)...);
        }
        catch(Exception& e)
        {
            std::cerr << "Failed to create a texture. Reason: " << e.what();
            return nullptr;
        }
    }

    /**
     * @return texture OpenGL id
     */
    [[nodiscard]] inline unsigned int GetId() const
    {
        return id;
    }

    /**
     * @return path the texture was loaded from
     */
    [[nodiscard]] std::string GetPath() const
    {
        return path;
    }

    /**
     * Binds the texture
     */
    void Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    /**
     * Deleting texture when we are done
     */
    ~Texture();

private:
    Texture() = default;

private:
    int width = 0;
    int height = 0;
    std::string path;
    unsigned int id = 0;
};

#endif //OPENGLPONG_TEXTURE_H
