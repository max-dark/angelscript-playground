#include "sfml_clock.h"
#include "scripts_sfml.h"
#include "scripts_tools.h"

namespace scripts::sfml::system
{
void Clock_ctor(sf::Clock* memory)
{
    new(memory) sf::Clock{};
}
void Clock_dtor(sf::Clock* clock)
{
    clock->~Clock();
}

void RegisterClock(asIScriptEngine* engine)
{
    using scripts::tools::check;
    using scripts::tools::not_null;

    auto* typeInfo = engine->GetTypeInfoByDecl("sf::Time");
    not_null(typeInfo, "sf::Time should be registered");

    const char *Type = "Clock";
    int r;

    asDWORD flags = asOBJ_VALUE | asGetTypeTraits<sf::Clock>();
    r = engine->RegisterObjectType(Type, sizeof(sf::Clock), flags);
    check(r, "sf::Clock");

    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Clock_ctor), asCALL_CDECL_OBJFIRST);
    check(r, "sf::Clock::Clock()");
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Clock_dtor), asCALL_CDECL_OBJFIRST);
    check(r, "sf::Clock::~Clock()");

    r = engine->RegisterObjectMethod(Type, "sf::Time getElapsedTime() const", asMETHODPR(sf::Clock, getElapsedTime, () const, sf::Time), asCALL_THISCALL);
    check(r, "sf::Clock::getElapsedTime()");
    r = engine->RegisterObjectMethod(Type, "sf::Time restart()", asMETHODPR(sf::Clock, restart, (), sf::Time), asCALL_THISCALL);
    check(r, "sf::Clock::restart()");
}
} // namespace scripts::sfml::system
