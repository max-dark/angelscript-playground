#ifndef SCRIPTS_SFML_H
#define SCRIPTS_SFML_H

#include <angelscript.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace scripts::sfml
{
int Register(asIScriptEngine* engine);
} // namespace scripts::sfml

#endif //SCRIPTS_SFML_H
