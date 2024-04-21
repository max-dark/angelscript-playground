#include "scripts_sfml.h"
#include "scripts_tools.h"

namespace scripts::sfml
{
using namespace scripts::tools;
namespace system
{
void Time_ctor(void* memory)
{
    new(memory) sf::Time{};
}
void Time_dtor(sf::Time* value)
{
    value->~Time();
}

void Register(asIScriptEngine* engine)
{
    // sf::Time
    // register as "Value type"
    {
        const char* Type = "Time";
        int r;
        r = engine->RegisterObjectType(Type, sizeof(sf::Time), asOBJ_VALUE | asGetTypeTraits<sf::Time>());
        check(r, "class sf::Time");
        // default ctor
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Time_ctor), asCALL_CDECL_OBJLAST);
        check(r, "sf::Time::Time()");
        // destructor
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Time_dtor), asCALL_CDECL_OBJLAST);;
        check(r, "sf::Time::~Time()");

        // methods

        r = engine->RegisterObjectMethod(Type, "float asSeconds() const", asMETHODPR(sf::Time, asSeconds, () const, float), asCALL_THISCALL);
        check(r, "sf::Time::asSeconds()");
        r = engine->RegisterObjectMethod(Type, "int32 asMilliseconds() const", asMETHODPR(sf::Time, asMilliseconds, () const, sf::Int32), asCALL_THISCALL);
        check(r, "sf::Time::asMilliseconds()");
        r = engine->RegisterObjectMethod(Type, "int64 asMicroseconds() const", asMETHODPR(sf::Time, asMicroseconds, () const, sf::Int64), asCALL_THISCALL);
        check(r, "sf::Time::asMicroseconds()");

        // global functions

        //r = engine->RegisterGlobalFunction("sf::Time seconds()", asFUNCTION(sf::seconds), asCALL_CDECL);
        //check(r, "sf::seconds");

        // operators
    }
}
}

int Register(asIScriptEngine *engine)
{
    int r;
    r = engine->SetDefaultNamespace("sf");
    check(r, "unable select namespace 'sf'");

    system::Register(engine);

    r = engine->SetDefaultNamespace("");
    check(r, "unable select namespace ''");

    return asSUCCESS;
}

} // namespace scripts::sfml
