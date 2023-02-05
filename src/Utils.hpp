//
// Created by Anton on 02.02.2023.
//

#ifndef OPENGLPONG_UTILS_HPP
#define OPENGLPONG_UTILS_HPP


#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <exception>


#include "inipp.h"
#include "Utils.hpp"

class Exception : public std::exception
{
public:

    /**
     * Creates exception
     * @param error error message
     */
    explicit Exception(std::string error) : message(std::move(error) + "\n") {};

    /**
     * Get error message
     * @return c-style const char array containing error message
     */
    [[nodiscard]] const char * what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};

constexpr void ASSERT(bool var, const std::string& message) { if (!var) { throw Exception(message); } }


namespace Utils
{
    void LoadConfig(int* width, int* height, std::string* name, std::string* scene);

    void LoadField(const std::string& scenePath, std::string* ballTexturePath, std::string* backgroundTexturePath, std::string* leftPaddleTexturePath, std::string* rightPaddleTexturePath);
}
#endif //OPENGLPONG_UTILS_HPP
