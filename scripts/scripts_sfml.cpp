#include "scripts_sfml.h"
#include "scripts_tools.h"

namespace scripts::sfml
{
using namespace scripts::tools;
namespace system
{
void Register(asIScriptEngine* engine)
{
    // sf::Time
    {
        const char* Type = "Time";
        int r;
        r = engine->RegisterObjectType(Type, sizeof(sf::Time), asOBJ_VALUE | asOBJ_POD);
        check(r, "class sf::Time");
    }
}
}

int Register(asIScriptEngine *engine)
{
    try
    {
        int r;
        r = engine->SetDefaultNamespace("sf");
        check(r, "unable select namespace 'sf'");

        system::Register(engine);

        r = engine->SetDefaultNamespace("");
        check(r, "unable select namespace ''");
    }
    catch (const script_exception& e)
    {
        return e.get_code();
    }
    return asSUCCESS;
}

} // namespace scripts::sfml
