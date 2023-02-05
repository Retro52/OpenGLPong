//
// Created by Anton on 03.02.2023.
//

#ifndef OPENGLPONG_INTERFACEHANDLER_HPP
#define OPENGLPONG_INTERFACEHANDLER_HPP

#include <iostream>
#include <map>
#include <ft2build.h>
#include <memory>
#include FT_FREETYPE_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Shader.h"
#include "Utils.hpp"

struct Character
{
    glm::ivec2   size;      // Size of glyph
    glm::ivec2   bearing;   // Offset from baseline to left/top of glyph
    unsigned int advance = 0;   // Horizontal offset to advance to next glyph
    unsigned int textureId = 0; // ID handle of the glyph texture
};


class InterfaceHandler
{
private:
    inline static std::map<char, Character> characters;
    inline static unsigned int fontSizeOnLoad;
    inline static unsigned int VAO, VBO;

public:
    /* Restriction to create an instance of this class */
    InterfaceHandler() = delete;
    InterfaceHandler(InterfaceHandler&&) = delete;
    InterfaceHandler(const InterfaceHandler&) = delete;

    /**
     * Loads default font into textures
     * @param fontPath path to the font file (usually ends with .ttf)
     * @param fontSize default size of the font
     */
    static void Initialize(const std::string& fontPath, int fontSize)
    {
        FT_Library ft = nullptr;

        // All functions return a value different from 0 whenever an error occurred
        ASSERT(FT_Init_FreeType(&ft) == 0, "FreeType library initialization error");

        // load font as face
        FT_Face face = nullptr;
        ASSERT(FT_New_Face(ft, fontPath.c_str(), 0, &face) == 0, "Failed to load font");

        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, fontSize);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
            {
                continue;
            }

            // generate texture
            unsigned int texture = 0;
            
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    static_cast<int>(face->glyph->bitmap.width),
                    static_cast<int>(face->glyph->bitmap.rows),
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
            );
            
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            // now store character for later use
            Character character =
                    {
                            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                            static_cast<unsigned int>(face->glyph->advance.x),
                            texture
                    };
            
            characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        // set loaded loaded font var
        fontSizeOnLoad = fontSize;

        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        
        // generate vertex arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        // create space for further usage
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        
        // reset all
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    /**
     * Renders given string on the screen
     * @param shader shader to apply
     * @param text text to render
     * @param x x position on the screen
     * @param y y position on the screen
     * @param fontSize text font size, in pixels
     * @param color text color
     */
    static void RenderText(Shader& shader, const std::string& text, float x, float y, int fontSize = 16, const glm::vec3& color = glm::vec3(1.0f))
    {
        auto projection = glm::ortho((float) - Window::GetWidth() / 2,
                                     (float) + Window::GetWidth() / 2,
                                     (float) - Window::GetHeight() / 2,
                                     (float) + Window::GetHeight() / 2);

        // activate corresponding render state
        shader.Use();
        shader.setInt("text", 0);
        shader.setVec3("textColor", color);
        shader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        float scale = (float) fontSize / (float) fontSizeOnLoad;

        for (const auto& c : text)
        {
            Character ch = characters[c];

            float xPos = x + (float) ch.bearing.x * scale;
            float yPos = y - (float) (ch.size.y - ch.bearing.y) * scale;

            float w = (float) ch.size.x * scale;
            float h = (float) ch.size.y * scale;

            // update VBO for each character
            float vertices[6][4] =
            {
                    { xPos,     yPos + h,   0.0f, 0.0f },
                    { xPos,     yPos,       0.0f, 1.0f },
                    { xPos + w, yPos,       1.0f, 1.0f },

                    { xPos,     yPos + h,   0.0f, 0.0f },
                    { xPos + w, yPos,       1.0f, 1.0f },
                    { xPos + w, yPos + h,   1.0f, 0.0f }
            };

            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.textureId);

            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (float) (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    static float GetTextWidth(const std::string& text,int fontSize = 16)
    {
        float res = 0.0F;
        float scale = (float) fontSize / (float) fontSizeOnLoad;

        for (const auto& c : text)
        {
            Character ch = characters[c];
            res += (float) (ch.size.x + ch.bearing.x);
        }

        return res * scale;
    }
    /**
     * Renders texture as a quad on the screen
     * @param shader shader to apply
     * @param x OX position on the screen
     * @param y OY position on the screen
     * @param w texture width
     * @param h texture height
     * @param texture texture to render
     */
    static void RenderTexture(Shader& shader, float x, float y, float w, float h, unsigned int texture) {};
};


#endif //OPENGLPONG_INTERFACEHANDLER_HPP
