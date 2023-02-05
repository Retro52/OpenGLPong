#include "Utils.hpp"

namespace Utils
{
    void LoadConfig(int* width, int* height, std::string* name, std::string* scene)
    {
        std::ifstream is("config.ini");
        ASSERT(is.is_open(), "Failed to open config.ini");

        inipp::Ini<char> ini;

        ini.parse(is);
        ini.default_section(ini.sections["WINDOW"]);
        ini.interpolate();

        inipp::get_value(ini.sections["WINDOW"], "windowName", *name);
        inipp::get_value(ini.sections["WINDOW"], "windowWidth", *width);
        inipp::get_value(ini.sections["WINDOW"], "windowHeight", *height);

        inipp::get_value(ini.sections["SCENE"], "defaultScenePath", *scene);
    }

    void LoadField(const std::string& scenePath, std::string* ballTexturePath, std::string* backgroundTexturePath, std::string* leftPaddleTexturePath, std::string* rightPaddleTexturePath)
    {
        std::ifstream is(scenePath);
        ASSERT(is.is_open(), "Failed to open scene " + scenePath);

        inipp::Ini<char> ini;

        ini.parse(is);
        ini.default_section(ini.sections["DATA"]);
        ini.interpolate();

        inipp::get_value(ini.sections["DATA"], "ballTexturePath", *ballTexturePath);
        inipp::get_value(ini.sections["DATA"], "backgroundTexturePath", *backgroundTexturePath);
        inipp::get_value(ini.sections["DATA"], "leftPaddleTexturePath", *leftPaddleTexturePath);
        inipp::get_value(ini.sections["DATA"], "rightPaddleTexturePath", *rightPaddleTexturePath);
    }
}