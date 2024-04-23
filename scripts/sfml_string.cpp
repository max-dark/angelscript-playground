#include "sfml_string.h"
#include "scripts_sfml.h"
#include "scripts_tools.h"

#include <string>


namespace scripts::sfml::system
{
using scripts::tools::check;
using scripts::tools::not_null;

void string_ctor(sf::String *memory)
{
    new(memory) sf::String{};
}

void string_ctor(sf::String *memory, const std::string &str)
{
    new(memory) sf::String{sf::String::fromUtf8(str.begin(), str.end())};
}

void string_ctor(sf::String *memory, const sf::String &str)
{
    new(memory) sf::String{str};
}

void string_dtor(sf::String *memory)
{
    memory->~String();
}

sf::String &string_assign(sf::String &left, const std::string &right)
{
    return (left = sf::String::fromUtf8(right.begin(), right.end()));
}

sf::String &string_add_assign(sf::String &left, const std::string &right)
{
    return (left += sf::String::fromUtf8(right.begin(), right.end()));
}

void RegisterString(asIScriptEngine *engine)
{
    const char *Type = "String";
    int r;

    asDWORD flags = asOBJ_VALUE | asGetTypeTraits<sf::String>();
    r = engine->RegisterObjectType(Type, sizeof(sf::String), flags);
    check(r, "sf::String");

    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, "void f()",
                                        asFUNCTIONPR(string_ctor, (sf::String * ), void), asCALL_CDECL_OBJFIRST);
    check(r, "string ctor()");

    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, "void f(const string &in)",
                                        asFUNCTIONPR(string_ctor, (sf::String * , const std::string&), void),
                                        asCALL_CDECL_OBJFIRST);
    check(r, "string ctor");

    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, "void f(const sf::String &in)",
                                        asFUNCTIONPR(string_ctor, (sf::String * , const sf::String&), void),
                                        asCALL_CDECL_OBJFIRST);
    check(r, "string ctor");

    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_DESTRUCT, "void f()",
                                        asFUNCTIONPR(string_dtor, (sf::String * ), void), asCALL_CDECL_OBJFIRST);
    check(r, "string dtor()");

    r = engine->RegisterObjectMethod(Type, "string to_string()", asMETHOD(sf::String, operator std::string),
                                     asCALL_THISCALL);
    check(r, "sf::string to_string()");

    r = engine->RegisterObjectMethod(Type, "String& opAssign(const String &in)",
                                     asMETHODPR(sf::String, operator=, (const sf::String&), sf::String &),
                                     asCALL_THISCALL);
    check(r, "assign");
    r = engine->RegisterObjectMethod(Type, "String& opAssign(const string &in)",
                                     asFUNCTIONPR(string_assign, (sf::String & , const std::string&), sf::String &),
                                     asCALL_CDECL_OBJFIRST);
    check(r, "assign");
    r = engine->RegisterObjectMethod(Type, "String& opAddAssign(const String &in)",
                                     asMETHODPR(sf::String, operator+=, (const sf::String&), sf::String &),
                                     asCALL_THISCALL);
    check(r, "assign");
    r = engine->RegisterObjectMethod(Type, "String& opAddAssign(const string &in)",
                                     asFUNCTIONPR(string_add_assign, (sf::String & , const std::string&), sf::String &),
                                     asCALL_CDECL_OBJFIRST);
    check(r, "assign");
}
} // namespace scripts::sfml::system
