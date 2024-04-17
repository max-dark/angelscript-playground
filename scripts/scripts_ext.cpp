#include "scripts_ext.h"
#include <angelscript/scriptstdstring/scriptstdstring.h>
#include <angelscript/scriptarray/scriptarray.h>
#include <angelscript/scriptmath/scriptmath.h>
#include <angelscript/datetime/datetime.h>
#include <angelscript/scriptfile/scriptfile.h>
#include <angelscript/scriptfile/scriptfilesystem.h>


namespace scripts::ext
{

    void registerExtensions(asIScriptEngine *engine) {
        RegisterScriptArray(engine, true);
        RegisterStdString(engine);
        RegisterStdStringUtils(engine);
        RegisterScriptMath(engine);
        RegisterScriptDateTime(engine);
        RegisterScriptFile(engine);
        RegisterScriptFileSystem(engine);
    }
}

#include <angelscript/scriptstdstring/scriptstdstring.cpp>
#include <angelscript/scriptstdstring/scriptstdstring_utils.cpp>
#include <angelscript/scriptarray/scriptarray.cpp>
#include <angelscript/scriptbuilder/scriptbuilder.cpp>
#include <angelscript/scriptmath/scriptmath.cpp>
#include <angelscript/scriptfile/scriptfile.cpp>
#include <angelscript/scriptfile/scriptfilesystem.cpp>
#include <angelscript/datetime/datetime.cpp>