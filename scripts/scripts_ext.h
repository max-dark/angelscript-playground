#ifndef SCRIPTS_EXT_H
#define SCRIPTS_EXT_H

#include <angelscript.h>
#include <angelscript/scriptbuilder/scriptbuilder.h>

namespace scripts::ext
{
    void registerExtensions(asIScriptEngine *engine);
}

#endif //SCRIPTS_EXT_H
